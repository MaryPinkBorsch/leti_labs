# part9_evaluation.py
# Запуск: python3 part9_evaluation.py
# Использует данные из п.1 (data/X_train_scaled.csv и т.д.)
# Артефакты: tables/9_*.csv, figures/9_*.png

import os
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier
from sklearn.neural_network import MLPClassifier
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import (accuracy_score, precision_score, recall_score,
                             f1_score, roc_auc_score, roc_curve,
                             precision_recall_curve, average_precision_score,
                             confusion_matrix, classification_report)
import warnings
warnings.filterwarnings("ignore")

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
# 9.1 ЗАГРУЗКА И ОБУЧЕНИЕ (быстрое, без вывода логов GridSearch)
# ============================================================
def train_all_models():
    hr("9.1 ОБУЧЕНИЕ МОДЕЛЕЙ (на основе п.8)")
    X_train = pd.read_csv("data/X_train_scaled.csv")
    X_test = pd.read_csv("data/X_test_scaled.csv")
    y_train = pd.read_csv("data/y_train.csv").squeeze()
    y_test = pd.read_csv("data/y_test.csv").squeeze()

    models = {}

    # Логистическая регрессия
    print("Обучение Logistic Regression...")
    lr = LogisticRegression(C=1, penalty="l2", solver="liblinear", 
                            class_weight="balanced", max_iter=1000, random_state=42)
    lr.fit(X_train, y_train)
    models["Logistic Regression"] = lr

    # Random Forest
    print("Обучение Random Forest...")
    rf = RandomForestClassifier(n_estimators=200, max_depth=None, min_samples_leaf=1,
                                class_weight="balanced", random_state=42, n_jobs=-1)
    rf.fit(X_train, y_train)
    models["Random Forest"] = rf

    # MLP
    print("Обучение MLP (Neural Network)...")
    mlp = MLPClassifier(hidden_layer_sizes=(50, 25), alpha=0.01, learning_rate_init=0.01,
                        max_iter=500, early_stopping=True, random_state=42)
    mlp.fit(X_train, y_train)
    models["MLP"] = mlp

    return models, X_test, y_test

# ============================================================
# 9.2 РАСЧЁТ ВСЕХ МЕТРИК + AVERAGE PRECISION
# ============================================================
def calculate_all_metrics(models, X_test, y_test):
    hr("9.2 РАСЧЁТ МЕТРИК (Accuracy, Precision, Recall, F1, ROC-AUC, AP)")
    results = []
    for name, model in models.items():
        y_pred = model.predict(X_test)
        y_proba = model.predict_proba(X_test)[:, 1]
        
        results.append({
            "Модель": name,
            "Accuracy": round(accuracy_score(y_test, y_pred), 4),
            "Precision": round(precision_score(y_test, y_pred), 4),
            "Recall": round(recall_score(y_test, y_pred), 4),
            "F1-score": round(f1_score(y_test, y_pred), 4),
            "ROC-AUC": round(roc_auc_score(y_test, y_proba), 4),
            "Average Precision": round(average_precision_score(y_test, y_proba), 4)
        })
    
    df_metrics = pd.DataFrame(results)
    print(df_metrics.to_string(index=False))
    save_table(df_metrics, "9_1_final_metrics")
    return df_metrics

# ============================================================
# 9.3 ROC-КРИВЫЕ
# ============================================================
def plot_roc_curves(models, X_test, y_test):
    hr("9.3 ПОСТРОЕНИЕ ROC-КРИВЫХ")
    fig, ax = plt.subplots(figsize=(8, 6))
    
    colors = {"Logistic Regression": "magenta", "Random Forest": "green", "MLP": "orange"}
    
    for name, model in models.items():
        y_proba = model.predict_proba(X_test)[:, 1]
        fpr, tpr, _ = roc_curve(y_test, y_proba)
        auc = roc_auc_score(y_test, y_proba)
        ax.plot(fpr, tpr, color=colors[name], lw=2, label=f"{name} (AUC = {auc:.3f})")
        
    ax.plot([0, 1], [0, 1], "k--", lw=1, label="Случайный классификатор")
    ax.set_xlabel("False Positive Rate (FPR)")
    ax.set_ylabel("True Positive Rate (TPR)")
    ax.set_title("ROC-кривые моделей классификации")
    ax.legend(loc="lower right")
    ax.grid(alpha=0.3)
    save_fig("9_3_roc_curves")

# ============================================================
# 9.4 PR-КРИВЫЕ (для дисбаланса)
# ============================================================
def plot_pr_curves(models, X_test, y_test):
    hr("9.4 ПОСТРОЕНИЕ PR-КРИВЫХ (Precision-Recall)")
    fig, ax = plt.subplots(figsize=(8, 6))
    
    colors = {"Logistic Regression": "magenta", "Random Forest": "green", "MLP": "orange"}
    
    for name, model in models.items():
        y_proba = model.predict_proba(X_test)[:, 1]
        precision, recall, _ = precision_recall_curve(y_test, y_proba)
        ap = average_precision_score(y_test, y_proba)
        ax.plot(recall, precision, color=colors[name], lw=2, label=f"{name} (AP = {ap:.3f})")
        
    # Baseline (доля положительного класса)
    baseline = y_test.sum() / len(y_test)
    ax.axhline(y=baseline, color="k", linestyle="--", lw=1, label=f"Baseline ({baseline:.2f})")
    
    ax.set_xlabel("Recall")
    ax.set_ylabel("Precision")
    ax.set_title("PR-кривые (Precision-Recall) моделей классификации")
    ax.legend(loc="lower left")
    ax.grid(alpha=0.3)
    save_fig("9_4_pr_curves")

# ============================================================
# 9.5 ВИЗУАЛИЗАЦИЯ МАТРИЦ ОШИБОК
# ============================================================
def plot_confusion_matrices(models, X_test, y_test):
    hr("9.5 ВИЗУАЛИЗАЦИЯ МАТРИЦ ОШИБОК")
    fig, axes = plt.subplots(1, 3, figsize=(15, 4))
    
    for ax, (name, model) in zip(axes, models.items()):
        y_pred = model.predict(X_test)
        cm = confusion_matrix(y_test, y_pred)
        
        sns.heatmap(cm, annot=True, fmt="d", cmap="Blues", ax=ax, cbar=False,
                    annot_kws={"size": 14})
        ax.set_title(name)
        ax.set_xlabel("Предсказанный класс")
        ax.set_ylabel("Истинный класс")
        ax.set_xticklabels(["Normal (0)", "Abnormal (1)"])
        ax.set_yticklabels(["Normal (0)", "Abnormal (1)"])
        
    save_fig("9_5_confusion_matrices")

# ============================================================
# 9.6 СРАВНИТЕЛЬНАЯ ДИАГРАММА МЕТРИК
# ============================================================
def plot_metric_comparison(df_metrics):
    hr("9.6 СРАВНИТЕЛЬНАЯ ДИАГРАММА МЕТРИК")
    df_plot = df_metrics.set_index("Модель").drop(columns=["Average Precision"])
    
    # Переводим в "длинный" формат для seaborn
    df_long = df_plot.reset_index().melt(id_vars="Модель", var_name="Метрика", value_name="Значение")
    
    fig, ax = plt.subplots(figsize=(10, 5))
    sns.barplot(data=df_long, x="Метрика", y="Значение", hue="Модель", ax=ax)
    ax.set_title("Сравнение метрик качества моделей")
    ax.set_ylim(0, 1.05)
    ax.grid(axis="y", alpha=0.3)
    ax.legend(loc="lower right")
    save_fig("9_6_metric_comparison")

# ============================================================
# MAIN
# ============================================================
def main():
    models, X_test, y_test = train_all_models()
    df_metrics = calculate_all_metrics(models, X_test, y_test)
    plot_roc_curves(models, X_test, y_test)
    plot_pr_curves(models, X_test, y_test)
    plot_confusion_matrices(models, X_test, y_test)
    plot_metric_comparison(df_metrics)
    
    hr("ГОТОВО")
    print("  Таблицы: ./tables/9_*.csv")
    print("  Графики: ./figures/9_*.png")

if __name__ == "__main__":
    main()