# ============================================================
# Лабораторная работа №2. Логистическая регрессия.
# Вариант 7.
# ============================================================
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression

from DataGenerator import norm_dataset, nonlinear_dataset_7


# ============================================================
# ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
# ============================================================
def sensitivity_specificity(Y_true, Y_pred):
    """
    Ручной расчёт точности, чувствительности и специфичности
    (без sklearn.metrics).

    Класс 1 — наличие признака,
    класс 0 — отсутствие признака.

    Возвращает:
      accuracy, sensitivity (TPR), specificity (TNR), (TP, TN, FP, FN)
    """
    TP = TN = FP = FN = 0
    for yt, yp in zip(Y_true, Y_pred):
        if   yt == 1 and yp == 1: TP += 1
        elif yt == 0 and yp == 0: TN += 1
        elif yt == 0 and yp == 1: FP += 1
        elif yt == 1 and yp == 0: FN += 1

    total = TP + TN + FP + FN
    accuracy    = (TP + TN) / total        if total        > 0 else 0.0
    sensitivity = TP / (TP + FN)           if (TP + FN)    > 0 else 0.0
    specificity = TN / (TN + FP)           if (TN + FP)    > 0 else 0.0
    return accuracy, sensitivity, specificity, (TP, TN, FP, FN)


def print_metrics_table(title, Y_train, Y_test, Pred_train, Pred_test):
    """Печатает красивую таблицу с метриками для train и test."""
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
                          title_suffix="",
                          save_prefix="logreg_proba"):
    """Две гистограммы (train и test) с распределением вероятностей."""
    fig, axes = plt.subplots(1, 2, figsize=(14, 5), sharey=False)

    # --- train ---
    axes[0].hist(Pred_train_proba[Y_train == 1, 1], bins='auto',
                 alpha=0.7, color='hotpink', label='Класс 1 (истина)')
    axes[0].hist(Pred_train_proba[Y_train == 0, 1], bins='auto',
                 alpha=0.7, color='blue',    label='Класс 0 (истина)')
    axes[0].set_title(f"Результаты классификации, train {title_suffix}")
    axes[0].set_xlabel("Вероятность принадлежности классу 1")
    axes[0].set_ylabel("Число объектов")
    axes[0].legend(); axes[0].grid(alpha=0.3)

    # --- test ---
    axes[1].hist(Pred_test_proba[Y_test == 1, 1], bins='auto',
                 alpha=0.7, color='hotpink', label='Класс 1 (истина)')
    axes[1].hist(Pred_test_proba[Y_test == 0, 1], bins='auto',
                 alpha=0.7, color='blue',    label='Класс 0 (истина)')
    axes[1].set_title(f"Результаты классификации, test {title_suffix}")
    axes[1].set_xlabel("Вероятность принадлежности классу 1")
    axes[1].legend(); axes[1].grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'{save_prefix}_hist_train_test.png', dpi=120)
    plt.show()


def plot_decision_boundary(class0, class1, X, clf,
                           title="Логистическая регрессия",
                           save_name="decision_boundary.png"):
    """Разделяющая прямая + облака точек двух классов."""
    plt.figure(figsize=(7, 7))
    plt.scatter(class0[:, 0], class0[:, 1], s=10, alpha=0.4,
                color='blue',    label='Класс 0')
    plt.scatter(class1[:, 0], class1[:, 1], s=10, alpha=0.4,
                color='hotpink', label='Класс 1')

    w = clf.coef_[0]
    b = clf.intercept_[0]
    x_vals = np.array([X[:, 0].min() - 0.5, X[:, 0].max() + 0.5])
    y_vals = -(w[0] * x_vals + b) / w[1]
    plt.plot(x_vals, y_vals, 'k--', linewidth=2, label='Разделяющая прямая')

    plt.xlabel('Признак 1')
    plt.ylabel('Признак 2')
    plt.title(title)
    plt.legend(); plt.grid(alpha=0.3); plt.axis('equal')
    plt.tight_layout()
    plt.savefig(save_name, dpi=120)
    plt.show()


def split_train_test(X, Y, train_ratio=0.7):
    """Простое разбиение по порядку (данные уже перемешаны)."""
    n = int(train_ratio * len(X))
    return X[:n], X[n:], Y[:n], Y[n:]


# ============================================================
# ОСНОВНАЯ ЧАСТЬ
# ============================================================
if __name__ == "__main__":

    # -------- общие настройки --------
    np.random.seed(7)          # воспроизводимость (глобальный seed)
    Nvar = 7                   # номер варианта
    N = 1000                   # число точек в каждом классе

    results = {}               # сводные результаты для итоговой таблицы

    # ========================================================
    # ПУНКТ 1–3: ЛИНЕЙНО РАЗДЕЛИМЫЕ ДАННЫЕ (базовый случай)
    # ========================================================
    print("\n" + "#" * 66)
    print("# ПУНКТ 1-3: линейно разделимые данные (σ=0.7)")
    print("#" * 66)

    mu    = [(0.0, 0.0), (5.0, 5.0)]
    sigma = [(0.7, 0.7), (0.7, 0.7)]

    X, Y, class0, class1 = norm_dataset(mu, sigma, N)
    Xtrain, Xtest, Ytrain, Ytest = split_train_test(X, Y, 0.7)
    print(f"X: {X.shape}, Y: {Y.shape} | train: {len(Ytrain)}, test: {len(Ytest)}")

    clf = LogisticRegression(random_state=Nvar, solver='saga', max_iter=5000)
    clf.fit(Xtrain, Ytrain)

    Pred_train       = clf.predict(Xtrain)
    Pred_train_proba = clf.predict_proba(Xtrain)
    Pred_test        = clf.predict(Xtest)
    Pred_test_proba  = clf.predict_proba(Xtest)

    # accuracy через score() и вручную — для сверки
    print(f"score() train = {clf.score(Xtrain, Ytrain):.4f}")
    print(f"score() test  = {clf.score(Xtest,  Ytest):.4f}")
    print(f"acc вручную   = {sum(Pred_test == Ytest)/len(Ytest):.4f}")

    results['Разделимые (σ=0.7)'] = print_metrics_table(
        "Пункт 1-3: линейно разделимые",
        Ytrain, Ytest, Pred_train, Pred_test
    )

    plot_proba_histograms(Pred_train_proba, Pred_test_proba, Ytrain, Ytest,
                          title_suffix="(σ=0.7)",
                          save_prefix="p3_separable")

    plot_decision_boundary(class0, class1, X, clf,
                           title="Пункт 1-3: линейно разделимые классы",
                           save_name="p3_separable_boundary.png")

    # ========================================================
    # ПУНКТ 4: ПЛОТНОЕ ПЕРЕСЕЧЕНИЕ КЛАССОВ (линейные данные)
    # ========================================================
    print("\n" + "#" * 66)
    print("# ПУНКТ 4: линейные данные с сильным пересечением (σ=1.5)")
    print("#" * 66)

    np.random.seed(7)          # сброс для чистоты эксперимента
    mu4    = [(0.0, 0.0), (1.5, 1.5)]
    sigma4 = [(1.5, 1.5), (1.5, 1.5)]

    X4, Y4, c0_4, c1_4 = norm_dataset(mu4, sigma4, N)
    Xtr4, Xte4, Ytr4, Yte4 = split_train_test(X4, Y4, 0.7)

    clf4 = LogisticRegression(random_state=Nvar, solver='saga', max_iter=5000)
    clf4.fit(Xtr4, Ytr4)

    Pred_tr4       = clf4.predict(Xtr4)
    Pred_tr4_proba = clf4.predict_proba(Xtr4)
    Pred_te4       = clf4.predict(Xte4)
    Pred_te4_proba = clf4.predict_proba(Xte4)

    results['Пересекающиеся (σ=1.5)'] = print_metrics_table(
        "Пункт 4: сильно пересекающиеся классы",
        Ytr4, Yte4, Pred_tr4, Pred_te4
    )

    plot_proba_histograms(Pred_tr4_proba, Pred_te4_proba, Ytr4, Yte4,
                          title_suffix="(σ=1.5)",
                          save_prefix="p4_overlap")

    plot_decision_boundary(c0_4, c1_4, X4, clf4,
                           title="Пункт 4: плотное пересечение классов",
                           save_name="p4_overlap_boundary.png")

    # ========================================================
    # ПУНКТ 5: НЕЛИНЕЙНО ПЕРЕСЕКАЕМЫЕ КЛАССЫ (Г-образные углы)
    # ========================================================
    print("\n" + "#" * 66)
    print("# ПУНКТ 5: нелинейные (Г-образные) классы")
    print("#" * 66)

    X5, Y5, c0_5, c1_5 = nonlinear_dataset_7(N=N, seed=Nvar)
    Xtr5, Xte5, Ytr5, Yte5 = split_train_test(X5, Y5, 0.7)

    clf5 = LogisticRegression(random_state=Nvar, solver='saga', max_iter=5000)
    clf5.fit(Xtr5, Ytr5)

    Pred_tr5       = clf5.predict(Xtr5)
    Pred_tr5_proba = clf5.predict_proba(Xtr5)
    Pred_te5       = clf5.predict(Xte5)
    Pred_te5_proba = clf5.predict_proba(Xte5)

    results['Нелинейные (Г-углы)'] = print_metrics_table(
        "Пункт 5: нелинейно пересекаемые классы",
        Ytr5, Yte5, Pred_tr5, Pred_te5
    )

    plot_proba_histograms(Pred_tr5_proba, Pred_te5_proba, Ytr5, Yte5,
                          title_suffix="(Г-углы)",
                          save_prefix="p5_nonlinear")

    plot_decision_boundary(c0_5, c1_5, X5, clf5,
                           title="Пункт 5: нелинейные классы (Г-образные углы)",
                           save_name="p5_nonlinear_boundary.png")

    # ========================================================
    # ИТОГОВАЯ СВОДНАЯ ТАБЛИЦА
    # ========================================================
    print("\n" + "=" * 78)
    print("ИТОГОВАЯ СВОДНАЯ ТАБЛИЦА (все эксперименты)")
    print("=" * 78)
    header = (f"{'Тип данных':<28}| {'Выборка':<7}| "
              f"{'Точность,%':>10} | {'Чувств.,%':>9} | {'Специф.,%':>9}")
    print(header)
    print("-" * 78)

    for name, res in results.items():
        for split in ('train', 'test'):
            acc, sens, spec = res[split]
            print(f"{name:<28}| {split:<7}| "
                  f"{acc*100:>10.2f} | {sens*100:>9.2f} | {spec*100:>9.2f}")
    print("=" * 78)