# ============================================================
# Лабораторная работа №3. Деревья и леса решений.
# Вариант 7.
# ============================================================
import numpy as np
import matplotlib.pyplot as plt

from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import roc_auc_score, roc_curve

from DataGenerator import norm_dataset, nonlinear_dataset_7

# Для ROC-кривых через scikit-plot (устанавливается отдельно):
#   pip install scikit-plot
try:
    import scikitplot as skplt
    HAS_SKPLT = True
except ImportError:
    HAS_SKPLT = False
    print("scikit-plot не установлен, ROC будет построен через sklearn.metrics.roc_curve")


# ============================================================
# ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (перенесены из лабы №2)
# ============================================================
def sensitivity_specificity(Y_true, Y_pred):
    """
    Ручной расчёт точности, чувствительности и специфичности.
    Класс 1 — наличие признака, класс 0 — отсутствие признака.
    """
    TP = TN = FP = FN = 0
    for yt, yp in zip(Y_true, Y_pred):
        if   yt == 1 and yp == 1: TP += 1
        elif yt == 0 and yp == 0: TN += 1
        elif yt == 0 and yp == 1: FP += 1
        elif yt == 1 and yp == 0: FN += 1

    total = TP + TN + FP + FN
    accuracy    = (TP + TN) / total if total     > 0 else 0.0
    sensitivity = TP / (TP + FN)    if (TP + FN) > 0 else 0.0
    specificity = TN / (TN + FP)    if (TN + FP) > 0 else 0.0
    return accuracy, sensitivity, specificity, (TP, TN, FP, FN)


def print_metrics_table(title, Y_train, Y_test, Pred_train, Pred_test):
    """Печатает таблицу метрик для train и test."""
    acc_tr, sens_tr, spec_tr, cm_tr = sensitivity_specificity(Y_train, Pred_train)
    acc_te, sens_te, spec_te, cm_te = sensitivity_specificity(Y_test,  Pred_test)

    print(f"\n===== {title} =====")
    print(f"Train: TP={cm_tr[0]}, TN={cm_tr[1]}, FP={cm_tr[2]}, FN={cm_tr[3]}")
    print(f"Test : TP={cm_te[0]}, TN={cm_te[1]}, FP={cm_te[2]}, FN={cm_te[3]}")
    print("-" * 66)
    print(f"{'':6}| {'Число объектов':>14} | {'Точность,%':>10} | "
          f"{'Чувств.,%':>9} | {'Специф.,%':>9}")
    print("-" * 66)
    print(f"{'Train':6}| {len(Y_train):>14} | {acc_tr*100:>10.2f} | "
          f"{sens_tr*100:>9.2f} | {spec_tr*100:>9.2f}")
    print(f"{'Test':6}| {len(Y_test):>14} | {acc_te*100:>10.2f} | "
          f"{sens_te*100:>9.2f} | {spec_te*100:>9.2f}")
    print("=" * 66)

    return {
        'train': (acc_tr, sens_tr, spec_tr),
        'test':  (acc_te, sens_te, spec_te),
    }


def plot_proba_histograms(Pred_train_proba, Pred_test_proba,
                          Y_train, Y_test,
                          title_suffix="", save_prefix="hist",
                          share_y=False):
    """
    Две гистограммы (train и test) распределения вероятностей класса 1.
    bins=20 и range=(0,1) — чтобы 0 и 1 не растягивались в «блоки».
    """
    fig, axes = plt.subplots(1, 2, figsize=(14, 5), sharey=share_y)

    # --- train ---
    axes[0].hist(Pred_train_proba[Y_train == 1, 1], bins=20, range=(0, 1),
                 alpha=0.7, color='hotpink', label='Класс 1 (истина)')
    axes[0].hist(Pred_train_proba[Y_train == 0, 1], bins=20, range=(0, 1),
                 alpha=0.7, color='blue',    label='Класс 0 (истина)')
    axes[0].set_title(f"Классификация, train {title_suffix}")
    axes[0].set_xlabel("Вероятность класса 1")
    axes[0].set_ylabel("Число объектов")
    axes[0].legend(); axes[0].grid(alpha=0.3)

    # --- test ---
    axes[1].hist(Pred_test_proba[Y_test == 1, 1], bins=20, range=(0, 1),
                 alpha=0.7, color='hotpink', label='Класс 1 (истина)')
    axes[1].hist(Pred_test_proba[Y_test == 0, 1], bins=20, range=(0, 1),
                 alpha=0.7, color='blue',    label='Класс 0 (истина)')
    axes[1].set_title(f"Классификация, test {title_suffix}")
    axes[1].set_xlabel("Вероятность класса 1")
    axes[1].set_ylabel("Число объектов")   # ← теперь подпись будет и здесь
    axes[1].legend(); axes[1].grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'{save_prefix}_hist_train_test.png', dpi=120)
    plt.show()

def split_train_test(X, Y, train_ratio=0.7):
    """Разбиение по порядку (данные уже перемешаны генератором)."""
    n = int(train_ratio * len(X))
    return X[:n], X[n:], Y[:n], Y[n:]


# ============================================================
# ОСНОВНАЯ ЧАСТЬ
# ============================================================
if __name__ == "__main__":

    np.random.seed(7)

    # ========================================================
    # ПУНКТ 1: Создание данных со средней степенью пересечения
    #          и разбиение на train/test
    # ========================================================
    # "Средняя степень пересечения" — облака близко, но не сливаются.
    # Берём параметры между хорошо разделимыми (σ=0.7) и
    # сильно пересекающимися (σ=1.5). Возьмём σ = 1.0 и умеренный сдвиг.
    N = 1000
    mu    = [(0.0, 0.0), (2.5, 2.5)]   # центры облаков
    sigma = [(1.0, 1.0), (1.0, 1.0)]   # средний разброс → среднее пересечение

    X, Y, class0, class1 = norm_dataset(mu, sigma, N)
    Xtrain, Xtest, Ytrain, Ytest = split_train_test(X, Y, 0.7)
    print(f"X: {X.shape}, Y: {Y.shape} | train: {len(Ytrain)}, test: {len(Ytest)}")

    # ========================================================
    # ПУНКТ 2: Обучение DecisionTreeClassifier
    # ========================================================
    tree = DecisionTreeClassifier(random_state=0)   # random_state=0 по заданию
    tree.fit(Xtrain, Ytrain)

    # Предсказания
    Pred_train_tree       = tree.predict(Xtrain)
    Pred_train_tree_proba = tree.predict_proba(Xtrain)
    Pred_test_tree        = tree.predict(Xtest)
    Pred_test_tree_proba  = tree.predict_proba(Xtest)

    # ========================================================
    # ПУНКТ 3: Метрики для дерева (train и test)
    # ========================================================
    print("\n" + "#" * 66)
    print("# ПУНКТ 3: Дерево решений (DecisionTreeClassifier)")
    print("#" * 66)

    acc_tr_tree = tree.score(Xtrain, Ytrain)
    acc_te_tree = tree.score(Xtest,  Ytest)
    print(f"score() train = {acc_tr_tree:.4f}")
    print(f"score() test  = {acc_te_tree:.4f}")

    results_tree = print_metrics_table(
        "Дерево решений (Decision Tree)",
        Ytrain, Ytest, Pred_train_tree, Pred_test_tree
    )

    # ========================================================
    # ПУНКТ 4: Обучение RandomForestClassifier + метрики
    # ========================================================
    print("\n" + "#" * 66)
    print("# ПУНКТ 4: Случайный лес (RandomForestClassifier)")
    print("#" * 66)

    forest = RandomForestClassifier(random_state=0, n_estimators=100)
    forest.fit(Xtrain, Ytrain)

    Pred_train_forest       = forest.predict(Xtrain)
    Pred_train_forest_proba = forest.predict_proba(Xtrain)
    Pred_test_forest        = forest.predict(Xtest)
    Pred_test_forest_proba  = forest.predict_proba(Xtest)

    acc_tr_forest = forest.score(Xtrain, Ytrain)
    acc_te_forest = forest.score(Xtest,  Ytest)
    print(f"score() train = {acc_tr_forest:.4f}")
    print(f"score() test  = {acc_te_forest:.4f}")

    results_forest = print_metrics_table(
        "Случайный лес (Random Forest, n_estimators=100)",
        Ytrain, Ytest, Pred_train_forest, Pred_test_forest
    )

    # Сравнительная таблица дерево vs лес
    print("\n" + "=" * 78)
    print("СРАВНЕНИЕ: ДЕРЕВО vs ЛЕС")
    print("=" * 78)
    print(f"{'Модель':<20}| {'Выборка':<7}| {'Точность,%':>10} | "
          f"{'Чувств.,%':>9} | {'Специф.,%':>9}")
    print("-" * 78)
    for name, res in [("Дерево", results_tree), ("Лес", results_forest)]:
        for split in ('train', 'test'):
            acc, sens, spec = res[split]
            print(f"{name:<20}| {split:<7}| "
                  f"{acc*100:>10.2f} | {sens*100:>9.2f} | {spec*100:>9.2f}")
    print("=" * 78)

    # ========================================================
    # ПУНКТ 5: ROC-кривые и AUC для дерева и леса
    # ========================================================
    print("\n" + "#" * 66)
    print("# ПУНКТ 5: ROC-кривые и AUC")
    print("#" * 66)

    # ---------- 5.1. ROC-кривая через scikit-plot (если установлен) ----------
    if HAS_SKPLT:
        print("\nROC-кривая для дерева (scikit-plot):")
        skplt.metrics.plot_roc_curve(Ytest, Pred_test_tree_proba, figsize=(8, 8))
        plt.title("ROC-кривая: Decision Tree")
        plt.savefig("roc_tree_skplt.png", dpi=120)
        plt.show()

        print("\nROC-кривая для леса (scikit-plot):")
        skplt.metrics.plot_roc_curve(Ytest, Pred_test_forest_proba, figsize=(8, 8))
        plt.title("ROC-кривая: Random Forest")
        plt.savefig("roc_forest_skplt.png", dpi=120)
        plt.show()

    # ---------- 5.2. ROC-кривая через sklearn (универсально) ----------
    fpr_tree, tpr_tree, _ = roc_curve(Ytest, Pred_test_tree_proba[:, 1])
    fpr_forest, tpr_forest, _ = roc_curve(Ytest, Pred_test_forest_proba[:, 1])

    auc_tree   = roc_auc_score(Ytest, Pred_test_tree_proba[:, 1])
    auc_forest = roc_auc_score(Ytest, Pred_test_forest_proba[:, 1])
    print(f"\nAUC tree   = {auc_tree:.4f}")
    print(f"AUC forest = {auc_forest:.4f}")

    plt.figure(figsize=(8, 8))
    plt.plot(fpr_tree,   tpr_tree,   label=f'Decision Tree (AUC = {auc_tree:.3f})',
             linewidth=2)
    plt.plot(fpr_forest, tpr_forest, label=f'Random Forest (AUC = {auc_forest:.3f})',
             linewidth=2)
    plt.plot([0, 1], [0, 1], 'k--', linewidth=1, label='Случайный классификатор')
    plt.xlabel('False Positive Rate (1 − Specificity)')
    plt.ylabel('True Positive Rate (Sensitivity)')
    plt.title('ROC-кривые: дерево vs лес')
    plt.legend(loc='lower right')
    plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.savefig("roc_compare.png", dpi=120)
    plt.show()

    # ========================================================
    # ПУНКТ 6: Гистограммы вероятностей для случайного леса
    # ========================================================
    print("\n" + "#" * 66)
    print("# ПУНКТ 6: Гистограммы вероятностей для случайного леса")
    print("#" * 66)

    plot_proba_histograms(
        Pred_train_forest_proba, Pred_test_forest_proba,
        Ytrain, Ytest,
        title_suffix="(Random Forest)",
        save_prefix="p6_forest"
    )

    # ---- (дополнительно, для сравнения) гистограммы для дерева ----
    # В задании этого нет, но полезно для отчёта — можно закомментировать.
    plot_proba_histograms(
        Pred_train_tree_proba, Pred_test_tree_proba,
        Ytrain, Ytest,
        title_suffix="(Decision Tree)",
        save_prefix="p6_tree"
    )