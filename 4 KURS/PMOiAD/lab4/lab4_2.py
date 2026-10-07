import numpy as np
import matplotlib.pyplot as plt

import DataGenerator as dg   # ваш DataGenerator


 
# ПУНКТ 2 сам. работы: функции активации (из хода работы)
 
def sigmoid(Z):
    return 1 / (1 + np.exp(-Z))


def sigmoid_derivative(p):
    """σ'(z) = σ(z)·(1 − σ(z)) = p·(1 − p)"""
    return p * (1 - p)


 
# Класс нейронной сети с дополнительным методом test()
 
class NeuralNetwork:
    """
    Простейшая нейронная сеть:
        вход (n_in) → скрытый слой (n_neuro, сигмоида) → выход (1, сигмоида)

    Методы:
        __init__     — инициализация
        feedforward  — прямой проход
        backprop     — обратное распространение
        train        — одна эпоха обучения
        test         — предсказание для новых данных (пункт 2 сам. работы)
    """

    def __init__(self, x, y, n_neuro=4, random_state=None):
        if random_state is not None:
            np.random.seed(random_state)

        self.input = x
        n_in = self.input.shape[1]

        # Веса случайными значениями [0, 1)
        self.weights1 = np.random.rand(n_in, n_neuro)
        self.weights2 = np.random.rand(n_neuro, 1)

        self.y = y
        self.output = np.zeros(y.shape)

    def feedforward(self, X=None):
        """
        Прямой проход.
        Если X=None — используем обучающие данные self.input.
        Если X передан — используем его (для test).
        """
        if X is None:
            X = self.input
        layer1 = sigmoid(np.dot(X, self.weights1))
        layer2 = sigmoid(np.dot(layer1, self.weights2))
        return layer2

    def backprop(self):
        """Обратное распространение + обновление весов."""
        # Градиент по выходному слою
        d_weights2 = np.dot(
            self.layer1.T,
            2 * (self.y - self.output) * sigmoid_derivative(self.output)
        )
        # Градиент по скрытому слою
        d_weights1 = np.dot(
            self.input.T,
            np.dot(
                2 * (self.y - self.output) * sigmoid_derivative(self.output),
                self.weights2.T
            ) * sigmoid_derivative(self.layer1)
        )
        # Обновление
        self.weights1 += d_weights1
        self.weights2 += d_weights2

    def train(self, X, y):
        """Одна эпоха: прямой проход + обратный."""
        self.input = X          # переключаемся на обучающие данные
        self.y = y
        self.output = self.feedforward()
        self.layer1 = sigmoid(np.dot(self.input, self.weights1))  # сохраняем для backprop
        self.backprop()

    def test(self, X, y):
        """
        ПУНКТ 2 (со звёздочкой): оценка на тестовой выборке.
        Возвращает: метки (0/1), вероятности, точность.
        """
        proba = self.feedforward(X)
        pred = (proba >= 0.5).astype(int)
        acc = np.mean(pred == y.astype(int))
        return pred, proba, acc


 
# Вспомогательные функции
 
def accuracy(y_true, proba):
    """Точность по вероятностям с порогом 0.5."""
    pred = (proba >= 0.5).astype(int)
    return np.mean(pred == y_true.astype(int))


def mse(y_true, proba):
    """Среднеквадратичная ошибка."""
    return np.mean((y_true - proba) ** 2)


 
# ОСНОВНАЯ ЧАСТЬ
 
if __name__ == "__main__":

    np.random.seed(7)   # вариант 7

    # ---------- ПУНКТ 1: генерация данных ----------
    N = 1000
    mu0    = [0, 2, 3]
    mu1    = [3, 5, 1]
    sigma0 = [2, 1, 2]
    sigma1 = [1, 2, 1]

    mu    = [mu0, mu1]
    sigma = [sigma0, sigma1]

    X, Y, class0, class1 = dg.norm_dataset(mu, sigma, N)
    Y = np.reshape(Y, (2 * N, 1)).astype(float)

    print(f"X: {X.shape}, Y: {Y.shape}")

    # ---------- Разбиение на train/test (для пункта 2 «со звёздочкой») ----------
    n_train = int(0.8 * len(X))          # 80% на обучение
    Xtrain, Xtest = X[:n_train], X[n_train:]
    Ytrain, Ytest = Y[:n_train], Y[n_train:]

    print(f"train: {Xtrain.shape}, test: {Xtest.shape}")


     
    # ПУНКТ 3 сам. работы: кривые потерь и точности
     
    print("\n" + "#" * 60)
    print("# ПУНКТ 3: кривые потерь и точности (n_neuro=4, 50 эпох)")
    print("#" * 60)

    NN = NeuralNetwork(Xtrain, Ytrain, n_neuro=4, random_state=7)

    N_epoch = 50
    losses, accs = [], []

    print(f"\n{'Эпоха':>6} | {'MSE (train)':>12} | {'Acc (train)':>12}")
    print("-" * 38)

    for i in range(N_epoch):
        proba = NN.feedforward()
        loss = mse(Ytrain, proba)
        acc  = accuracy(Ytrain, proba)

        losses.append(loss)
        accs.append(acc)

        print(f"{i:>6} | {loss:>12.5f} | {acc:>12.4f}")
        NN.train(Xtrain, Ytrain)

    # Финальное качество на train и test
    _, proba_tr, acc_tr = NN.test(Xtrain, Ytrain)
    _, proba_te, acc_te = NN.test(Xtest,  Ytest)
    print(f"\nTrain accuracy: {acc_tr:.4f}")
    print(f"Test  accuracy: {acc_te:.4f}")

    # Графики кривых обучения
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))

    axes[0].plot(losses, marker='o', color='navy')
    axes[0].set_title("Функция потерь (MSE) по эпохам")
    axes[0].set_xlabel("Эпоха"); axes[0].set_ylabel("MSE")
    axes[0].grid(alpha=0.3)

    axes[1].plot(accs, marker='s', color='green')
    axes[1].set_title("Точность (train) по эпохам")
    axes[1].set_xlabel("Эпоха"); axes[1].set_ylabel("Accuracy")
    axes[1].grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig("p3_train_curves.png", dpi=120)
    plt.show()


     
    # ПУНКТ 4 сам. работы: подбор числа нейронов и эпох
     
    print("\n" + "#" * 60)
    print("# ПУНКТ 4: подбор n_neuro и N_epoch")
    print("#" * 60)

    # ---- 4.1. Подбор числа нейронов (при фиксированных 50 эпохах) ----
    neuro_range = [1, 2, 3, 4, 5, 8, 12, 16, 24, 32]
    train_auc_neuro = []
    test_auc_neuro  = []
    test_acc_neuro  = []

    print(f"\n{'n_neuro':>8} | {'train_acc':>10} | {'test_acc':>10}")
    print("-" * 34)

    for n in neuro_range:
        nn_n = NeuralNetwork(Xtrain, Ytrain, n_neuro=n, random_state=7)
        for _ in range(N_epoch):
            nn_n.train(Xtrain, Ytrain)

        _, proba_tr, acc_tr = nn_n.test(Xtrain, Ytrain)
        _, proba_te, acc_te = nn_n.test(Xtest,  Ytest)

        train_auc_neuro.append(acc_tr)
        test_auc_neuro.append(acc_te)
        test_acc_neuro.append(acc_te)

        print(f"{n:>8} | {acc_tr:>10.4f} | {acc_te:>10.4f}")

    best_n_neuro = neuro_range[int(np.argmax(test_auc_neuro))]
    print(f"\nЛучшее n_neuro = {best_n_neuro} "
          f"(test_acc = {max(test_auc_neuro):.4f})")

    # График зависимости точности от числа нейронов
    plt.figure(figsize=(9, 5))
    plt.plot(neuro_range, train_auc_neuro, marker='o', label='Train accuracy')
    plt.plot(neuro_range, test_auc_neuro,  marker='s', label='Test accuracy')
    plt.axvline(best_n_neuro, color='red', linestyle='--',
                label=f'Лучшее n_neuro = {best_n_neuro}')
    plt.xlabel('Число нейронов в скрытом слое')
    plt.ylabel('Accuracy')
    plt.title('Зависимость точности от числа нейронов')
    plt.legend(); plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.savefig("p4_n_neuro.png", dpi=120)
    plt.show()

    # ---- 4.2. Подбор числа эпох (при лучшем n_neuro) ----
    epoch_range = [5, 10, 15, 20, 30, 40, 50, 75, 100, 150]
    test_acc_epoch = []

    print(f"\n{'N_epoch':>8} | {'test_acc':>10}")
    print("-" * 22)

    for ne in epoch_range:
        nn_e = NeuralNetwork(Xtrain, Ytrain, n_neuro=best_n_neuro, random_state=7)
        for _ in range(ne):
            nn_e.train(Xtrain, Ytrain)
        _, _, acc_te = nn_e.test(Xtest, Ytest)
        test_acc_epoch.append(acc_te)
        print(f"{ne:>8} | {acc_te:>10.4f}")

    best_epoch = epoch_range[int(np.argmax(test_acc_epoch))]
    print(f"\nЛучшее N_epoch = {best_epoch} "
          f"(test_acc = {max(test_acc_epoch):.4f})")

    # График зависимости точности от числа эпох
    plt.figure(figsize=(9, 5))
    plt.plot(epoch_range, test_acc_epoch, marker='o', color='darkorange')
    plt.axvline(best_epoch, color='red', linestyle='--',
                label=f'Лучшее N_epoch = {best_epoch}')
    plt.xlabel('Число эпох обучения')
    plt.ylabel('Test accuracy')
    plt.title('Зависимость точности от числа эпох')
    plt.legend(); plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.savefig("p4_n_epoch.png", dpi=120)
    plt.show()


     
    # ПУНКТ 5 сам. работы: вынести веса в отдельные переменные
     
    print("\n" + "#" * 60)
    print(f"# ПУНКТ 5: итоговые веса (n_neuro={best_n_neuro}, N_epoch={best_epoch})")
    print("#" * 60)

    # Обучаем финальную сеть с оптимальными гиперпараметрами
    NN_final = NeuralNetwork(Xtrain, Ytrain,
                             n_neuro=best_n_neuro, random_state=7)
    for _ in range(best_epoch):
        NN_final.train(Xtrain, Ytrain)

    # -------- ПУНКТ 5: выносим веса в отдельные переменные --------
    W1 = NN_final.weights1.copy()   # (n_in, n_neuro) — веса входа → скрытого слоя
    W2 = NN_final.weights2.copy()   # (n_neuro, 1)   — веса скрытого → выхода

    print(f"\nweights1 (вход → скрытый): {W1.shape}")
    print(W1)
    print(f"\nweights2 (скрытый → выход): {W2.shape}")
    print(W2)

    # Сохраним в файл для отчёта
    np.savetxt("weights1.txt", W1, fmt="%.6f")
    np.savetxt("weights2.txt", W2, fmt="%.6f")
    print("\nВеса сохранены в weights1.txt и weights2.txt")


     
    # ПУНКТ 2 сам. работы: итоговая точность
     
    _, proba_tr, acc_tr_final = NN_final.test(Xtrain, Ytrain)
    _, proba_te, acc_te_final = NN_final.test(Xtest,  Ytest)

    print("\n" + "=" * 50)
    print("ИТОГОВАЯ ТОЧНОСТЬ (n_neuro = {}, N_epoch = {})".format(
        best_n_neuro, best_epoch))
    print("=" * 50)
    print(f"Train accuracy: {acc_tr_final:.4f}")
    print(f"Test  accuracy: {acc_te_final:.4f}")
    print("=" * 50)


     
    # ДОПОЛНИТЕЛЬНО: кривые финального обучения
     
    NN_plot = NeuralNetwork(Xtrain, Ytrain,
                            n_neuro=best_n_neuro, random_state=7)
    final_losses, final_accs = [], []

    for _ in range(best_epoch):
        proba = NN_plot.feedforward()
        final_losses.append(mse(Ytrain, proba))
        final_accs.append(accuracy(Ytrain, proba))
        NN_plot.train(Xtrain, Ytrain)

    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    axes[0].plot(final_losses, marker='o', color='navy')
    axes[0].set_title(f"MSE, n_neuro={best_n_neuro}, epochs={best_epoch}")
    axes[0].set_xlabel("Эпоха"); axes[0].set_ylabel("MSE")
    axes[0].grid(alpha=0.3)

    axes[1].plot(final_accs, marker='s', color='green')
    axes[1].set_title(f"Accuracy, n_neuro={best_n_neuro}, epochs={best_epoch}")
    axes[1].set_xlabel("Эпоха"); axes[1].set_ylabel("Accuracy")
    axes[1].grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig("p2_final_curves.png", dpi=120)
    plt.show()