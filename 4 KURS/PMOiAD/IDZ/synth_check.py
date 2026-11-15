# synth_check.py
import sys, warnings
import numpy as np
import pandas as pd
from scipy import stats
from scipy.stats import kstest
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import cross_val_score
from sklearn.preprocessing import LabelEncoder

warnings.filterwarnings("ignore")
pd.set_option("display.width", 160)

def hr(t):
    print("\n" + "="*70 + f"\n{t}\n" + "="*70)

def load(path):
    for sep in [",", ";", "\t"]:
        for enc in ["utf-8", "cp1251", "latin-1"]:
            try:
                df = pd.read_csv(path, sep=sep, encoding=enc)
                if df.shape[1] > 1:
                    return df
            except Exception:
                pass
    raise RuntimeError("не прочитать файл")

def test_uniformity(df, num_cols):
    """KS-тест на равномерность на [min, max] — сильный маркер генератора."""
    hr("1. ТЕСТ НА РАВНОМЕРНОСТЬ (uniform[min,max])")
    print("   p-value: если >0.05 — данные НЕ отвергают равномерность (подозрительно для реальных)")
    rows = []
    for c in num_cols:
        s = df[c].dropna().values
        lo, hi = s.min(), s.max()
        if hi <= lo: continue
        ks = kstest(s, "uniform", args=(lo, hi - lo))
        # также нормируем и смотрим на гистограмму
        rows.append({"col": c, "min": lo, "max": hi,
                     "p_uniform": round(ks.pvalue, 4),
                     "skew": round(stats.skew(s), 3),
                     "kurt": round(stats.kurtosis(s), 3)})
    res = pd.DataFrame(rows).sort_values("p_uniform", ascending=False)
    print(res.to_string(index=False))
    suspects = res[res["p_uniform"] > 0.05]["col"].tolist()
    print(f"\n  ⚠ Похожи на uniform (p>0.05): {suspects if suspects else 'нет'}")
    return suspects

def test_normality(df, num_cols):
    hr("2. ТЕСТ НА НОРМАЛЬНОСТЬ (Shapiro)")
    rows = []
    for c in num_cols:
        s = df[c].dropna()
        if len(s) < 8 or s.nunique() < 5: continue
        stat, p = stats.shapiro(s if len(s) <= 5000 else s.sample(5000, random_state=0))
        rows.append({"col": c, "p_normality": round(p, 4)})
    print(pd.DataFrame(rows).to_string(index=False))

def test_grid_pattern(df, num_cols):
    """Проверка: значения лежат 'на сетке' (шаг .1, .5, 1) — маркер round() в генераторе."""
    hr("3. ШАГ ДИСКРЕТИЗАЦИИ (grid check)")
    print("   Если >95% значений кратны 0.1 / 0.5 / 1 — вероятно, сгенерированы округлением")
    for c in num_cols:
        s = df[c].dropna().values
        r = []
        for step in [0.1, 0.5, 1.0]:
            frac = np.mean(np.isclose(np.mod(s, step), 0, atol=1e-9))
            r.append(f"{step}:{frac:.0%}")
        print(f"   {c:35s} -> " + ", ".join(r))

def test_bounds(df, num_cols):
    """Границы min/max — 'ровные' числа — маркер uniform(low, high)."""
    hr("4. 'РОВНЫЕ' ГРАНИЦЫ")
    for c in num_cols:
        s = df[c].dropna()
        print(f"   {c:35s} min={s.min():>10} max={s.max():>10} "
              f"(ровные? {'ДА' if s.min()==round(s.min()) and s.max()==round(s.max()) else 'частично'})")

def test_duplicate_rules(df):
    """Проверка детерминированных связей — производные столбцы и правила."""
    hr("5. ДЕТЕРМИНИРОВАННЫЕ СВЯЗИ (утечки / правила генерации)")
    if {"Weight (kg)", "Height (m)", "BMI"}.issubset(df.columns):
        calc = df["Weight (kg)"] / df["Height (m)"]**2
        err = np.abs(calc - df["BMI"])
        print(f"   BMI: max|ошибка| = {err.max():.6f}, средняя = {err.mean():.6f}")
        print(f"   -> BMI вычисляется из W,H: {'ДА (утечка!)' if err.max()<0.5 else 'нет'}")
    # другие пары: ищем функциональные зависимости через R^2
    num = df.select_dtypes(include=[np.number]).columns.tolist()
    print("\n   Пары с |r|>0.9 (почти функциональные связи — подозрительно):")
    corr = df[num].corr().abs()
    pairs = [(i,j,round(corr.loc[i,j],3)) for i in corr.index for j in corr.columns
             if i<j and corr.loc[i,j] > 0.9]
    print("   ", pairs if pairs else "нет")

def test_ml_separability(df):
    """
    'Adversarial validation': если RF легко отличает реальные от синтетики —
    тут не с чем сравнивать, поэтому смотрим на другое:
    RF с высокой CV accuracy предсказывает ОДИН признак по остальным -> правило-генератор.
    """
    hr("6. 'ПРАВИЛО-ГЕНЕРАТОР': можно ли предсказать столбец по остальным?")
    print("   Если RF даёт accuracy >0.95 — столбец сгенерирован по формуле/правилу из других.")
    num = df.select_dtypes(include=[np.number]).columns.tolist()
    cat = df.select_dtypes(include=["object","category","bool"]).columns.tolist()
    for target in num + cat:
        X = df.drop(columns=[target]).copy()
        for c in X.select_dtypes(include=["object","category","bool"]).columns:
            X[c] = LabelEncoder().fit_transform(X[c].astype(str))
        X = X.fillna(X.median(numeric_only=True))
        y = df[target]
        if y.dtype == object or str(y.dtype).startswith("category"):
            y_enc = LabelEncoder().fit_transform(y.astype(str))
        else:
            # биннуём числовую цель на 5 корзин
            if y.nunique() > 10:
                y_enc = pd.qcut(y, 5, labels=False, duplicates="drop")
            else:
                y_enc = y
        clf = RandomForestClassifier(n_estimators=100, random_state=0, n_jobs=-1)
        try:
            acc = cross_val_score(clf, X, y_enc, cv=3, scoring="accuracy").mean()
        except Exception:
            continue
        flag = "  <-- ПОДОЗРИТЕЛЬНО (правило!)" if acc > 0.95 else ""
        print(f"   {target:35s} CV acc = {acc:.3f}{flag}")

def test_class_balance(df, cat_cols):
    hr("7. БАЛАНС КАТЕГОРИЙ")
    for c in cat_cols:
        vc = df[c].value_counts(normalize=True)
        print(f"   {c}:")
        for k,v in vc.items():
            print(f"       {k:12s} {v:.3f}")
        if vc.max() - vc.min() < 0.05 and len(vc) <= 6:
            print("       ⚠ почти равномерный баланс — типично для random.choice")

def test_repeated_patterns(df):
    """Повторяющиеся шаблоны строк/значений (генераторы любят повторять)."""
    hr("8. ПОВТОРЫ И ШАБЛОНЫ")
    # повторяющиеся значения в категориальных
    for c in df.select_dtypes(include=["object","category"]).columns:
        top = df[c].value_counts().head(3)
        print(f"   {c}: топ-3 {dict(top)}")
    # уникальность строк
    print(f"   Уникальных строк: {df.drop_duplicates().shape[0]} / {len(df)}")
    # уникальность по числовым
    for c in df.select_dtypes(include=[np.number]).columns:
        u = df[c].nunique()
        if u < 20:
            print(f"   {c}: всего {u} уникальных значений -> {sorted(df[c].dropna().unique())[:20]}")

if __name__ == "__main__":
    df = load(sys.argv[1])
    num_cols = df.select_dtypes(include=[np.number]).columns.tolist()
    cat_cols = df.select_dtypes(include=["object","category","bool"]).columns.tolist()
    print(f"Загружено: {df.shape}")
    test_uniformity(df, num_cols)
    test_normality(df, num_cols)
    test_grid_pattern(df, num_cols)
    test_bounds(df, num_cols)
    test_duplicate_rules(df)
    test_ml_separability(df)
    test_class_balance(df, cat_cols)
    test_repeated_patterns(df)
    print("\n" + "="*70 + "\nИТОГ\n" + "="*70)
    print("""Интерпретация:
 • Много p_uniform > 0.05 + 'ровные' min/max → данные из uniform(low, high) — генератор.
 • kurt ≈ -1.2 у числовых → равномерное распределение (не реальное).
 • CV accuracy одного столбца по другим > 0.95 → правило-генератор.
 • BMI = W/H² идеально → производный (утечка), убирать из моделей.
 • Идеальный баланс категорий → random.choice, не реальные данные.
""")