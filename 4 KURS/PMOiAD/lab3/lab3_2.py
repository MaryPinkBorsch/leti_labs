# ============================================================
# Лабораторная работа №3. Деревья и леса решений.
# ЗАДАНИЕ ДЛЯ САМОСТОЯТЕЛЬНОЙ РАБОТЫ. Вариант 7.
# ============================================================
import numpy as np
import matplotlib.pyplot as plt

from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import roc_auc_score

from DataGenerator import norm_dataset, nonlinear_dataset_7


# ============================================================
# ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
# ============================================================
def sensitivity_specificity(Y_true, Y_pred):
    """
    Ручной расчёт точности, чувствительности и специфичности
    (класс 1 — наличие признака, класс 0 — отсутствие признака).
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
                          title_suffix="", save_prefix="hist"):
    """
    Гистограммы вероятностей класса 1 для train и test.
    bins=20, range=(0,1) — чтобы значения 0 и 1 не растягивались в блоки.
    """
    fig, axes = plt.subplots(1, 2, figsize=(14, 5), sharey=True)

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
    axes[1].set_ylabel("Число объектов")
    axes[1].tick_params(labelleft=True)   # подписи Y и справа
    axes[1].legend(); axes[1].grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'{save_prefix}___hist_train_test.png', dpi=120)
    plt.show()


def split_train_test(X, Y, train_ratio=0.7):
    n = int(train_ratio * len(X))
    return X[:n], X[n:], Y[:n], Y[n:]


def plot_roc_compare(y_true, proba_tree, proba_forest, save_name="roc___compare.png"):
    """ROC-кривые для дерева и леса на одном графике + AUC в легенде."""
    from sklearn.metrics import roc_curve

    fpr_t, tpr_t, _ = roc_curve(y_true, proba_tree)
    fpr_f, tpr_f, _ = roc_curve(y_true, proba_forest)
    auc_t = roc_auc_score(y_true, proba_tree)
    auc_f = roc_auc_score(y_true, proba_forest)

    plt.figure(figsize=(8, 8))
    plt.plot(fpr_t, tpr_t, linewidth=2,
             label=f'Decision Tree (AUC = {auc_t:.3f})')
    plt.plot(fpr_f, tpr_f, linewidth=2,
             label=f'Random Forest (AUC = {auc_f:.3f})')
    plt.plot([0, 1], [0, 1], 'k--', linewidth=1, label='Случайный классификатор')
    plt.xlabel('False Positive Rate (1 − Specificity)')
    plt.ylabel('True Positive Rate (Sensitivity)')
    plt.title('ROC-кривые: дерево vs лес')
    plt.legend(loc='lower right')
    plt.grid(alpha=0.3)
    plt.tight_layout()
    #plt.savefig(save_name, dpi=120)
    plt.show()

    return auc_t, auc_f


# ============================================================
# ОСНОВНАЯ ЧАСТЬ
# ============================================================
if __name__ == "__main__":

    np.random.seed(7)

    # ################################################################
    # ПУНКТЫ 1–3: ДАННЫЕ СО СРЕДНЕЙ СТЕПЕНЬЮ ПЕРЕСЕЧЕНИЯ,
    #             ОБУЧЕНИЕ ДЕРЕВА, МЕТРИКИ
    # ################################################################
    print("\n" + "#" * 70)
    print("# ПУНКТЫ 1-3: DecisionTreeClassifier, среднее пересечение")
    print("#" * 70)

    N = 1000
    mu    = [(0.0, 0.0), (2.5, 2.5)]
    sigma = [(1.0, 1.0), (1.0, 1.0)]

    X, Y, class0, class1 = norm_dataset(mu, sigma, N)
    Xtrain, Xtest, Ytrain, Ytest = split_train_test(X, Y, 0.7)
    print(f"X: {X.shape}, Y: {Y.shape} | "
          f"train: {len(Ytrain)}, test: {len(Ytest)}")

    # --- дерево ---
    tree = DecisionTreeClassifier(random_state=0)
    tree.fit(Xtrain, Ytrain)

    Pred_train_tree       = tree.predict(Xtrain)
    Pred_train_tree_proba = tree.predict_proba(Xtrain)
    Pred_test_tree        = tree.predict(Xtest)
    Pred_test_tree_proba  = tree.predict_proba(Xtest)

    print(f"\nTree score: train = {tree.score(Xtrain, Ytrain):.4f}, "
          f"test = {tree.score(Xtest, Ytest):.4f}")

    results_tree = print_metrics_table(
        "Дерево решений (среднее пересечение)",
        Ytrain, Ytest, Pred_train_tree, Pred_test_tree
    )

    # --- лес (пункт 4) ---
    forest = RandomForestClassifier(random_state=0, n_estimators=100)
    forest.fit(Xtrain, Ytrain)

    Pred_train_forest       = forest.predict(Xtrain)
    Pred_train_forest_proba = forest.predict_proba(Xtrain)
    Pred_test_forest        = forest.predict(Xtest)
    Pred_test_forest_proba  = forest.predict_proba(Xtest)

    print(f"\nForest score: train = {forest.score(Xtrain, Ytrain):.4f}, "
          f"test = {forest.score(Xtest, Ytest):.4f}")

    results_forest = print_metrics_table(
        "Случайный лес (среднее пересечение, n_estimators=100)",
        Ytrain, Ytest, Pred_train_forest, Pred_test_forest
    )

    # ---- сводная таблица по пункту 3 ----
    print("\n" + "=" * 78)
    print("ПУНКТ 3: СВОДНАЯ ТАБЛИЦА (дерево и лес, среднее пересечение)")
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

    # ---- ROC (пункт 5) ----
    print("\n" + "#" * 70)
    print("# ПУНКТ 5: ROC-кривые и AUC (среднее пересечение)")
    print("#" * 70)
    auc_tree, auc_forest = plot_roc_compare(
        Ytest, Pred_test_tree_proba[:, 1], Pred_test_forest_proba[:, 1],
        save_name="p5_roc_medium.png"
    )
    print(f"AUC tree   = {auc_tree:.4f}")
    print(f"AUC forest = {auc_forest:.4f}")

    # ---- Гистограммы (пункт 6) ----
    print("\n" + "#" * 70)
    print("# ПУНКТ 6: Гистограммы вероятностей для леса")
    print("#" * 70)
    plot_proba_histograms(
        Pred_train_forest_proba, Pred_test_forest_proba,
        Ytrain, Ytest,
        title_suffix="(Random Forest)",
        save_prefix="p6_forest"
    )

    # ################################################################
    # ПУНКТ 4: НЕЛИНЕЙНО-ПЕРЕСЕКАЕМЫЕ КЛАССЫ (Г-ОБРАЗНЫЕ УГЛЫ)
    # ################################################################
    print("\n" + "#" * 70)
    print("# ПУНКТ 4: Нелинейные (Г-образные) классы")
    print("#" * 70)

    X5, Y5, c0_5, c1_5 = nonlinear_dataset_7(N=N, seed=7)
    Xtr5, Xte5, Ytr5, Yte5 = split_train_test(X5, Y5, 0.7)

    # --- дерево на Г-углах ---
    tree5 = DecisionTreeClassifier(random_state=0)
    tree5.fit(Xtr5, Ytr5)

    Pred_tr5_tree       = tree5.predict(Xtr5)
    Pred_te5_tree       = tree5.predict(Xte5)
    Pred_tr5_tree_proba = tree5.predict_proba(Xtr5)
    Pred_te5_tree_proba = tree5.predict_proba(Xte5)

    results_tree5 = print_metrics_table(
        "Дерево решений на Г-углах",
        Ytr5, Yte5, Pred_tr5_tree, Pred_te5_tree
    )

    # --- лес на Г-углах ---
    forest5 = RandomForestClassifier(random_state=0, n_estimators=100)
    forest5.fit(Xtr5, Ytr5)

    Pred_tr5_forest       = forest5.predict(Xtr5)
    Pred_te5_forest       = forest5.predict(Xte5)
    Pred_tr5_forest_proba = forest5.predict_proba(Xtr5)
    Pred_te5_forest_proba = forest5.predict_proba(Xte5)

    results_forest5 = print_metrics_table(
        "Случайный лес на Г-углах",
        Ytr5, Yte5, Pred_tr5_forest, Pred_te5_forest
    )

    auc_tree5, auc_forest5 = plot_roc_compare(
        Yte5, Pred_te5_tree_proba[:, 1], Pred_te5_forest_proba[:, 1],
        save_name="p4_roc_nonlinear.png"
    )
    print(f"AUC tree   (Г-углы) = {auc_tree5:.4f}")
    print(f"AUC forest (Г-углы) = {auc_forest5:.4f}")

    # ################################################################
    # ПУНКТ 5: ПОДБОР ГИПЕРПАРАМЕТРОВ ДЕРЕВА (max_depth)
    #          ДЛЯ СНИЖЕНИЯ ПЕРЕОБУЧЕНИЯ
    # ################################################################
    print("\n" + "#" * 70)
    print("# ПУНКТ 5: Подбор max_depth для снижения переобучения")
    print("#" * 70)

    # Используем данные со средним пересечением (пункты 1-3)
    depths = range(1, 21)         # от 1 до 20
    train_scores, test_scores = [], []

    print(f"\n{'max_depth':>10} | {'train_acc':>10} | {'test_acc':>10} | "
          f"{'test_AUC':>10}")
    print("-" * 52)

    best_depth = None
    best_auc = -1.0

    for d in depths:
        clf_d = DecisionTreeClassifier(random_state=0, max_depth=d)
        clf_d.fit(Xtrain, Ytrain)

        acc_tr = clf_d.score(Xtrain, Ytrain)
        acc_te = clf_d.score(Xtest,  Ytest)

        proba_te = clf_d.predict_proba(Xtest)[:, 1]
        auc_te = roc_auc_score(Ytest, proba_te)

        train_scores.append(acc_tr)
        test_scores.append(acc_te)

        print(f"{d:>10} | {acc_tr:>10.4f} | {acc_te:>10.4f} | {auc_te:>10.4f}")

        if auc_te > best_auc:
            best_auc = auc_te
            best_depth = d

    print("-" * 52)
    print(f"Лучшая глубина: max_depth = {best_depth}, AUC = {best_auc:.4f}")

    # График зависимости точности от глубины
    plt.figure(figsize=(9, 5))
    plt.plot(list(depths), train_scores, marker='o', label='Train accuracy')
    plt.plot(list(depths), test_scores,  marker='s', label='Test accuracy')
    plt.axvline(best_depth, color='red', linestyle='--',
                label=f'Лучшая глубина = {best_depth}')
    plt.xlabel('max_depth')
    plt.ylabel('Accuracy')
    plt.title('Зависимость точности дерева от глубины')
    plt.legend(); plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.savefig("p5___max_depth.png", dpi=120)
    plt.show()

    # Обучим финальное дерево с лучшей глубиной и покажем метрики
    print("\nМетрики дерева с оптимальной глубиной:")
    tree_best = DecisionTreeClassifier(random_state=0, max_depth=best_depth)
    tree_best.fit(Xtrain, Ytrain)
    Pred_tr_best = tree_best.predict(Xtrain)
    Pred_te_best = tree_best.predict(Xtest)
    print_metrics_table(
        f"Дерево с max_depth={best_depth}",
        Ytrain, Ytest, Pred_tr_best, Pred_te_best
    )

    # ################################################################
    # ПУНКТ 6: ПОДБОР n_estimators ДЛЯ ЛЕСА
    #          (от 1 до 300 с шагом 10)
    # ################################################################
    print("\n" + "#" * 70)
    print("# ПУНКТ 6: Подбор n_estimators для случайного леса")
    print("#" * 70)

    n_estimators_range = range(1, 301, 10)
    aucs = []

    print(f"\n{'n_estimators':>14} | {'AUC (test)':>10}")
    print("-" * 28)

    best_n = None
    best_n_auc = -1.0

    for n in n_estimators_range:
        rf = RandomForestClassifier(random_state=0, n_estimators=n)
        rf.fit(Xtrain, Ytrain)
        proba = rf.predict_proba(Xtest)[:, 1]
        auc = roc_auc_score(Ytest, proba)
        aucs.append(auc)

        print(f"{n:>14} | {auc:>10.4f}")

        if auc > best_n_auc:
            best_n_auc = auc
            best_n = n

    print("-" * 28)
    print(f"Лучшее n_estimators = {best_n}, AUC = {best_n_auc:.4f}")

    # График зависимости AUC от n_estimators
    plt.figure(figsize=(10, 5))
    plt.plot(list(n_estimators_range), aucs, marker='o', color='darkorange')
    plt.axvline(best_n, color='red', linestyle='--',
                label=f'Лучшее n_estimators = {best_n} (AUC = {best_n_auc:.4f})')
    plt.xlabel('n_estimators (число деревьев в лесу)')
    plt.ylabel('AUC на тестовой выборке')
    plt.title('Зависимость AUC от числа деревьев в случайном лесе')
    plt.legend(); plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.savefig("p6___n_estimators.png", dpi=120)
    plt.show()

    # ################################################################
    # ИТОГОВАЯ СВОДНАЯ ТАБЛИЦА
    # ################################################################
    print("\n" + "=" * 90)
    print("ИТОГОВАЯ СВОДНАЯ ТАБЛИЦА")
    print("=" * 90)
    print(f"{'Данные / Модель':<42}| {'Выборка':<7}| {'Точн.,%':>9} | "
          f"{'Чувств.,%':>10} | {'Спец.,%':>9}")
    print("-" * 90)

    all_results = [
        ("Среднее пересечение / Дерево",              results_tree),
        ("Среднее пересечение / Лес",                  results_forest),
        ("Г-углы (нелинейные) / Дерево",               results_tree5),
        ("Г-углы (нелинейные) / Лес",                  results_forest5),
    ]

    for name, res in all_results:
        for split in ('train', 'test'):
            acc, sens, spec = res[split]
            print(f"{name:<42}| {split:<7}| "
                  f"{acc*100:>9.2f} | {sens*100:>10.2f} | {spec*100:>9.2f}")

    print("-" * 90)
    print(f"AUC (среднее пересечение): дерево = {auc_tree:.4f}, "
          f"лес = {auc_forest:.4f}")
    print(f"AUC (Г-углы):              дерево = {auc_tree5:.4f}, "
          f"лес = {auc_forest5:.4f}")
    print(f"Лучшая глубина дерева: max_depth = {best_depth} (AUC = {best_auc:.4f})")
    print(f"Лучшее число деревьев:  n_estimators = {best_n} (AUC = {best_n_auc:.4f})")
    print("=" * 90)