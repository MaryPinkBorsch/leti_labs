# part2_descriptive.py
# Запуск: python3 part2_descriptive.py fetal_clean.csv
# Артефакты: tables/2_*.csv, figures/2_*.png

import sys
import os
import numpy as np
import pandas as pd
from scipy import stats
import matplotlib
matplotlib.use("Agg")  # без GUI, чтобы работало на сервере
import matplotlib.pyplot as plt

pd.set_option("display.width", 200)
pd.set_option("display.max_columns", 50)

# ============================================================
# ВСПОМОГАТЕЛЬНОЕ
# ============================================================
def hr(title):
    print("\n" + "=" * 80)
    print(title)
    print("=" * 80)


def save_table(df, name):
    os.makedirs("tables", exist_ok=True)
    path = f"tables/{name}.csv"
    df.to_csv(path, index=False, encoding="utf-8-sig")
    print(f"  [saved] {path}")


def save_fig(name):
    os.makedirs("figures", exist_ok=True)
    path = f"figures/{name}.png"
    plt.tight_layout()
    plt.savefig(path, dpi=120, bbox_inches="tight")
    plt.close()
    print(f"  [saved] {path}")


# ============================================================
# 2.1 ЧИСЛОВЫЕ ПРИЗНАКИ: ПОЛНАЯ ОПИСАТЕЛЬНАЯ СТАТИСТИКА
# ============================================================
def descriptive_numeric(df, num_cols):
    hr("2.1 ОПИСАТЕЛЬНАЯ СТАТИСТИКА ЧИСЛОВЫХ ПРИЗНАКОВ")
    rows = []
    for c in num_cols:
        s = df[c].dropna()
        n = len(s)
        mean = s.mean()
        median = s.median()
        # мода: может быть несколько; берём первую
        mode_vals = s.mode()
        mode = mode_vals.iloc[0] if len(mode_vals) else np.nan
        vmin, vmax = s.min(), s.max()
        rng = vmax - vmin
        var = s.var(ddof=1)          # несмещённая дисперсия
        std = s.std(ddof=1)
        cv = std / mean if mean != 0 else np.nan
        q1, q3 = s.quantile(0.25), s.quantile(0.75)
        iqr = q3 - q1
        skew = stats.skew(s, bias=False)
        kurt = stats.kurtosis(s, bias=False)  # excess kurtosis (норма = 0)
        rows.append({
            "Признак": c,
            "n": n,
            "Среднее": mean,
            "Медиана": median,
            "Мода": mode,
            "Min": vmin,
            "Max": vmax,
            "Размах": rng,
            "Дисперсия": var,
            "Ст.отклонение": std,
            "CV": cv,
            "Q1": q1,
            "Q3": q3,
            "IQR": iqr,
            "Асимметрия": skew,
            "Эксцесс": kurt,
        })
    rep = pd.DataFrame(rows)
    # округление для читаемости
    num_fmt = ["Среднее", "Медиана", "Мода", "Min", "Max", "Размах",
               "Дисперсия", "Ст.отклонение", "CV", "Q1", "Q3", "IQR",
               "Асимметрия", "Эксцесс"]
    for c in num_fmt:
        rep[c] = rep[c].round(4)
    print(rep.to_string(index=False))
    save_table(rep, "2_1_numeric_descriptive")
    return rep


# ============================================================
# 2.2 КАТЕГОРИАЛЬНЫЕ ПРИЗНАКИ: ТАБЛИЦЫ ЧАСТОТ
# ============================================================
def frequency_tables(df, cat_cols):
    hr("2.2 ТАБЛИЦЫ ЧАСТОТ И ОТНОСИТЕЛЬНЫХ ЧАСТОТ (категориальные)")
    all_rows = []
    for c in cat_cols:
        vc = df[c].value_counts(dropna=False)
        freq = pd.DataFrame({
            "Признак": c,
            "Значение": vc.index.astype(str),
            "Частота (абс.)": vc.values,
            "Частота (отн.)": (vc.values / vc.values.sum()).round(4),
            "Накопленная отн.": (vc.values / vc.values.sum()).cumsum().round(4),
        })
        print(f"\n--- {c} ---")
        print(freq.to_string(index=False))
        all_rows.append(freq)
    full = pd.concat(all_rows, ignore_index=True)
    save_table(full, "2_2_frequency_tables")
    return full


# ============================================================
# 2.3 ГИСТОГРАММЫ
# ============================================================
def plot_histograms(df, num_cols):
    hr("2.3 ГИСТОГРАММЫ ЧИСЛОВЫХ ПРИЗНАКОВ")
    # разбиваем на группы по 6 для читаемости
    for i in range(0, len(num_cols), 6):
        chunk = num_cols[i:i + 6]
        fig, axes = plt.subplots(2, 3, figsize=(15, 8))
        axes = axes.flatten()
        for j, c in enumerate(chunk):
            ax = axes[j]
            ax.hist(df[c].dropna(), bins=30, color="steelblue", edgecolor="black", alpha=0.8)
            ax.set_title(c, fontsize=9)
            ax.set_ylabel("Частота")
            ax.grid(alpha=0.3)
        # скрыть пустые оси
        for k in range(len(chunk), len(axes)):
            axes[k].axis("off")
        save_fig(f"2_3_histograms_part{i // 6 + 1}")


# ============================================================
# 2.4 ДИАГРАММЫ РАЗМАХА (BOXPLOT)
# ============================================================
def plot_boxplots(df, num_cols):
    hr("2.4 ДИАГРАММЫ РАЗМАХА (BOXPLOT)")
    # Для наглядности — по 6 признаков на рисунок.
    # Масштабы разные, поэтому используем отдельные оси Y.
    for i in range(0, len(num_cols), 6):
        chunk = num_cols[i:i + 6]
        fig, axes = plt.subplots(2, 3, figsize=(15, 8))
        axes = axes.flatten()
        for j, c in enumerate(chunk):
            ax = axes[j]
            ax.boxplot(df[c].dropna(), vert=True, patch_artist=True,
                       boxprops=dict(facecolor="lightblue"),
                       medianprops=dict(color="red"))
            ax.set_title(c, fontsize=9)
            ax.set_ylabel("Значение")
            ax.grid(alpha=0.3)
        for k in range(len(chunk), len(axes)):
            axes[k].axis("off")
        save_fig(f"2_4_boxplots_part{i // 6 + 1}")


# ============================================================
# 2.5 ДОВЕРИТЕЛЬНЫЕ ИНТЕРВАЛЫ 95% ДЛЯ СРЕДНИХ
# ============================================================
def ci_means(df, num_cols, confidence=0.95):
    hr(f"2.5 ДОВЕРИТЕЛЬНЫЕ ИНТЕРВАЛЫ {int(confidence*100)}% ДЛЯ СРЕДНИХ")
    rows = []
    for c in num_cols:
        s = df[c].dropna()
        n = len(s)
        mean = s.mean()
        std = s.std(ddof=1)
        se = std / np.sqrt(n)                       # стандартная ошибка
        t_crit = stats.t.ppf((1 + confidence) / 2, df=n - 1)
        ci_low = mean - t_crit * se
        ci_high = mean + t_crit * se
        rows.append({
            "Признак": c,
            "n": n,
            "Среднее": round(mean, 4),
            "Ст.ошибка (SE)": round(se, 4),
            "t-крит": round(t_crit, 4),
            "ДИ_нижн": round(ci_low, 4),
            "ДИ_верх": round(ci_high, 4),
            "Ширина ДИ": round(ci_high - ci_low, 4),
        })
    rep = pd.DataFrame(rows)
    print(rep.to_string(index=False))
    save_table(rep, "2_5_confidence_intervals")
    return rep


# ============================================================
# 2.6 ДОПОЛНИТЕЛЬНО: СТОЛБЧАТЫЕ ДИАГРАММЫ ЧАСТОТ
# ============================================================
def plot_frequency_bars(df, cat_cols):
    hr("2.6 СТОЛБЧАТЫЕ ДИАГРАММЫ ЧАСТОТ (категориальные)")
    for c in cat_cols:
        vc = df[c].value_counts(dropna=False)
        fig, ax = plt.subplots(figsize=(7, 4))
        ax.bar(vc.index.astype(str), vc.values, color="teal", edgecolor="black")
        ax.set_title(f"Распределение: {c}")
        ax.set_ylabel("Частота")
        ax.grid(axis="y", alpha=0.3)
        for i, v in enumerate(vc.values):
            ax.text(i, v, str(v), ha="center", va="bottom", fontsize=9)
        save_fig(f"2_6_freq_{c.replace(' ', '_')}")


# ============================================================
# MAIN
# ============================================================
def main():
    if len(sys.argv) < 2:
        print("Использование: python part2_descriptive.py fetal_clean.csv")
        sys.exit(1)
    path = sys.argv[1]
    df = pd.read_csv(path)
    print(f"Загружено: {df.shape}")

    # Определяем столбцы
    # исходные числовые (21): исключаем цели, ordinal-коды, one-hot, бинарные флаги
    exclude = {
        "fetal_health", "health_label", "is_abnormal", "is_pathological",
        "baseline_group_ord", "variability_level_ord", "peaks_group_ord",
        "tend_decreasing", "tend_increasing", "tend_stable",
        "has_prolongued_decel", "has_severe_decel",
    }
    num_cols = [c for c in df.select_dtypes(include=[np.number]).columns
                if c not in exclude]
    # категориальные: исходные созданные (строки)
    cat_cols = [c for c in ["health_label", "baseline_group", "variability_level",
                            "tendency_cat", "peaks_group"]
                if c in df.columns]

    print(f"Числовых признаков для анализа: {len(num_cols)}")
    print(f"Категориальных признаков: {len(cat_cols)}")

    descriptive_numeric(df, num_cols)
    frequency_tables(df, cat_cols)
    plot_histograms(df, num_cols)
    plot_boxplots(df, num_cols)
    ci_means(df, num_cols)
    plot_frequency_bars(df, cat_cols)

    hr("ГОТОВО")
    print("  Таблицы: ./tables/2_*.csv")
    print("  Графики: ./figures/2_*.png")


if __name__ == "__main__":
    main()