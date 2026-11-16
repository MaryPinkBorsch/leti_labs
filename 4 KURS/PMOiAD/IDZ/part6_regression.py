# part6_regression.py
# Запуск: python3 part6_regression.py fetal_clean.csv
# Артефакты: tables/6_*.csv, figures/6_*.png

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

from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score
from sklearn.model_selection import train_test_split
import statsmodels.api as sm


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
# 6.1 ПРОСТАЯ ЛИНЕЙНАЯ РЕГРЕССИЯ
# ============================================================
def simple_regression(df, target, predictor):
    hr(f"6.1 ПРОСТАЯ ЛИНЕЙНАЯ РЕГРЕССИЯ: {target} ~ {predictor}")
    data = df[[target, predictor]].dropna()
    X = data[[predictor]].values
    y = data[target].values

    # Разделение train/test 80/20
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

    # Обучение
    model = LinearRegression()
    model.fit(X_train, y_train)

    # Прогнозы
    y_pred_train = model.predict(X_train)
    y_pred_test = model.predict(X_test)

    # Метрики
    metrics = {
        "Модель": "Простая",
        "Цель": target,
        "Предикторы": predictor,
        "Коэф. a (intercept)": round(model.intercept_, 4),
        "Коэф. b (наклон)": round(model.coef_[0], 4),
        "R² (train)": round(r2_score(y_train, y_pred_train), 4),
        "R² (test)": round(r2_score(y_test, y_pred_test), 4),
        "MAE (test)": round(mean_absolute_error(y_test, y_pred_test), 4),
        "MSE (test)": round(mean_squared_error(y_test, y_pred_test), 4),
        "RMSE (test)": round(np.sqrt(mean_squared_error(y_test, y_pred_test)), 4),
    }
    print(f"  Уравнение: {target} = {model.intercept_:.2f} + {model.coef_[0]:.2f} * {predictor}")
    print(f"  R² (test) = {metrics['R² (test)']}, RMSE = {metrics['RMSE (test)']}")
    return metrics, (y_test, y_pred_test)


# ============================================================
# 6.2 МНОЖЕСТВЕННАЯ ЛИНЕЙНАЯ РЕГРЕССИЯ
# ============================================================
def multiple_regression(df, target, predictors):
    hr(f"6.2 МНОЖЕСТВЕННАЯ ЛИНЕЙНАЯ РЕГРЕССИЯ: {target} ~ ...")
    print(f"  Предикторы ({len(predictors)}): {predictors}")
    data = df[[target] + predictors].dropna()
    X = data[predictors]
    y = data[target]

    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

    # --- sklearn для метрик ---
    model = LinearRegression()
    model.fit(X_train, y_train)
    y_pred_test = model.predict(X_test)

    metrics = {
        "Модель": "Множественная",
        "Цель": target,
        "Предикторы": ", ".join(predictors),
        "R² (train)": round(r2_score(y_train, model.predict(X_train)), 4),
        "R² (test)": round(r2_score(y_test, y_pred_test), 4),
        "MAE (test)": round(mean_absolute_error(y_test, y_pred_test), 4),
        "MSE (test)": round(mean_squared_error(y_test, y_pred_test), 4),
        "RMSE (test)": round(np.sqrt(mean_squared_error(y_test, y_pred_test)), 4),
    }
    print(f"  R² (test) = {metrics['R² (test)']}, RMSE = {metrics['RMSE (test)']}")

    # --- statsmodels для p-value и проверки значимости коэффициентов ---
    X_train_sm = sm.add_constant(X_train)
    sm_model = sm.OLS(y_train, X_train_sm).fit()
    print("\n--- Коэффициенты (statsmodels) ---")
    coef_df = pd.DataFrame({
        "Признак": sm_model.params.index,
        "Коэффициент": sm_model.params.values.round(4),
        "p-value": sm_model.pvalues.values.round(6),
    })
    print(coef_df.to_string(index=False))
    save_table(coef_df, "6_2_multiple_coefs")

    return metrics, (y_test, y_pred_test), model, X_test, y_test


# ============================================================
# 6.3 АНАЛИЗ ОСТАТКОВ
# ============================================================
def residual_analysis(y_true, y_pred, target, model_name):
    hr(f"6.3 АНАЛИЗ ОСТАТКОВ ({model_name})")
    residuals = y_true - y_pred
    # Тест Шапиро-Уилка на нормальность остатков
    sw_stat, sw_p = stats.shapiro(residuals[:5000])  # ограничение для теста
    print(f"  Shapiro-Wilk p-value = {sw_p:.4e} (H0: остатки нормальны)")
    print(f"  {'Остатки нормальны' if sw_p > 0.05 else 'Остатки НЕ нормальны (типично для реальных данных)'}")

    # График остатков
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))

    # 1) Остатки vs Предсказанные
    axes[0].scatter(y_pred, residuals, alpha=0.3, color="magenta", s=10)
    axes[0].axhline(0, color="red", linestyle="--")
    axes[0].set_xlabel("Предсказанные значения")
    axes[0].set_ylabel("Остатки")
    axes[0].set_title(f"{model_name}: Остатки vs Прогноз")
    axes[0].grid(alpha=0.3)

    # 2) Гистограмма остатков
    axes[1].hist(residuals, bins=40, color="pink", edgecolor="black", density=True)
    # Наложим нормальное распределение
    mu, std = residuals.mean(), residuals.std()
    x_grid = np.linspace(residuals.min(), residuals.max(), 100)
    axes[1].plot(x_grid, stats.norm.pdf(x_grid, mu, std), "r-", lw=2)
    axes[1].set_title(f"{model_name}: Распределение остатков")
    axes[1].set_xlabel("Остатки")
    axes[1].set_ylabel("Плотность")
    axes[1].grid(alpha=0.3)

    save_fig(f"6_3_residuals_{model_name}".replace(" ", "_"))


# ============================================================
# 6.4 ГРАФИК "ФАКТ vs ПРОГНОЗ" (для сравнения)
# ============================================================
def plot_actual_vs_pred(y_true, y_pred, model_name):
    fig, ax = plt.subplots(figsize=(6, 6))
    ax.scatter(y_true, y_pred, alpha=0.3, s=10, color="magenta")
    # Идеальная линия
    lims = [min(y_true.min(), y_pred.min()), max(y_true.max(), y_pred.max())]
    ax.plot(lims, lims, "r--", lw=2, label="Идеальное совпадение")
    ax.set_xlabel("Фактические значения")
    ax.set_ylabel("Предсказанные значения")
    ax.set_title(f"{model_name}: Факт vs Прогноз")
    ax.legend()
    ax.grid(alpha=0.3)
    save_fig(f"6_4_actual_vs_pred_{model_name}".replace(" ", "_"))


# ============================================================
# MAIN
# ============================================================
def main():
    if len(sys.argv) < 2:
        print("Использование: python part6_regression.py fetal_clean.csv")
        sys.exit(1)
    df = pd.read_csv(sys.argv[1])

    target = "histogram_mean"
    # Предикторы: без мультиколлинеарных дублей (median, mode, width)
    predictors = [
        "baseline value", "accelerations", "fetal_movement",
        "abnormal_short_term_variability", "histogram_min",
        "histogram_max", "histogram_variance"
    ]

    # Для простой регрессии возьмем самый сильный коррелят (baseline value)
    simple_predictor = "baseline value"

    metrics_simple, preds_simple = simple_regression(df, target, simple_predictor)
    metrics_multi, preds_multi, model_multi, X_test, y_test = multiple_regression(df, target, predictors)

    # Остатки
    residual_analysis(preds_simple[0], preds_simple[1], target, "Простая")
    residual_analysis(preds_multi[0], preds_multi[1], target, "Множественная")

    # Графики факт vs прогноз
    plot_actual_vs_pred(preds_simple[0], preds_simple[1], "Простая")
    plot_actual_vs_pred(preds_multi[0], preds_multi[1], "Множественная")

    # Сводная таблица
    hr("6.5 СРАВНЕНИЕ МОДЕЛЕЙ")
    summary = pd.DataFrame([metrics_simple, metrics_multi])
    print(summary.to_string(index=False))
    save_table(summary, "6_5_model_comparison")

    hr("ГОТОВО")
    print("  Таблицы: ./tables/6_*.csv")
    print("  Графики: ./figures/6_*.png")


if __name__ == "__main__":
    main()