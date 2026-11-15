# part1_preprocessing.py
# Запуск: python3 part1_preprocessing.py fetal_health.csv
# Артефакты: fetal_clean.csv, X_train.csv, X_test.csv, y_train.csv, y_test.csv, scaler.pkl

import sys
import os
import joblib
import numpy as np
import pandas as pd
from scipy import stats
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import RobustScaler, OrdinalEncoder

pd.set_option("display.width", 180)
pd.set_option("display.max_columns", 50)

# ============================================================
# ВСПОМОГАТЕЛЬНОЕ
# ============================================================
def hr(title):
    print("\n" + "=" * 78)
    print(title)
    print("=" * 78)


def save_table(df, name):
    """Сохраняет таблицу в CSV для вставки в отчёт."""
    os.makedirs("tables", exist_ok=True)
    path = f"tables/{name}.csv"
    df.to_csv(path, index=False, encoding="utf-8-sig")
    print(f"  [saved] {path}")


# ============================================================
# 1.1 ЗАГРУЗКА И СТРУКТУРА
# ============================================================
def load_and_inspect(path):
    hr("1.1 ЗАГРУЗКА И СТРУКТУРА ДАТАСЕТА")
    df = pd.read_csv(path)
    print(f"  Файл: {path}")
    print(f"  Размер: {df.shape[0]} строк × {df.shape[1]} столбцов")
    print(f"  Память: {df.memory_usage(deep=True).sum() / 1024**2:.3f} MB")

    print("\n  --- df.info() ---")
    df.info()

    print("\n  --- Первые 5 строк ---")
    print(df.head().to_string())

    # Таблица признаков: имя, тип, уникальных, min, max, пропуски
    rows = []
    for c in df.columns:
        s = df[c]
        rows.append({
            "Признак": c,
            "dtype": str(s.dtype),
            "n_unique": s.nunique(dropna=True),
            "min": s.min() if pd.api.types.is_numeric_dtype(s) else "-",
            "max": s.max() if pd.api.types.is_numeric_dtype(s) else "-",
            "n_missing": int(s.isna().sum()),
        })
    feat_table = pd.DataFrame(rows)
    print("\n  --- Таблица признаков ---")
    print(feat_table.to_string(index=False))
    save_table(feat_table, "1_1_features")
    return df


# ============================================================
# 1.2 ОБЪЁМ, КОЛИЧЕСТВО И ТИПЫ ПРИЗНАКОВ
# ============================================================
def describe_shape(df):
    hr("1.2 ОБЪЁМ ВЫБОРКИ И ТИПЫ ПРИЗНАКОВ")
    n, p = df.shape
    num = df.select_dtypes(include=[np.number]).columns.tolist()
    cat = df.select_dtypes(include=["object", "category", "bool"]).columns.tolist()

    print(f"  Объём выборки (n): {n}")
    print(f"  Количество признаков (p): {p}")
    print(f"  Числовых: {len(num)}")
    print(f"  Категориальных (object/category/bool): {len(cat)}")

    type_counts = df.dtypes.astype(str).value_counts()
    print("\n  Распределение типов:")
    print(type_counts.to_string())

    summary = pd.DataFrame({
        "Показатель": ["n (строк)", "p (столбцов)", "числовых", "категориальных"],
        "Значение": [n, p, len(num), len(cat)],
    })
    save_table(summary, "1_2_shape")
    return num, cat


# ============================================================
# 1.3 ПРОПУСКИ, ДУБЛИКАТЫ, АНОМАЛИИ, ВЫБРОСЫ
# ============================================================
def check_missing(df):
    hr("1.3а ПРОПУЩЕННЫЕ ЗНАЧЕНИЯ")
    miss = df.isna().sum()
    total = int(miss.sum())
    print(f"  Всего пропущенных значений: {total}")
    if total == 0:
        print("  → Пропусков нет, импутация не требуется.")
    else:
        print(miss[miss > 0].to_string())
    return total


def check_duplicates(df):
    hr("1.3б ДУБЛИРУЮЩИЕСЯ ЗАПИСИ")
    n_dup = int(df.duplicated().sum())
    print(f"  Полных дубликатов строк: {n_dup} ({n_dup / len(df):.2%})")
    df_clean = df.drop_duplicates().reset_index(drop=True)
    print(f"  После удаления: {len(df_clean)} строк (удалено {len(df) - len(df_clean)})")
    return df_clean, n_dup


def check_anomalies(df, num_cols):
    hr("1.3в АНОМАЛЬНЫЕ НАБЛЮДЕНИЯ (физиологические границы)")
    # Известные физиологические границы для КТГ-признаков
    rules = {
        "baseline value": (50, 250, "ЧСС плода 50–250 уд/мин"),
        "accelerations": (0, None, "число ускорений ≥ 0"),
        "fetal_movement": (0, None, "число движений ≥ 0"),
        "uterine_contractions": (0, None, "сокращения ≥ 0"),
        "light_decelerations": (0, None, "замедления ≥ 0"),
        "severe_decelerations": (0, None, "замедления ≥ 0"),
        "prolongued_decelerations": (0, None, "замедления ≥ 0"),
        "abnormal_short_term_variability": (0, 100, "% времени 0–100"),
        "percentage_of_time_with_abnormal_long_term_variability": (0, 100, "% времени 0–100"),
        "histogram_tendency": (-1, 1, "тренд ∈ {-1, 0, 1}"),
        "fetal_health": (1, 3, "класс ∈ {1, 2, 3}"),
    }
    rows = []
    for col, (lo, hi, desc) in rules.items():
        if col not in df.columns:
            continue
        s = df[col]
        bad_lo = int((s < lo).sum()) if lo is not None else 0
        bad_hi = int((s > hi).sum()) if hi is not None else 0
        rows.append({
            "Признак": col,
            "Правило": desc,
            "Нарушений снизу": bad_lo,
            "Нарушений сверху": bad_hi,
        })
    rep = pd.DataFrame(rows)
    print(rep.to_string(index=False))
    total_bad = rep["Нарушений снизу"].sum() + rep["Нарушений сверху"].sum()
    print(f"\n  Итого физиологически невозможных значений: {total_bad}")
    save_table(rep, "1_3c_anomalies")
    return total_bad


def detect_outliers(df, num_cols):
    hr("1.3г ВЫБРОСЫ (правило Тьюки: вне [Q1−1.5·IQR; Q3+1.5·IQR])")
    rows = []
    for c in num_cols:
        s = df[c].dropna()
        q1, q3 = s.quantile(0.25), s.quantile(0.75)
        iqr = q3 - q1
        lo, hi = q1 - 1.5 * iqr, q3 + 1.5 * iqr
        n_out = int(((s < lo) | (s > hi)).sum())
        rows.append({
            "Признак": c,
            "Q1": round(q1, 3),
            "Q3": round(q3, 3),
            "IQR": round(iqr, 3),
            "Нижняя граница": round(lo, 3),
            "Верхняя граница": round(hi, 3),
            "Выбросов": n_out,
            "%": round(100 * n_out / len(s), 2),
        })
    rep = pd.DataFrame(rows).sort_values("Выбросов", ascending=False)
    print(rep.to_string(index=False))
    save_table(rep, "1_3d_outliers")
    print("\n  ВАЖНО: выбросы в клинических данных отражают реальные")
    print("  тяжёлые патологии (severe/prolongued decelerations).")
    print("  Решение: НЕ удалять; для чувствительных к масштабу алгоритмов")
    print("  применять RobustScaler, для деревьев — масштаб не важен.")
    return rep


# ============================================================
# 1.4 ОБРАБОТКА ПРОПУСКОВ И КАТЕГОРИАЛЬНЫХ ПРИЗНАКОВ
# ============================================================
def add_categorical_features(df):
    hr("1.4 ПРЕОБРАЗОВАНИЕ КАТЕГОРИАЛЬНЫХ ПРИЗНАКОВ")
    df = df.copy()

    # 1.4.1 Читаемые метки классов
    health_map = {1: "Normal", 2: "Suspect", 3: "Pathological"}
    df["health_label"] = df["fetal_health"].map(health_map).astype("category")

    # 1.4.2 Бинарные цели для классификации
    df["is_abnormal"] = (df["fetal_health"] != 1).astype(int)
    df["is_pathological"] = (df["fetal_health"] == 3).astype(int)

    # 1.4.3 Категориальные факторы для ANOVA
    df["baseline_group"] = pd.cut(
        df["baseline value"], bins=[0, 120, 140, 200],
        labels=["low", "normal", "high"],
    ).astype("category")
    df["variability_level"] = pd.cut(
        df["abnormal_short_term_variability"], bins=[0, 33, 66, 100],
        labels=["low", "medium", "high"],
    ).astype("category")

    # 1.4.4 Бинарные факторы
    df["has_prolongued_decel"] = (df["prolongued_decelerations"] > 0).astype(int)
    df["has_severe_decel"] = (df["severe_decelerations"] > 0).astype(int)

    # 1.4.5 Категоризация тренда и пиков
    tend_map = {-1: "decreasing", 0: "stable", 1: "increasing"}
    df["tendency_cat"] = df["histogram_tendency"].map(tend_map).astype("category")
    df["peaks_group"] = pd.cut(
        df["histogram_number_of_peaks"], bins=[-1, 2, 6, 20],
        labels=["few", "medium", "many"],
    ).astype("category")

    new_cols = [
        "health_label", "is_abnormal", "is_pathological",
        "baseline_group", "variability_level",
        "has_prolongued_decel", "has_severe_decel",
        "tendency_cat", "peaks_group",
    ]
    print("  Созданные категориальные/производные признаки:")
    for c in new_cols:
        print(f"    - {c:25s} dtype={df[c].dtype}, уникальных={df[c].nunique()}")

    print("\n  Частоты health_label:")
    print(df["health_label"].value_counts().to_string())

    return df


# ============================================================
# 1.5 КОДИРОВАНИЕ, НОРМАЛИЗАЦИЯ, СТАНДАРТИЗАЦИЯ
# ============================================================
def encode_and_scale(df):
    hr("1.5 КОДИРОВАНИЕ, НОРМАЛИЗАЦИЯ И СТАНДАРТИЗАЦИЯ")

    df = df.copy()

    # --- 1.5.1 Ordinal encoding для порядковых категориальных ---
    ord_cols = ["baseline_group", "variability_level", "peaks_group"]
    # категории уже упорядочены через pd.Categorical; берём cat.codes
    for c in ord_cols:
        df[f"{c}_ord"] = df[c].cat.codes  # low/medium/high → 0/1/2
        print(f"  Ordinal: {c:20s} → {c}_ord (0/1/2)")

    # --- 1.5.2 One-Hot Encoding для номинального tendency_cat ---
    ohe = pd.get_dummies(df["tendency_cat"], prefix="tend", dtype=int)
    df = pd.concat([df, ohe], axis=1)
    print(f"  One-Hot: tendency_cat → {list(ohe.columns)}")

    # --- 1.5.3 Label encoding целевой переменной ---
    # health_label уже категориальна; для ML используем fetal_health (1/2/3) — уже числовой

    # --- 1.5.4 Определяем признаки для моделирования ---
    # исключаем цели и «сырые» категориальные дубли
    target_cols = ["fetal_health", "health_label", "is_abnormal", "is_pathological"]
    drop_cols = target_cols + ["tendency_cat"]  # сырой object-столбец
    # Оставляем только числовые признаки (включая *_ord и one-hot)
    feature_df = df.drop(columns=[c for c in drop_cols if c in df.columns])
    feature_df = feature_df.select_dtypes(include=[np.number])

    # Удаляем один из трёх коллинеарных: histogram_median и histogram_mode
    # (оставляем histogram_mean)
    for c in ["histogram_median", "histogram_mode"]:
        if c in feature_df.columns:
            feature_df = feature_df.drop(columns=[c])
            print(f"  Удалён коллинеарный признак: {c}")

    print(f"\n  Итоговая матрица признаков: {feature_df.shape}")
    print(f"  Признаки ({len(feature_df.columns)}): {list(feature_df.columns)}")

    return df, feature_df


def scale_features(feature_df, y, test_size=0.2, random_state=42):
    """
    Разделение train/test и обучение RobustScaler ТОЛЬКО на train
    (во избежание утечки данных).
    """
    hr("1.5б МАСШТАБИРОВАНИЕ (RobustScaler) — только на train")

    X_train, X_test, y_train, y_test = train_test_split(
        feature_df, y, test_size=test_size,
        random_state=random_state, stratify=y,
    )
    print(f"  Train: {X_train.shape}, Test: {X_test.shape}")
    print(f"  Дисбаланс в train: {y_train.value_counts().to_dict()}")
    print(f"  Дисбаланс в test:  {y_test.value_counts().to_dict()}")

    scaler = RobustScaler()
    X_train_scaled = pd.DataFrame(
        scaler.fit_transform(X_train),
        columns=X_train.columns, index=X_train.index,
    )
    X_test_scaled = pd.DataFrame(
        scaler.transform(X_test),
        columns=X_test.columns, index=X_test.index,
    )

    print("\n  Пример масштабирования (первые 3 признака, медиана/масштаб):")
    print(pd.DataFrame({
        "Признак": X_train.columns[:3],
        "Медиана (train)": scaler.center_[:3],
        "IQR (train)": scaler.scale_[:3],
    }).to_string(index=False))

    return X_train, X_test, X_train_scaled, X_test_scaled, y_train, y_test, scaler


# ============================================================
# 1.6 ФОРМИРОВАНИЕ ВЫБОРКИ
# ============================================================
def make_samples(df, feature_df, y, X_train_scaled, X_test_scaled, y_train, y_test, scaler):
    hr("1.6 ФОРМИРОВАНИЕ ВЫБОРОК")
    os.makedirs("data", exist_ok=True)

    # Полный очищенный датасет
    df.to_csv("fetal_clean.csv", index=False)
    print("  [saved] fetal_clean.csv — полный очищенный датасет (n=2113)")

    # Для ML
    X_train_scaled.to_csv("data/X_train_scaled.csv", index=False)
    X_test_scaled.to_csv("data/X_test_scaled.csv", index=False)
    y_train.to_csv("data/y_train.csv", index=False)
    y_test.to_csv("data/y_test.csv", index=False)
    joblib.dump(scaler, "data/scaler.pkl")
    print("  [saved] data/X_train_scaled.csv, X_test_scaled.csv, y_train.csv, y_test.csv, scaler.pkl")

    print("\n  Для статистического анализа (п.2–6): используется весь fetal_clean.csv (n=2113)")
    print("  Для ML (п.7–9): X_train_scaled (n≈1690), X_test_scaled (n≈423)")


# ============================================================
# MAIN
# ============================================================
def main():
    if len(sys.argv) < 2:
        print("Использование: python part1_preprocessing.py fetal_health.csv")
        sys.exit(1)

    path = sys.argv[1]
    os.makedirs("tables", exist_ok=True)
    os.makedirs("data", exist_ok=True)

    # 1.1
    df = load_and_inspect(path)
    # 1.2
    num_cols, cat_cols = describe_shape(df)
    # 1.3
    check_missing(df)
    df_clean, n_dup = check_duplicates(df)
    check_anomalies(df_clean, num_cols)
    detect_outliers(df_clean, num_cols)
    # 1.4
    df_feat = add_categorical_features(df_clean)
    # 1.5
    df_full, feature_df = encode_and_scale(df_feat)
    # Целевая для классификации — is_abnormal
    y = df_full["is_abnormal"]
    X_train, X_test, X_train_scaled, X_test_scaled, y_train, y_test, scaler = \
        scale_features(feature_df, y)
    # 1.6
    make_samples(df_full, feature_df, y, X_train_scaled, X_test_scaled, y_train, y_test, scaler)

    hr("ГОТОВО")
    print("  Все таблицы для отчёта: ./tables/")
    print("  Все данные для следующих шагов: ./data/ и fetal_clean.csv")


if __name__ == "__main__":
    main()