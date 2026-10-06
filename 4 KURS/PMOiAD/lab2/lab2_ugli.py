# ============================================================
# Лабораторная работа №2. Логистическая регрессия.
# Вариант 7.
#
# ВСЕ эксперименты используют nonlinear_dataset_7,
# но с разными геометрическими параметрами:
#   Пункты 1-3 :  почти разделимые Г-углы (t=0.05, сдвиг большой)
#   Пункт 4    :  плотно пересекающиеся Г-углы (t=0.8, сдвиг малый)
#   Пункт 5    :  стандартные Г-углы из методички (t=0.15)
# ============================================================
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression

from DataGenerator import nonlinear_dataset_7


# ============================================================
# ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
# ============================================================
def sensitivity_specificity(Y_true, Y_pred):
    """Ручной расчёт accuracy / sensitivity / specificity."""
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
                          title_suffix="",
                          save_prefix="logreg_proba"):
    """Две гистограммы (train и test) распределения вероятностей."""
    fig, axes = plt.subplots(1, 2, figsize=(14, 5), sharey=True)

    # train
    axes[0].hist(Pred_train_proba[Y_train == 1, 1], bins='auto',
                 alpha=0.7, color='hotpink', label='Класс 1 (истина)')
    axes[0].hist(Pred_train_proba[Y_train == 0, 1], bins='auto',
                 alpha=0.7, color='blue',    label='Класс 0 (истина)')
    axes[0].set_title(f"Классификация, train {title_suffix}")
    axes[0].set_xlabel("Вероятность принадлежности классу 1")
    axes[0].set_ylabel("Число объектов")
    axes[0].legend(); axes[0].grid(alpha=0.3)

    # test
    axes[1].hist(Pred_test_proba[Y_test == 1, 1], bins='auto',
                 alpha=0.7, color='hotpink', label='Класс 1 (истина)')
    axes[1].hist(Pred_test_proba[Y_test == 0, 1], bins='auto',
                 alpha=0.7, color='blue',    label='Класс 0 (истина)')
    axes[1].set_title(f"Классификация, test {title_suffix}")
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
    """Разбиение по порядку (данные уже перемешаны)."""
    n = int(train_ratio * len(X))
    return X[:n], X[n:], Y[:n], Y[n:]


def run_experiment(title, X, Y, class0, class1,
                   random_state=7, train_ratio=0.7,
                   save_prefix="exp", verbose_table=True):
    """
    Полный цикл эксперимента на одних данных:
    split -> fit -> predict -> метрики -> графики.
    Возвращает словарь метрик.
    """
    Xtr, Xte, Ytr, Yte = split_train_test(X, Y, train_ratio)

    clf = LogisticRegression(random_state=random_state,
                             solver='saga', max_iter=5000)
    clf.fit(Xtr, Ytr)

    Pred_tr       = clf.predict(Xtr)
    Pred_tr_proba = clf.predict_proba(Xtr)
    Pred_te       = clf.predict(Xte)
    Pred_te_proba = clf.predict_proba(Xte)

    if verbose_table:
        res = print_metrics_table(title, Ytr, Yte, Pred_tr, Pred_te)
    else:
        res = None

    plot_proba_histograms(Pred_tr_proba, Pred_te_proba, Ytr, Yte,
                          title_suffix=f"({title})",
                          save_prefix=save_prefix)

    plot_decision_boundary(class0, class1, X, clf,
                           title=title,
                           save_name=f"{save_prefix}_boundary.png")

    return res


# ============================================================
# ОСНОВНАЯ ЧАСТЬ
# ============================================================
if __name__ == "__main__":

    np.random.seed(7)     # воспроизводимость
    Nvar = 7              # номер варианта
    N    = 1000           # число точек в каждом классе

    results = {}

    # ========================================================
    # ПУНКТЫ 1-3: почти разделимые Г-углы
    #   - маленький шум t=0.05 (полосы тонкие),
    #   - большой сдвиг между углами.
    # ========================================================
    print("\n" + "#" * 66)
    print("# ПУНКТЫ 1-3: почти разделимые Г-углы (t=0.05, сдвиг большой)")
    print("#" * 66)

    X1, Y1, c0_1, c1_1 = nonlinear_dataset_7(
        N=N, seed=Nvar,
        L=3.0, S=1.0, t=0.05,        # тонкие полосы
        x0=0.0, y0=3.0,
        x1=4.5, y1=1.0               # сдвинуты сильнее
    )
    results['Г-углы почти разделимые (t=0.05)'] = run_experiment(
        title="Пункты 1-3: почти разделимые Г-углы",
        X=X1, Y=Y1, class0=c0_1, class1=c1_1,
        random_state=Nvar,
        save_prefix="p3_near_separable"
    )

    # ========================================================
    # ПУНКТ 4: плотно пересекающиеся Г-углы
    #   - большой шум t=0.8 (полосы толстые),
    #   - малый сдвиг между углами.
    # ========================================================
    print("\n" + "#" * 66)
    print("# ПУНКТ 4: плотно пересекающиеся Г-углы (t=0.8, сдвиг малый)")
    print("#" * 66)

    X4, Y4, c0_4, c1_4 = nonlinear_dataset_7(
        N=N, seed=Nvar,
        L=3.0, S=1.0, t=0.8,        # толстые полосы
        x0=0.0, y0=3.0,
        x1=2.5, y1=1.5              # углы ближе друг к другу
    )
    results['Г-углы плотно пересекающиеся (t=0.8)'] = run_experiment(
        title="Пункт 4: плотное пересечение Г-углов",
        X=X4, Y=Y4, class0=c0_4, class1=c1_4,
        random_state=Nvar,
        save_prefix="p4_overlap"
    )

    # ========================================================
    # ПУНКТ 5: стандартные Г-углы из методички
    # ========================================================
    print("\n" + "#" * 66)
    print("# ПУНКТ 5: стандартные Г-углы (t=0.15, из методички)")
    print("#" * 66)

    X5, Y5, c0_5, c1_5 = nonlinear_dataset_7(
        N=N, seed=Nvar,
        L=3.0, S=1.0, t=0.15,
        x0=0.0, y0=3.0,
        x1=3.5, y1=2.0
    )
    results['Г-углы стандартные (t=0.15)'] = run_experiment(
        title="Пункт 5: стандартные нелинейные Г-углы",
        X=X5, Y=Y5, class0=c0_5, class1=c1_5,
        random_state=Nvar,
        save_prefix="p5_nonlinear"
    )

    # ========================================================
    # ИТОГОВАЯ СВОДНАЯ ТАБЛИЦА
    # ========================================================
    print("\n" + "=" * 82)
    print("ИТОГОВАЯ СВОДНАЯ ТАБЛИЦА")
    print("=" * 82)
    header = (f"{'Тип данных':<36}| {'Выборка':<7}| "
              f"{'Точность,%':>10} | {'Чувств.,%':>9} | {'Специф.,%':>9}")
    print(header)
    print("-" * 82)

    for name, res in results.items():
        if res is None:
            continue
        for split in ('train', 'test'):
            acc, sens, spec = res[split]
            print(f"{name:<36}| {split:<7}| "
                  f"{acc*100:>10.2f} | {sens*100:>9.2f} | {spec*100:>9.2f}")
    print("=" * 82)