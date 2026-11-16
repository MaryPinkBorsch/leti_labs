# part3_distributions.py
# Запуск: python3 part3_distributions.py fetal_clean.csv
# Артефакты: tables/3_*.csv, figures/3_*.png

import sys
import os
import numpy as np
import pandas as pd
from scipy import stats
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import warnings
warnings.filterwarnings("ignore")  # подавляем warning от фитинга

# ============================================================
# ВЫБРАННЫЕ ПРИЗНАКИ ДЛЯ АНАЛИЗА РАСПРЕДЕЛЕНИЙ
# ============================================================
SELECTED_FEATURES = [
    "baseline value",
    "abnormal_short_term_variability",
    "histogram_width",
    "histogram_variance",
    "mean_value_of_long_term_variability"
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
# 3.1 ГИСТОГРАММЫ + KDE + ЭМПИРИЧЕСКАЯ CDF
# ============================================================
def plot_hist_kde_cdf(df, features):
    hr("3.1 ГИСТОГРАММЫ, KDE И ЭМПИРИЧЕСКИЕ CDF")
    for c in features:
        s = df[c].dropna()
        fig, axes = plt.subplots(1, 3, figsize=(16, 4))

        # 1) Гистограмма + KDE
        axes[0].hist(s, bins=40, density=True, alpha=0.6,
                     color="steelblue", edgecolor="black", label="Гистограмма")
        kde = stats.gaussian_kde(s)
        x_grid = np.linspace(s.min(), s.max(), 300)
        axes[0].plot(x_grid, kde(x_grid), "r-", lw=2, label="KDE")
        axes[0].set_title(f"{c}\nГистограмма + KDE", fontsize=9)
        axes[0].set_xlabel("Значение")
        axes[0].set_ylabel("Плотность")
        axes[0].legend(fontsize=8)
        axes[0].grid(alpha=0.3)

        # 2) Гистограмма + нормальное распределение (для наглядности)
        mu, sigma = s.mean(), s.std(ddof=1)
        axes[1].hist(s, bins=40, density=True, alpha=0.6,
                     color="steelblue", edgecolor="black", label="Эмпирика")
        axes[1].plot(x_grid, stats.norm.pdf(x_grid, mu, sigma),
                     "g--", lw=2, label=f"N({mu:.2f}, {sigma:.2f}²)")
        axes[1].set_title(f"{c}\nСравнение с нормальным", fontsize=9)
        axes[1].set_xlabel("Значение")
        axes[1].set_ylabel("Плотность")
        axes[1].legend(fontsize=8)
        axes[1].grid(alpha=0.3)

        # 3) Эмпирическая CDF
        sorted_s = np.sort(s)
        y_ecdf = np.arange(1, len(s) + 1) / len(s)
        axes[2].step(sorted_s, y_ecdf, where="post", color="navy", lw=2)
        axes[2].set_title(f"{c}\nЭмпирическая CDF", fontsize=9)
        axes[2].set_xlabel("Значение")
        axes[2].set_ylabel("F(x)")
        axes[2].grid(alpha=0.3)

        save_fig(f"3_1_dist_{c.replace(' ', '_')}")


# ============================================================
# 3.2 ПОДБОР ТЕОРЕТИЧЕСКИХ РАСПРЕДЕЛЕНИЙ
# ============================================================
def fit_distributions(df, features):
    hr("3.2 ОЦЕНКА ПАРАМЕТРОВ ТЕОРЕТИЧЕСКИХ РАСПРЕДЕЛЕНИЙ")
    dist_names = ["norm", "lognorm", "gamma", "expon", "weibull_min"]
    rows = []
    for c in features:
        s = df[c].dropna()
        # Смещаем данные, если есть нули/отрицательные (для lognorm, gamma, weibull)
        shift = 0.0
        if s.min() <= 0:
            shift = abs(s.min()) + 1e-6
            s_shifted = s + shift
        else:
            s_shifted = s

        for dname in dist_names:
            dist = getattr(stats, dname)
            try:
                params = dist.fit(s_shifted)
                # KS-тест на соответствие
                D, p = stats.kstest(s_shifted, dname, args=params)
                # Лог-правдоподобие
                loglik = np.sum(dist.logpdf(s_shifted, *params))
                # AIC = 2k - 2*loglik
                k = len(params)
                aic = 2 * k - 2 * loglik
                rows.append({
                    "Признак": c,
                    "Распределение": dname,
                    "Сдвиг": round(shift, 4),
                    "Параметры": str([round(p, 4) for p in params]),
                    "KS D": round(D, 4),
                    "KS p-value": round(p, 4),
                    "LogLik": round(loglik, 2),
                    "AIC": round(aic, 2),
                })
            except Exception as e:
                rows.append({
                    "Признак": c, "Распределение": dname,
                    "Сдвиг": round(shift, 4), "Параметры": "ошибка фитинга",
                    "KS D": np.nan, "KS p-value": np.nan,
                    "LogLik": np.nan, "AIC": np.nan,
                })
    rep = pd.DataFrame(rows).sort_values(["Признак", "AIC"])
    print(rep.to_string(index=False))
    save_table(rep, "3_2_fitted_distributions")
    return rep


# ============================================================
# 3.3 ПРОВЕРКА НОРМАЛЬНОСТИ
# ============================================================
def normality_tests(df, features):
    hr("3.3 ПРОВЕРКА ГИПОТЕЗЫ О НОРМАЛЬНОСТИ")
    rows = []
    for c in features:
        s = df[c].dropna()
        # Shapiro-Wilk (для n <= 5000)
        if len(s) <= 5000:
            sw_stat, sw_p = stats.shapiro(s)
        else:
            sw_stat, sw_p = np.nan, np.nan
        # D'Agostino K² (для n > 20)
        da_stat, da_p = stats.normaltest(s)
        # Anderson-Darling
        ad = stats.anderson(s, dist="norm")
        ad_stat = ad.statistic
        ad_crit = ad.critical_values[2]  # 5% уровень
        rows.append({
            "Признак": c,
            "n": len(s),
            "Shapiro W": round(sw_stat, 4) if not np.isnan(sw_stat) else "n/a",
            "Shapiro p": round(sw_p, 6) if not np.isnan(sw_p) else "n/a",
            "D'Agostino p": round(da_p, 6),
            "AD stat": round(ad_stat, 4),
            "AD crit(5%)": round(ad_crit, 4),
            "Нормальность": "ДА" if (sw_p > 0.05 if not np.isnan(sw_p) else da_p > 0.05) else "НЕТ",
        })
    rep = pd.DataFrame(rows)
    print(rep.to_string(index=False))
    save_table(rep, "3_3_normality_tests")
    return rep


# ============================================================
# 3.4 Q-Q ГРАФИКИ
# ============================================================
def plot_qq(df, features):
    hr("3.4 Q-Q ГРАФИКИ")
    for c in features:
        s = df[c].dropna()
        fig, ax = plt.subplots(figsize=(6, 5))
        stats.probplot(s, dist="norm", plot=ax)
        ax.set_title(f"Q-Q график: {c}\n(сравнение с нормальным)", fontsize=10)
        ax.grid(alpha=0.3)
        save_fig(f"3_4_qq_{c.replace(' ', '_')}")


# ============================================================
# MAIN
# ============================================================
def main():
    if len(sys.argv) < 2:
        print("Использование: python part3_distributions.py fetal_clean.csv")
        sys.exit(1)
    df = pd.read_csv(sys.argv[1])
    print(f"Загружено: {df.shape}")

    features = [c for c in SELECTED_FEATURES if c in df.columns]
    print(f"Выбрано для анализа распределений: {len(features)} признаков")
    print(features)

    plot_hist_kde_cdf(df, features)
    fit_distributions(df, features)
    normality_tests(df, features)
    plot_qq(df, features)

    hr("ГОТОВО")
    print("  Таблицы: ./tables/3_*.csv")
    print("  Графики: ./figures/3_*.png")


if __name__ == "__main__":
    main()