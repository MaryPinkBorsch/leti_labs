# part4_anova.py
# Запуск: python3 part4_anova.py fetal_clean.csv
# Артефакты: tables/4_*.csv, figures/4_*.png

import sys
import os
import numpy as np
import pandas as pd
from scipy import stats
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import warnings
warnings.filterwarnings("ignore")

# ============================================================
# ПАРЫ (Фактор → Числовой показатель) ДЛЯ АНАЛИЗА
# ============================================================
ANALYSES = [
    {"target": "abnormal_short_term_variability", "factor": "variability_level"},
    {"target": "baseline value", "factor": "health_label"},
    {"target": "histogram_width", "factor": "peaks_group"}
]


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
# 4.1 ВЫПОЛНЕНИЕ ANOVA И ПРОВЕРКА ПРЕДПОСЫЛОК
# ============================================================
def run_anova(df, target, factor):
    hr(f"АНАЛИЗ: Фактор '{factor}' → Показатель '{target}'")
    
    # Убираем пропуски в паре
    data = df[[target, factor]].dropna()
    groups = [group[target].values for name, group in data.groupby(factor, observed=True)]
    group_names = [str(name) for name, group in data.groupby(factor, observed=True)]
    
    # 1. Описательная статистика по группам
    desc = data.groupby(factor, observed=True)[target].agg(
        ["count", "mean", "std", "median"]
    ).reset_index()
    print("\n--- Описательная статистика по группам ---")
    print(desc.to_string(index=False))
    
    # 2. Проверка предпосылки: равенство дисперсий (тест Левена)
    # H0: дисперсии равны
    lev_stat, lev_p = stats.levene(*groups)
    print(f"\n[Предпосылка] Тест Левена (однородность дисперсий):")
    print(f"  Статистика = {lev_stat:.4f}, p-value = {lev_p:.4e}")
    print(f"  Вывод: дисперсии {'равны (предпосылка ОК)' if lev_p > 0.05 else 'НЕ равны (предпосылка нарушена)'}")
    
    # 3. Классический однофакторный ANOVA
    f_stat, anova_p = stats.f_oneway(*groups)
    
    # 4. Оценка размера эффекта (Eta-squared)
    # Eta^2 = SS_between / SS_total
    all_vals = data[target].values
    grand_mean = all_vals.mean()
    ss_total = np.sum((all_vals - grand_mean)**2)
    ss_between = sum(len(g) * (g.mean() - grand_mean)**2 for g in groups)
    ss_within = ss_total - ss_between
    eta_sq = ss_between / ss_total if ss_total > 0 else 0
    
    # 5. Альтернатива (непараметрический тест Краскела-Уоллиса)
    kw_stat, kw_p = stats.kruskal(*groups)
    
    print(f"\n--- Результаты тестов ---")
    print(f"ANOVA (F-критерий): F = {f_stat:.4f}, p-value = {anova_p:.4e}")
    print(f"Kruskal-Wallis:     H = {kw_stat:.4f}, p-value = {kw_p:.4e}")
    print(f"Размер эффекта (Eta^2): {eta_sq:.4f}")
    
    # 6. Пост-хок анализ (если различия значимы)
    # Используем Mann-Whitney U с поправкой Бонферрони
    posthoc_rows = []
    if kw_p < 0.05:
        from itertools import combinations
        n_pairs = len(groups) * (len(groups) - 1) // 2
        alpha_corrected = 0.05 / n_pairs
        for (i, g1), (j, g2) in combinations(enumerate(group_names), 2):
            u_stat, u_p = stats.mannwhitneyu(groups[i], groups[j], alternative="two-sided")
            posthoc_rows.append({
                "Группа 1": g1, "Группа 2": g2,
                "U-стат": round(u_stat, 2),
                "p-value": round(u_p, 4),
                "Значимо (Bonf)": "ДА" if u_p < alpha_corrected else "НЕТ"
            })
    print("\n--- Пост-хок (Mann-Whitney U + Бонферрони) ---")
    if posthoc_rows:
        print(pd.DataFrame(posthoc_rows).to_string(index=False))
    else:
        print("  Различия не значимы, пост-хок не требуется.")
        
    # Сохраняем итоговую таблицу
    summary = pd.DataFrame([{
        "Фактор": factor, "Показатель": target,
        "Levene p": round(lev_p, 4),
        "F (ANOVA)": round(f_stat, 4), "p (ANOVA)": round(anova_p, 6),
        "H (Kruskal)": round(kw_stat, 4), "p (Kruskal)": round(kw_p, 6),
        "Eta^2 (эффект)": round(eta_sq, 4),
        "Значимо": "ДА" if kw_p < 0.05 else "НЕТ"
    }])
    save_table(summary, f"4_1_anova_{factor}_vs_{target}".replace(" ", "_"))
    
    if posthoc_rows:
        save_table(pd.DataFrame(posthoc_rows), f"4_2_posthoc_{factor}_vs_{target}".replace(" ", "_"))
        
    return groups, group_names


# ============================================================
# 4.2 ВИЗУАЛИЗАЦИЯ (BOXPLOT + СТРИПЛОТ)
# ============================================================
def plot_anova(groups, group_names, target, factor):
    fig, ax = plt.subplots(figsize=(8, 5))
    
    # Боксплот
    bp = ax.boxplot(groups, patch_artist=True, labels=group_names,
                    medianprops=dict(color="magenta", linewidth=2))
    for patch in bp['boxes']:
        patch.set_facecolor('lightgreen')
        
    # Стрипплот (точки данных поверх)
    for i, g in enumerate(groups):
        # Добавляем небольшой "шум" по X, чтобы точки не слипались
        x_jitter = np.random.normal(i + 1, 0.04, size=len(g))
        ax.plot(x_jitter, g, 'o', alpha=0.3, color='pink', markersize=3)
        
    ax.set_title(f"Влияние фактора '{factor}' на '{target}'")
    ax.set_ylabel(target)
    ax.grid(alpha=0.3, axis="y")
    
    save_fig(f"4_3_anova_{factor}_vs_{target}".replace(" ", "_"))


# ============================================================
# MAIN
# ============================================================
def main():
    if len(sys.argv) < 2:
        print("Использование: python part4_anova.py fetal_clean.csv")
        sys.exit(1)
    df = pd.read_csv(sys.argv[1])
    
    for analysis in ANALYSES:
        target = analysis["target"]
        factor = analysis["factor"]
        
        if target not in df.columns or factor not in df.columns:
            print(f"Пропуск: {target} или {factor} нет в датасете")
            continue
            
        groups, group_names = run_anova(df, target, factor)
        plot_anova(groups, group_names, target, factor)

    hr("ГОТОВО")
    print("  Таблицы: ./tables/4_*.csv")
    print("  Графики: ./figures/4_*.png")


if __name__ == "__main__":
    main()