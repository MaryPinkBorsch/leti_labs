import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression

#  генератор из лабы №1 (Г-образные углы) 
def nonlinear_dataset_7(N=1000, seed=None):
    """
    Генерирует двумерный датасет из двух классов,
    каждый класс — Г-образный угол (две перпендикулярные полосы).
    """
    rng = np.random.default_rng(seed)

    #  общие геометрические параметры 
    L  = 3.0    # длина длинной полосы
    S  = 1.0    # длина короткой полосы
    t  = 0.15   # половина "толщины" полосы (СКО шума по нормали)

    #  класс 0: верхний левый угол 
    x0, y0 = 0.0, 3.0

    n_h = int(round(N * L / (L + S)))
    n_v = N - n_h

    xh0 = rng.uniform(x0, x0 + L, n_h)
    yh0 = y0 + rng.normal(0.0, t, n_h)

    xv0 = x0 + rng.normal(0.0, t, n_v)
    yv0 = rng.uniform(y0 - S, y0, n_v)

    class0 = np.column_stack([
        np.concatenate([xh0, xv0]),
        np.concatenate([yh0, yv0]),
    ])

    #  класс 1: нижний правый угол 
    x1, y1 = 3.5, 2.0

    n_h = int(round(N * L / (L + S)))
    n_v = N - n_h

    xv1 = x1 + rng.normal(0.0, t, n_v)
    yv1 = rng.uniform(y1, y1 + S, n_v)

    xh1 = rng.uniform(x1 - L, x1, n_h)
    yh1 = y1 + rng.normal(0.0, t, n_h)

    class1 = np.column_stack([
        np.concatenate([xh1, xv1]),
        np.concatenate([yh1, yv1]),
    ])

    #  объединяем, формируем метки 
    X = np.vstack([class0, class1])
    Y = np.concatenate([
        np.zeros(len(class0), dtype=int),
        np.ones(len(class1),  dtype=int),
    ])

    #  перемешиваем 
    idx = rng.permutation(len(X))
    X = X[idx]
    Y = Y[idx]

    return X, Y, class0, class1


#  0. Воспроизводимость 
np.random.seed(7)

#  1. Генерация данных (Г-образные углы) 
N = 1000
X, Y, class0, class1 = nonlinear_dataset_7(N=N, seed=7)
print("X:", X.shape, "Y:", Y.shape)

#  2. Train / test 
trainCount = int(0.7 * len(X))
Xtrain, Xtest = X[:trainCount], X[trainCount:]
Ytrain, Ytest = Y[:trainCount], Y[trainCount:]
print(f"train: {len(Xtrain)}, test: {len(Xtest)}")

#  3. Обучение 
Nvar = 7
clf = LogisticRegression(random_state=Nvar, solver='saga', max_iter=5000)
clf.fit(Xtrain, Ytrain)

#  4. Предсказания 
Pred_train       = clf.predict(Xtrain)
Pred_train_proba = clf.predict_proba(Xtrain)

Pred_test        = clf.predict(Xtest)
Pred_test_proba  = clf.predict_proba(Xtest)

#  5. Точность 
acc_train  = clf.score(Xtrain, Ytrain)
acc_test   = clf.score(Xtest,  Ytest)
acc_manual = sum(Pred_test == Ytest) / len(Ytest)

print(f"acc_train = {acc_train:.4f}")
print(f"acc_test  = {acc_test:.4f}  (вручную: {acc_manual:.4f})")

#  6. Гистограмма вероятностей: train 
plt.figure(figsize=(8, 5))
plt.hist(Pred_train_proba[Ytrain == 1, 1], bins='auto',
         alpha=0.7, color='hotpink', label='Класс 1 (истина)')
plt.hist(Pred_train_proba[Ytrain == 0, 1], bins='auto',
         alpha=0.7, color='blue',    label='Класс 0 (истина)')
plt.title("Результаты классификации, train")
plt.xlabel("Вероятность принадлежности классу 1")
plt.ylabel("Число объектов")
plt.legend()
plt.grid(alpha=0.3)
plt.tight_layout()
plt.savefig('logreg_proba_hist_train.png', dpi=120)
plt.show()

#  6b. Гистограмма вероятностей: test 
plt.figure(figsize=(8, 5))
plt.hist(Pred_test_proba[Ytest == 1, 1], bins='auto',
         alpha=0.7, color='hotpink', label='Класс 1 (истина)')
plt.hist(Pred_test_proba[Ytest == 0, 1], bins='auto',
         alpha=0.7, color='blue',    label='Класс 0 (истина)')
plt.title("Результаты классификации, тест")
plt.xlabel("Вероятность принадлежности классу 1")
plt.ylabel("Число объектов")
plt.legend()
plt.grid(alpha=0.3)
plt.tight_layout()
plt.savefig('logreg_proba_hist_test.png', dpi=120)
plt.show()

#  7. Разделяющая прямая 
plt.figure(figsize=(7, 7))
plt.scatter(class0[:, 0], class0[:, 1], s=10, alpha=0.4, color='blue',    label='Класс 0 (верхний-левый угол)')
plt.scatter(class1[:, 0], class1[:, 1], s=10, alpha=0.4, color='hotpink', label='Класс 1 (нижний-правый угол)')

w = clf.coef_[0]
b = clf.intercept_[0]
x_vals = np.array([X[:, 0].min() - 0.5, X[:, 0].max() + 0.5])
y_vals = -(w[0] * x_vals + b) / w[1]
plt.plot(x_vals, y_vals, 'k--', linewidth=2, label='Разделяющая прямая')

plt.xlabel('Признак 1')
plt.ylabel('Признак 2')
plt.title('Логистическая регрессия на Г-образных углах (вариант 7)')
plt.legend()
plt.grid(alpha=0.3)
plt.axis('equal')
plt.tight_layout()
plt.savefig('logreg_decision_boundary.png', dpi=120)
plt.show()