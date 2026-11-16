# part5_correlation.py
# Запуск: python3 part5_correlation.py fetal_clean.csv
# Артефакты: tables/5_*.csv, figures/5_*.png

import sys
import os
import numpy as np
import pandas as pd
from scipy import stats
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import seaborn as sns
import warnings
warnings.filterwarnings("ignore")

# ============================================================
# ЧИСЛОВЫЕ ПРИЗНАКИ ДЛЯ АНАЛИЗА
# ============================================================
# Исключаем цели, one-hot, ordinal-коды и бинарные флаги
EXCLUDE = {
    "fetal_health", "health_label", "is_abnormal", "is_pathological",
    "baseline_group_ord", "variability_level_ord", "peaks_group_ord",
    "tend_decreasing", "tend_increasing", "tend_stable",
    "has_prolongued_decel", "has_severe_decel",
    "histogram_tendency"  # дискретный, не для корреляций
}


def hr(title):
    print("\n" + "=" * 80)
    print(title)
    print("=" * 80)


def save_table(df, name):
    os.makedirs("tables", exist_ok=True)
    path = f"tables/{name}.csv"
    df.to_csv(path, encoding="utf-8-sig")
    print(f"  [saved] {path}")


def save_fig(name):
    os.makedirs("figures", exist_ok=True)
    path = f"figures/{name}.png"
    plt.tight_layout()
    plt.savefig(path, dpi=120, bbox_inches="tight")
    plt.close()
    print(f"  [saved] {path}")


# ============================================================
# 5.1 РАСЧЁТ МАТРИЦ КОРРЕЛЯЦИЙ
# ============================================================
def compute_correlations(df, num_cols):
    hr("5.1 РАСЧЁТ МАТРИЦ КОРРЕЛЯЦИЙ (Пирсон и Спирмен)")
    # Пирсон (линейная связь)
    corr_p = df[num_cols].corr(method="pearson")
    # Спирмен (монотонная связь, ранговый)
    corr_s = df[num_cols].corr(method="spearman")
    
    print(f"Матрица Пирсона: {corr_p.shape}")
    print(f"Матрица Спирмена: {corr_s.shape}")
    
    save_table(corr_p.round(4), "5_1_corr_pearson")
    save_table(corr_s.round(4), "5_1_corr_spearman")
    return corr_p, corr_s


# ============================================================
# 5.2 ОЦЕНКА ЗНАЧИМОСТИ (p-value для пар)
# ============================================================
def significance_table(df, num_cols):
    hr("5.2 ОЦЕНКА СТАТИСТИЧЕСКОЙ ЗНАЧИМОСТИ (попарно)")
    rows = []
    n = len(num_cols)
    for i in range(n):
        for j in range(i + 1, n):
            c1, c2 = num_cols[i], num_cols[j]
            # Убираем пропуски в паре
            data = df[[c1, c2]].dropna()
            if len(data) < 3:
                continue
            # Пирсон
            r_p, p_p = stats.pearsonr(data[c1], data[c2])
            # Спирмен
            r_s, p_s = stats.spearmanr(data[c1], data[c2])
            
            # Определяем силу связи (по шкале Чеддока для |r|)
            abs_r = abs(r_s)
            if abs_r < 0.3: strength = "слабая"
            elif abs_r < 0.5: strength = "умеренная"
            elif abs_r < 0.7: strength = "заметная"
            elif abs_r < 0.9: strength = "высокая"
            else: strength = "очень высокая"
            
            direction = "прямая" if r_s > 0 else "обратная"
            
            rows.append({
                "Признак 1": c1, "Признак 2": c2,
                "r (Пирсон)": round(r_p, 4), "p (Пирсон)": f"{p_p:.2e}",
                "rho (Спирмен)": round(r_s, 4), "p (Спирмен)": f"{p_s:.2e}",
                "Сила (по Спирмену)": strength,
                "Направление": direction,
                "Значимо": "ДА" if p_s < 0.05 else "НЕТ"
            })
    rep = pd.DataFrame(rows)
    # Сортируем по убыванию модуля Спирмена
    rep["_abs_rho"] = rep["rho (Спирмен)"].abs()
    rep = rep.sort_values("_abs_rho", ascending=False).drop(columns=["_abs_rho"])
    print(f"Всего пар: {len(rep)}. Топ-10 по |rho|:")
    print(rep.head(10).to_string(index=False))
    save_table(rep, "5_2_pairwise_significance")
    return rep


# ============================================================
# 5.3 ТОП ПОЛОЖИТЕЛЬНЫХ И ОТРИЦАТЕЛЬНЫХ КОРРЕЛЯЦИЙ
# ============================================================
def top_correlations(rep, top_n=5):
    hr(f"5.3 ТОП-{top_n} ПОЛОЖИТЕЛЬНЫХ И ОТРИЦАТЕЛЬНЫХ КОРРЕЛЯЦИЙ")
    positive = rep[rep["rho (Спирмен)"] > 0].head(top_n)
    negative = rep[rep["rho (Спирмен)"] < 0].sort_values("rho (Спирмен)").head(top_n)
    
    print(f"\n--- Топ-{top_n} ПОЛОЖИТЕЛЬНЫХ (Спирмен) ---")
    print(positive[["Признак 1", "Признак 2", "rho (Спирмен)", "p (Спирмен)", "Сила (по Спирмену)"]].to_string(index=False))
    
    print(f"\n--- Топ-{top_n} ОТРИЦАТЕЛЬНЫХ (Спирмен) ---")
    print(negative[["Признак 1", "Признак 2", "rho (Спирмен)", "p (Спирмен)", "Сила (по Спирмену)"]].to_string(index=False))
    
    return positive, negative


# ============================================================
# 5.4 ТЕПЛОВЫЕ КАРТЫ
# ============================================================
def plot_heatmaps(corr_p, corr_s):
    hr("5.4 ТЕПЛОВЫЕ КАРТЫ КОРРЕЛЯЦИЙ")
    
    for name, corr in [("pearson", corr_p), ("spearman", corr_s)]:
        fig, ax = plt.subplots(figsize=(14, 12))
        # Маска для верхнего треугольника (чтобы не дублировать)
        mask = np.triu(np.ones_like(corr, dtype=bool), k=1)
        sns.heatmap(corr, mask=mask, annot=True, fmt=".2f",
                    cmap="RdBu_r", center=0, vmin=-1, vmax=1,
                    square=True, linewidths=0.5,
                    cbar_kws={"shrink": 0.8}, ax=ax,
                    annot_kws={"size": 7})
        ax.set_title(f"Матрица корреляций ({name.capitalize()})", fontsize=14)
        plt.xticks(rotation=45, ha="right", fontsize=8)
        plt.yticks(fontsize=8)
        save_fig(f"5_4_heatmap_{name}")


# ============================================================
# 5.5 ДИАГРАММЫ РАССЕЯНИЯ ДЛЯ САМЫХ СИЛЬНЫХ ПАР
# ============================================================
def plot_scatter(df, pairs, title_prefix):
    hr(f"5.5 ДИАГРАММЫ РАССЕЯНИЯ ({title_prefix})")
    for k, (c1, c2) in enumerate(pairs, 1):
        s = df[[c1, c2]].dropna()
        if len(s) < 3:
            continue
        fig, ax = plt.subplots(figsize=(7, 5))
        ax.scatter(s[c1], s[c2], alpha=0.3, s=10, color="magenta", edgecolor="none")
        # Линия тренда (полином 1-й степени)
        try:
            z = np.polyfit(s[c1], s[c2], 1)
            p = np.poly1d(z)
            x_sorted = np.sort(s[c1].values)
            ax.plot(x_sorted, p(x_sorted), "r-", lw=2, label="Линейный тренд")
        except Exception:
            pass
        ax.set_xlabel(c1)
        ax.set_ylabel(c2)
        ax.set_title(f"{c1} vs {c2}")
        ax.grid(alpha=0.3)
        ax.legend(fontsize=8)
        save_fig(f"5_5_scatter_{title_prefix}_{k}_{c1}_vs_{c2}".replace(" ", "_"))


# ============================================================
# MAIN
# ============================================================
def main():
    if len(sys.argv) < 2:
        print("Использование: python part5_correlation.py fetal_clean.csv")
        sys.exit(1)
    df = pd.read_csv(sys.argv[1])
    
    # Определяем числовые признаки
    all_num = df.select_dtypes(include=[np.number]).columns.tolist()
    num_cols = [c for c in all_num if c not in EXCLUDE]
    print(f"Числовых признаков для корреляционного анализа: {len(num_cols)}")
    
    corr_p, corr_s = compute_correlations(df, num_cols)
    rep = significance_table(df, num_cols)
    pos, neg = top_correlations(rep, top_n=5)
    plot_heatmaps(corr_p, corr_s)
    
    # Диаграммы рассеяния для топ-3 положительных и топ-3 отрицательных
    top_pos_pairs = list(zip(pos["Признак 1"].head(3), pos["Признак 2"].head(3)))
    top_neg_pairs = list(zip(neg["Признак 1"].head(3), neg["Признак 2"].head(3)))
    
    plot_scatter(df, top_pos_pairs, "top_positive")
    plot_scatter(df, top_neg_pairs, "top_negative")
    
    hr("ГОТОВО")
    print("  Таблицы: ./tables/5_*.csv")
    print("  Графики: ./figures/5_*.png")


if __name__ == "__main__":
    main()