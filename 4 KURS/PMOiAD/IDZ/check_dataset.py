# check_dataset.py
import sys
import warnings
import numpy as np
import pandas as pd
from scipy import stats
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score

warnings.filterwarnings("ignore")
pd.set_option("display.width", 160)
pd.set_option("display.max_columns", 50)

# ---------- утилиты ----------
def hr(title):
    print("\n" + "=" * 70)
    print(title)
    print("=" * 70)

def ok(cond, msg):
    print(("  [OK]     " if cond else "  [FAIL]   ") + msg)
    return cond

def warn(cond, msg):
    print(("  [OK]     " if cond else "  [WARN]   ") + msg)
    return cond

def load(path):
    # пробуем разные разделители/кодировки
    for sep in [",", ";", "\t"]:
        for enc in ["utf-8", "cp1251", "latin-1"]:
            try:
                df = pd.read_csv(path, sep=sep, encoding=enc)
                if df.shape[1] > 1:
                    print(f"  Загружено: sep='{sep}', encoding='{enc}', shape={df.shape}")
                    return df
            except Exception:
                continue
    raise RuntimeError("Не удалось прочитать файл")

# ---------- основная проверка ----------
def analyze(df):
    results = {}  # имя проверки -> (ok_bool, обязательная?)
    n, m = df.shape
    hr("1. ОБЩАЯ ИНФОРМАЦИЯ")
    print(f"  Строк: {n}, столбцов: {m}")
    print(f"  Память: {df.memory_usage(deep=True).sum() / 1024**2:.2f} MB")

    # типы столбцов
    num_cols = df.select_dtypes(include=[np.number]).columns.tolist()
    cat_cols = df.select_dtypes(include=["object", "category", "bool"]).columns.tolist()
    print(f"  Числовых признаков: {len(num_cols)} -> {num_cols}")
    print(f"  Категориальных:     {len(cat_cols)} -> {cat_cols}")

    hr("2. БАЗОВЫЕ ТРЕБОВАНИЯ")
    results["≥500 строк"]        = ok(n >= 500,  f"n = {n} (нужно ≥ 500)")
    results[">=3 числовых"]      = ok(len(num_cols) >= 3, f"числовых: {len(num_cols)} (нужно ≥ 3)")
    results[">=1 категориальный"]= ok(len(cat_cols) >= 1, f"категориальных: {len(cat_cols)} (нужно ≥ 1)")

    # пропуски и дубли
    miss = df.isna().mean().sort_values(ascending=False)
    bad_miss = miss[miss > 0.3]
    results["пропуски <30%"]     = warn(len(bad_miss) == 0,
                                        f"столбцов с >30% пропусков: {len(bad_miss)} {list(bad_miss.index)}")
    dup = df.duplicated().sum()
    results["дубликаты <5%"]     = warn(dup / max(n,1) < 0.05,
                                        f"дубликатов: {dup} ({dup/max(n,1):.1%})")

    # константные / почти константные
    const_cols = [c for c in df.columns if df[c].nunique(dropna=True) <= 1]
    near_const = [c for c in num_cols if df[c].std(skipna=True) == 0]
    results["нет констант"]      = warn(not (const_cols or near_const),
                                        f"константные: {const_cols + near_const}")

    hr("3. ОПИСАТЕЛЬНАЯ СТАТИСТИКА (числовые)")
    if num_cols:
        desc = df[num_cols].describe().T
        desc["CV"] = desc["std"] / desc["mean"].replace(0, np.nan)
        desc["skew"] = df[num_cols].skew()
        desc["kurt"] = df[num_cols].kurt()
        print(desc.round(3).to_string())
    else:
        print("  Числовых признаков нет — п.3 не применим")

    hr("4. РАСПРЕДЕЛЕНИЯ / НОРМАЛЬНОСТЬ")
    if num_cols:
        norm_tests = {}
        for c in num_cols:
            s = df[c].dropna()
            if len(s) < 8 or s.nunique() < 5:
                continue
            # Shapiro для n <= 5000, иначе нормальный тест D’Agostino
            if len(s) <= 5000:
                stat, p = stats.shapiro(s)
            else:
                stat, p = stats.normaltest(s)
            norm_tests[c] = p
        norm_df = pd.DataFrame(norm_tests.items(), columns=["col", "p_normality"])
        print(norm_df.round(4).to_string(index=False))
        non_normal = norm_df[norm_df["p_normality"] < 0.05]
        results["есть распределения"] = ok(len(norm_tests) >= 2,
                                           f"проверено на нормальность: {len(norm_tests)}")
        print(f"  (Ненормальных по тесту: {len(non_normal)} — это нормально, ANOVA/регрессия "
              f"работают и на ненормальных при большом n)")

    hr("5. ANOVA — пригодность")
    anova_ok = False
    anova_detail = []
    for c in cat_cols + [col for col in num_cols if df[col].nunique() <= 10]:
        vc = df[c].value_counts()
        groups = vc[vc >= 20]
        if len(groups) >= 2:
            anova_detail.append((c, len(groups), int(vc.max())))
            if len(groups) >= 3:
                anova_ok = True
    results["ANOVA возможен"] = ok(len(anova_detail) > 0,
        f"категориальных признаков с ≥2 группами (по ≥20 набл.): {anova_detail}")
    print("  Рекомендация: для ANOVA желательно ≥3 группы по ≥20 набл. "
          f"-> {'ДА' if anova_ok else 'можно только 2-групповой t-test'}")

    hr("6. КОРРЕЛЯЦИИ")
    corr_ok = False
    if len(num_cols) >= 2:
        corr = df[num_cols].corr(numeric_only=True)
        # максимальная корреляция по модулю вне диагонали
        c = corr.abs().where(~np.eye(len(corr), dtype=bool)).max()
        strong_pairs = [(i, j, round(corr.loc[i,j],3))
                        for i in corr.index for j in corr.columns
                        if i < j and abs(corr.loc[i,j]) > 0.3]
        print(f"  Сильные пары (|r|>0.3): {strong_pairs if strong_pairs else 'нет'}")
        corr_ok = len(strong_pairs) >= 1
        results["есть корреляции"] = warn(corr_ok, "хотя бы 1 пара с |r|>0.3")
    else:
        results["есть корреляции"] = ok(False, "меньше 2 числовых столбцов")

    hr("7. РЕГРЕССИЯ — пригодность")
    reg_ok = False
    reg_candidates = []
    for c in num_cols:
        s = df[c].dropna()
        if s.nunique() >= 10 and s.std() > 0:
            reg_candidates.append((c, int(s.nunique())))
    reg_ok = len(num_cols) >= 3 and len(reg_candidates) >= 1
    results["регрессия возможна"] = ok(reg_ok,
        f"кандидаты-цели (≥10 уникальных, разброс > 0): {reg_candidates[:5]}")
    print(f"  Предикторов для регрессии: {len(num_cols)-1}+ категориальных: {len(cat_cols)}")

    hr("8. КЛАСТЕРИЗАЦИЯ — пригодность")
    clus_ok = False
    if len(num_cols) >= 2:
        X = df[num_cols].dropna()
        if len(X) >= 50:
            Xs = StandardScaler().fit_transform(X)
            try:
                sil_scores = {}
                for k in range(2, min(6, len(X)//10) + 1):
                    km = KMeans(n_clusters=k, n_init=10, random_state=0).fit(Xs)
                    sil_scores[k] = silhouette_score(Xs, km.labels_)
                best_k = max(sil_scores, key=sil_scores.get)
                print(f"  Silhouette по k: { {k: round(v,3) for k,v in sil_scores.items()} }")
                print(f"  Лучший k={best_k} (silhouette={sil_scores[best_k]:.3f})")
                clus_ok = sil_scores[best_k] > 0.15
            except Exception as e:
                print("  K-means проверка не удалась:", e)
        else:
            print("  Слишком мало строк для кластеризации")
    results["кластеризация возможна"] = ok(clus_ok,
        "silhouette лучшего k > 0.15 (нет — кластеры слабые, но анализ формально можно провести)")

    hr("9. КЛАССИФИКАЦИЯ — пригодность")
    cls_ok = False
    cls_candidate = None
    # 1) уже есть бинарная переменная
    for c in num_cols + cat_cols:
        vc = df[c].value_counts(dropna=True)
        if len(vc) == 2 and vc.min() >= 30:
            cls_candidate = (c, "уже бинарная", dict(vc))
            cls_ok = True
            break
    # 2) иначе можно бинаризовать числовую
    if not cls_ok:
        for c in reg_candidates:
            col = c[0]
            med = df[col].median()
            mask = df[col] > med
            pos, neg = mask.sum(), (~mask).sum()
            if min(pos, neg) >= 30:
                cls_candidate = (col, f"бинаризация по медиане {med:.3g}", {"pos": int(pos), "neg": int(neg)})
                cls_ok = True
                break
    results["классификация возможна"] = ok(cls_ok, f"кандидат: {cls_candidate}")
    if cls_ok and isinstance(cls_candidate[2], dict):
        vals = list(cls_candidate[2].values())
        imbalance = max(vals)/min(vals)
        print(f"  Дисбаланс классов: {imbalance:.2f}:1 "
              f"({'OK' if imbalance < 10 else 'сильный — используйте class_weight и PR-AUC'})")

    # ---------- ИТОГ ----------
    hr("ИТОГОВЫЙ ВЕРДИКТ")
    mandatory = ["≥500 строк", ">=3 числовых", ">=1 категориальный",
                 "регрессия возможна", "классификация возможна"]
    core_ok = all(results[k] for k in mandatory if k in results)
    for k, v in results.items():
        mark = "OK " if v else "!! "
        print(f"  {mark}{k}: {'да' if v else 'НЕТ'}")

    hr("РЕКОМЕНДАЦИИ")
    if not results["≥500 строк"]:
        print("  • Мало строк — добавьте больше данных или возьмите репрезентативную подвыборку крупнее.")
    if not results[">=3 числовых"]:
        print("  • Нужно ≥3 числовых признака (или сгенерируйте/закодируйте категориальные).")
    if not results[">=1 категориальный"]:
        print("  • Нужен хотя бы один категориальный фактор (можно бинаризовать числовой для ANOVA).")
    if not results["регрессия возможна"]:
        print("  • Выберите/создайте непрерывную целевую для регрессии.")
    if not results["классификация возможна"]:
        print("  • Нет бинарной цели — сделайте её бинаризацией (например, выше/ниже медианы).")
    if not results["есть корреляции"]:
        print("  • Слабая корреляция — ожидаемо для реальных данных, но добавьте пару связанных признаков.")
    if not results["ANOVA возможен"]:
        print("  • Для ANOVA нужен фактор с ≥3 группами по ≥20 набл.")

    if core_ok:
        print("\n>>> ДАТАСЕТ ПОДХОДИТ для выполнения ИДЗ (при условии выполнения рекомендаций выше).")
    else:
        print("\n>>> ДАТАСЕТ, СКОРЕЕ ВСЕГО, НЕ ПОДХОДИТ без доработки. См. рекомендации.")

    return results, df

# ---------- main ----------
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Использование: python check_dataset.py path/to/dataset.csv")
        sys.exit(1)
    df = load(sys.argv[1])
    analyze(df)