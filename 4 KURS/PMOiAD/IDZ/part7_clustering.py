# part7_clustering.py
# Запуск: python3 part7_clustering.py fetal_clean.csv
# Артефакты: tables/7_*.csv, figures/7_*.png

import sys
import os
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import seaborn as sns
import warnings
warnings.filterwarnings("ignore")

from sklearn.preprocessing import RobustScaler
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score
from sklearn.decomposition import PCA

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
# 7.1 ВЫБОР ПРИЗНАКОВ И МАСШТАБИРОВАНИЕ
# ============================================================
def prepare_data(df):
    hr("7.1 ВЫБОР ПРИЗНАКОВ И МАСШТАБИРОВАНИЕ")
    # Выбираем 6 информативных клинических признаков (без дублей)
    features = [
        "baseline value",
        "abnormal_short_term_variability",
        "mean_value_of_short_term_variability",
        "histogram_min",
        "histogram_max",
        "histogram_variance"
    ]
    print(f"Признаки для кластеризации ({len(features)}): {features}")

    # Берем только эти столбцы и убираем пропуски (на всякий случай)
    X = df[features].dropna()

    # Масштабирование. Используем RobustScaler, т.к. в данных есть выбросы
    scaler = RobustScaler()
    X_scaled = scaler.fit_transform(X)
    print("Применен RobustScaler (медиана / IQR), так как данные не нормальны и содержат выбросы.")
    return X, X_scaled, features

# ============================================================
# 7.2 ВЫБОР ОПТИМАЛЬНОГО КОЛИЧЕСТВА КЛАСТЕРОВ (ЛОКОТЬ + СИЛУЭТ)
# ============================================================
def find_optimal_k(X_scaled):
    hr("7.2 ОПРЕДЕЛЕНИЕ РАЦИОНАЛЬНОГО ЧИСЛА КЛАСТЕРОВ")
    k_range = range(2, 11)
    inertias = []
    sil_scores = []

    for k in k_range:
        kmeans = KMeans(n_clusters=k, random_state=42, n_init=10)
        labels = kmeans.fit_predict(X_scaled)
        inertias.append(kmeans.inertia_)
        # Силуэт считаем на подвыборке (для скорости), если данных много
        if len(X_scaled) > 3000:
            sample_idx = np.random.choice(len(X_scaled), 3000, replace=False)
            score = silhouette_score(X_scaled[sample_idx], labels[sample_idx])
        else:
            score = silhouette_score(X_scaled, labels)
        sil_scores.append(score)

    results = pd.DataFrame({
        "k": list(k_range),
        "Inertia (WCSS)": [round(x, 2) for x in inertias],
        "Silhouette": [round(x, 4) for x in sil_scores]
    })
    print(results.to_string(index=False))
    save_table(results, "7_2_k_optimization")

    # --- График локтя ---
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 4))
    ax1.plot(k_range, inertias, "o-", color="pink", lw=2)
    ax1.set_title("Метод локтя (Inertia)")
    ax1.set_xlabel("Количество кластеров (k)")
    ax1.set_ylabel("WCSS (Inertia)")
    ax1.grid(alpha=0.3)

    ax2.plot(k_range, sil_scores, "o-", color="green", lw=2)
    ax2.set_title("Коэффициент силуэта (Silhouette)")
    ax2.set_xlabel("Количество кластеров (k)")
    ax2.set_ylabel("Silhouette Score")
    ax2.grid(alpha=0.3)
    save_fig("7_2_optimal_k")

    # Выбираем k с максимальным силуэтом
    optimal_k = results.loc[results["Silhouette"].idxmax(), "k"]
    print(f"\n  -> Оптимальное k по силуэту: {optimal_k}")
    return optimal_k

# ============================================================
# 7.3 K-MEANS И ОЦЕНКА КАЧЕСТВА
# ============================================================
def perform_kmeans(X_scaled, X_raw, optimal_k, features):
    hr("7.3 ОБУЧЕНИЕ K-MEANS И АНАЛИЗ РЕЗУЛЬТАТОВ")
    kmeans = KMeans(n_clusters=optimal_k, random_state=42, n_init=10)
    labels = kmeans.fit_predict(X_scaled)
    
    sil = silhouette_score(X_scaled, labels)
    print(f"Итоговый коэффициент силуэта для k={optimal_k}: {sil:.4f}")
    print("(0.5-1.0 — отличная кластеризация, 0.25-0.5 — средняя, <0.25 — слабая)")

    # Добавляем метки в исходный датасет
    df_result = X_raw.copy()
    df_result["Cluster"] = labels

    # Статистика по кластерам (средние)
    cluster_means = df_result.groupby("Cluster")[features].mean().round(2)
    cluster_means["Count"] = df_result["Cluster"].value_counts().sort_index()
    print("\n--- Средние значения признаков по кластерам ---")
    print(cluster_means.to_string())
    save_table(cluster_means.reset_index(), "7_3_cluster_means")

    return labels, df_result, cluster_means

# ============================================================
# 7.4 ВИЗУАЛИЗАЦИЯ В 2D (PCA)
# ============================================================
def plot_pca_clusters(X_scaled, labels, optimal_k):
    hr("7.4 ВИЗУАЛИЗАЦИЯ КЛАСТЕРОВ (PCA)")
    # Снижаем размерность до 2D с помощью PCA
    pca = PCA(n_components=2, random_state=42)
    X_pca = pca.fit_transform(X_scaled)

    # Объяснённая дисперсия
    var_exp = pca.explained_variance_ratio_
    print(f"Объяснённая дисперсия: PC1 = {var_exp[0]:.2%}, PC2 = {var_exp[1]:.2%}")
    print(f"Суммарно: {sum(var_exp):.2%}")

    # График
    fig, ax = plt.subplots(figsize=(9, 7))
    palette = sns.color_palette("Set1", optimal_k)
    for i in range(optimal_k):
        mask = labels == i
        ax.scatter(X_pca[mask, 0], X_pca[mask, 1], 
                   label=f"Кластер {i}", alpha=0.5, s=15, color=palette[i])
        
    # Центроиды
    centroids = pca.transform(KMeans(n_clusters=optimal_k, random_state=42, n_init=10).fit(X_scaled).cluster_centers_)
    ax.scatter(centroids[:, 0], centroids[:, 1], marker="X", s=200, c="black", label="Центроиды")
    
    ax.set_title(f"Визуализация кластеров K-means (PCA, k={optimal_k})")
    ax.set_xlabel(f"PC1 ({var_exp[0]:.1%})")
    ax.set_ylabel(f"PC2 ({var_exp[1]:.1%})")
    ax.legend()
    ax.grid(alpha=0.3)
    save_fig("7_4_pca_clusters")

# ============================================================
# 7.5 СРАВНЕНИЕ С ИСТИННЫМИ КЛАССАМИ (SANITY CHECK)
# ============================================================
def compare_with_truth(df, labels):
    hr("7.5 СРАВНЕНИЕ КЛАСТЕРОВ С ИСТИННЫМ СОСТОЯНИЕМ ПЛОДА (SANITY CHECK)")
    # Убедимся, что метки классов есть в датафрейме
    if "health_label" not in df.columns:
        print("Столбец health_label отсутствует, сравнение невозможно.")
        return

    # Кросс-табуляция (Contingency table)
    ct = pd.crosstab(labels, df["health_label"], margins=True)
    print("Таблица сопряженности (Кластер x Истинный класс):")
    print(ct.to_string())
    save_table(ct.reset_index(), "7_5_cluster_vs_truth")

# ============================================================
# MAIN
# ============================================================
def main():
    if len(sys.argv) < 2:
        print("Использование: python part7_clustering.py fetal_clean.csv")
        sys.exit(1)
    df = pd.read_csv(sys.argv[1])

    X_raw, X_scaled, features = prepare_data(df)
    optimal_k = find_optimal_k(X_scaled)
    labels, df_result, cluster_means = perform_kmeans(X_scaled, X_raw, optimal_k, features)
    plot_pca_clusters(X_scaled, labels, optimal_k)
    
    # Для sanity check нам нужны только те строки, что попали в X_raw
    df_matched = df.loc[X_raw.index].copy()
    compare_with_truth(df_matched, labels)

    hr("ГОТОВО")
    print("  Таблицы: ./tables/7_*.csv")
    print("  Графики: ./figures/7_*.png")

if __name__ == "__main__":
    main()