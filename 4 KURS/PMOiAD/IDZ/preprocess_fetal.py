# preprocess_fetal.py
# Запуск: python3 preprocess_fetal.py fetal_health.csv
# Результат: fetal_health_clean.csv

import sys
import pandas as pd
import numpy as np


def load_data(path: str) -> pd.DataFrame:
    """Загрузка CSV с автоопределением разделителя и кодировки."""
    for sep in [",", ";", "\t"]:
        for enc in ["utf-8", "cp1251", "latin-1"]:
            try:
                df = pd.read_csv(path, sep=sep, encoding=enc)
                if df.shape[1] > 1:
                    print(f"[load] sep='{sep}', encoding='{enc}', shape={df.shape}")
                    return df
            except Exception:
                continue
    raise RuntimeError(f"Не удалось прочитать файл: {path}")


def clean_duplicates(df: pd.DataFrame) -> pd.DataFrame:
    """Удаление полных дубликатов строк."""
    before = len(df)
    df = df.drop_duplicates().reset_index(drop=True)
    after = len(df)
    print(f"[dedup] Удалено дубликатов: {before - after} ({before} -> {after})")
    return df


def add_categorical_features(df: pd.DataFrame) -> pd.DataFrame:
    """
    Создание категориальных и производных признаков
    для ANOVA / классификации / кластеризации.
    """
    df = df.copy()

    # ---------- 1) Читаемые метки классов (для отчётов и графиков) ----------
    health_map = {1: "Normal", 2: "Suspect", 3: "Pathological"}
    df["health_label"] = df["fetal_health"].map(health_map).astype("category")

    # ---------- 2) Бинарные цели для классификации ----------
    # Вариант A: норма vs всё остальное (дисбаланс ~3.5:1)
    df["is_abnormal"] = (df["fetal_health"] != 1).astype(int)
    # Вариант B: только патология vs всё остальное (дисбаланс ~12:1, для PR-AUC)
    df["is_pathological"] = (df["fetal_health"] == 3).astype(int)

    # ---------- 3) Категориальные факторы для ANOVA ----------
    # 3.1 Уровень базовой ЧСС (низкий / нормальный / высокий)
    df["baseline_group"] = pd.cut(
        df["baseline value"],
        bins=[0, 120, 140, 200],
        labels=["low", "normal", "high"],
    ).astype("category")

    # 3.2 Уровень вариабельности (короткой)
    df["variability_level"] = pd.cut(
        df["abnormal_short_term_variability"],
        bins=[0, 33, 66, 100],
        labels=["low", "medium", "high"],
    ).astype("category")

    # 3.3 Наличие пролонгированных замедлений (бинарный фактор)
    df["has_prolongued_decel"] = (df["prolongued_decelerations"] > 0).astype(int)

    # 3.4 Наличие тяжёлых замедлений (бинарный фактор)
    df["has_severe_decel"] = (df["severe_decelerations"] > 0).astype(int)

    # ---------- 4) Тренд гистограммы как категория ----------
    tend_map = {-1: "decreasing", 0: "stable", 1: "increasing"}
    df["tendency_cat"] = df["histogram_tendency"].map(tend_map).astype("category")

    # ---------- 5) Дискретизация числа пиков гистограммы ----------
    df["peaks_group"] = pd.cut(
        df["histogram_number_of_peaks"],
        bins=[-1, 2, 6, 20],
        labels=["few", "medium", "many"],
    ).astype("category")

    print("[features] Добавлены столбцы:")
    new_cols = [
        "health_label", "is_abnormal", "is_pathological",
        "baseline_group", "variability_level",
        "has_prolongued_decel", "has_severe_decel",
        "tendency_cat", "peaks_group",
    ]
    for c in new_cols:
        print(f"   - {c}: {df[c].dtype}, уникальных={df[c].nunique()}")

    return df


def report(df: pd.DataFrame) -> None:
    """Краткий отчёт по итогам предобработки."""
    print("\n" + "=" * 60)
    print("ИТОГОВЫЙ ОТЧЁТ")
    print("=" * 60)
    print(f"Строк: {len(df)}")
    print(f"Столбцов: {df.shape[1]}")
    print(f"\nТипы столбцов:")
    print(df.dtypes.value_counts().to_string())

    print(f"\nБаланс классов fetal_health:")
    print(df["fetal_health"].value_counts().sort_index().to_string())
    print(f"\nБаланс health_label:")
    print(df["health_label"].value_counts().to_string())

    print(f"\nДисбаланс is_abnormal (0=Normal, 1=Abnormal):")
    vc = df["is_abnormal"].value_counts().sort_index()
    print(vc.to_string())
    print(f"   соотношение: {vc.max() / vc.min():.2f} : 1")

    print(f"\nДисбаланс is_pathological (0=не патология, 1=патология):")
    vc = df["is_pathological"].value_counts().sort_index()
    print(vc.to_string())
    print(f"   соотношение: {vc.max() / vc.min():.2f} : 1")

    print(f"\nГруппы для ANOVA (baseline_group):")
    print(df["baseline_group"].value_counts().to_string())

    print(f"\nГруппы для ANOVA (variability_level):")
    print(df["variability_level"].value_counts().to_string())

    print("\nПроверка на пропуски после преобразований:")
    miss = df.isna().sum()
    miss = miss[miss > 0]
    if len(miss) == 0:
        print("   пропусков нет")
    else:
        print(miss.to_string())


def main():
    if len(sys.argv) < 2:
        print("Использование: python preprocess_fetal.py fetal_health.csv")
        sys.exit(1)

    path_in = sys.argv[1]
    path_out = "fetal_health_clean.csv"

    df = load_data(path_in)
    df = clean_duplicates(df)
    df = add_categorical_features(df)
    report(df)

    df.to_csv(path_out, index=False)
    print(f"\n[ save ] Очищенный датасет сохранён: {path_out}")
    print(f"[ info ] shape = {df.shape}")


if __name__ == "__main__":
    main()