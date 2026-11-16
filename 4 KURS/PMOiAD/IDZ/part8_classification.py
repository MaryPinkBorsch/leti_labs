# part8_classification.py
# Запуск: python3 part8_classification.py
# ВАЖНО: скрипт использует артефакты, созданные в п.1 (data/X_train_scaled.csv и т.д.)
# Артефакты: tables/8_*.csv, figures/8_*.png

import os
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier
from sklearn.neural_network import MLPClassifier
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import (accuracy_score, precision_score, recall_score,
                             f1_score, roc_auc_score, confusion_matrix,
                             classification_report)
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
# 8.1 ЗАГРУЗКА ДАННЫХ
# ============================================================
def load_data():
    hr("8.1 ЗАГРУЗКА ДАННЫХ (единое разбиение из п.1)")
    X_train = pd.read_csv("data/X_train_scaled.csv")
    X_test = pd.read_csv("data/X_test_scaled.csv")
    y_train = pd.read_csv("data/y_train.csv").squeeze()
    y_test = pd.read_csv("data/y_test.csv").squeeze()

    print(f"Train: {X_train.shape}, Test: {X_test.shape}")
    print(f"Дисбаланс в train: {dict(y_train.value_counts())}")
    print(f"Дисбаланс в test:  {dict(y_test.value_counts())}")
    return X_train, X_test, y_train, y_test


# ============================================================
# 8.2 ОЦЕНКА МОДЕЛИ (вынесена в функцию для переиспользования)
# ============================================================
def evaluate_model(model, X_test, y_test, model_name):
    y_pred = model.predict(X_test)
    y_proba = model.predict_proba(X_test)[:, 1] if hasattr(model, "predict_proba") else None

    metrics = {
        "Модель": model_name,
        "Accuracy": round(accuracy_score(y_test, y_pred), 4),
        "Precision": round(precision_score(y_test, y_pred, zero_division=0), 4),
        "Recall": round(recall_score(y_test, y_pred, zero_division=0), 4),
        "F1-score": round(f1_score(y_test, y_pred, zero_division=0), 4),
        "ROC-AUC": round(roc_auc_score(y_test, y_proba), 4) if y_proba is not None else np.nan,
    }
    cm = confusion_matrix(y_test, y_pred)
    
    print(f"\n--- {model_name} ---")
    print(f"Accuracy:  {metrics['Accuracy']}")
    print(f"Precision: {metrics['Precision']}")
    print(f"Recall:    {metrics['Recall']}")
    print(f"F1-score:  {metrics['F1-score']}")
    print(f"ROC-AUC:   {metrics['ROC-AUC']}")
    print(f"Матрица ошибок:\n{cm}")
    
    return metrics, cm, y_proba


# ============================================================
# 8.3 ЛОГИСТИЧЕСКАЯ РЕГРЕССИЯ
# ============================================================
def train_logreg(X_train, y_train, X_test, y_test):
    hr("8.3 ЛОГИСТИЧЕСКАЯ РЕГРЕССИЯ (с GridSearchCV)")
    # Подбор гиперпараметров на train с 5-fold CV
    param_grid = {
        "C": [0.01, 0.1, 1, 10],  # сила регуляризации
        "penalty": ["l2"],
        "solver": ["lbfgs", "liblinear"],
    }
    base = LogisticRegression(class_weight="balanced", max_iter=1000, random_state=42)
    grid = GridSearchCV(base, param_grid, cv=5, scoring="f1", n_jobs=-1)
    grid.fit(X_train, y_train)
    
    print(f"Лучшие параметры: {grid.best_params_}")
    print(f"CV F1 (на train): {grid.best_score_:.4f}")
    
    return evaluate_model(grid.best_estimator_, X_test, y_test, "Logistic Regression")


# ============================================================
# 8.4 RANDOM FOREST
# ============================================================
def train_rf(X_train, y_train, X_test, y_test):
    hr("8.4 RANDOM FOREST (с GridSearchCV)")
    param_grid = {
        "n_estimators": [100, 200],
        "max_depth": [5, 10, None],
        "min_samples_leaf": [1, 5],
    }
    base = RandomForestClassifier(class_weight="balanced", random_state=42, n_jobs=-1)
    grid = GridSearchCV(base, param_grid, cv=5, scoring="f1", n_jobs=-1)
    grid.fit(X_train, y_train)
    
    print(f"Лучшие параметры: {grid.best_params_}")
    print(f"CV F1 (на train): {grid.best_score_:.4f}")
    
    return evaluate_model(grid.best_estimator_, X_test, y_test, "Random Forest")


# ============================================================
# 8.5 MLP (НЕЙРОСЕТЬ)
# ============================================================
def train_mlp(X_train, y_train, X_test, y_test):
    hr("8.5 MLP (Нейросеть) (с GridSearchCV)")
    param_grid = {
        "hidden_layer_sizes": [(50,), (100,), (50, 25)],
        "alpha": [0.0001, 0.001, 0.01],  # L2-регуляризация
        "learning_rate_init": [0.001, 0.01],
    }
    base = MLPClassifier(max_iter=500, early_stopping=True, random_state=42)
    grid = GridSearchCV(base, param_grid, cv=5, scoring="f1", n_jobs=-1)
    grid.fit(X_train, y_train)
    
    print(f"Лучшие параметры: {grid.best_params_}")
    print(f"CV F1 (на train): {grid.best_score_:.4f}")
    
    return evaluate_model(grid.best_estimator_, X_test, y_test, "MLP")


# ============================================================
# MAIN
# ============================================================
def main():
    X_train, X_test, y_train, y_test = load_data()

    m_logreg, cm_logreg, proba_logreg = train_logreg(X_train, y_train, X_test, y_test)
    m_rf, cm_rf, proba_rf = train_rf(X_train, y_train, X_test, y_test)
    m_mlp, cm_mlp, proba_mlp = train_mlp(X_train, y_train, X_test, y_test)

    hr("8.6 СРАВНЕНИЕ МОДЕЛЕЙ")
    summary = pd.DataFrame([m_logreg, m_rf, m_mlp])
    print(summary.to_string(index=False))
    save_table(summary, "8_1_model_comparison")

    # Сохраним матрицы ошибок в текстовый файл для отчета
    with open("tables/8_2_confusion_matrices.txt", "w", encoding="utf-8") as f:
        f.write("Logistic Regression:\n" + str(cm_logreg) + "\n\n")
        f.write("Random Forest:\n" + str(cm_rf) + "\n\n")
        f.write("MLP:\n" + str(cm_mlp) + "\n")

    hr("ГОТОВО")
    print("  Таблицы: ./tables/8_*.csv")
    print("  График сравнения метрик будет в п.9 (вместе с ROC-кривыми)")


if __name__ == "__main__":
    main()