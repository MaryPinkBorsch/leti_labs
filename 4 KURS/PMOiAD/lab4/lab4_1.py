
import numpy as np
import matplotlib.pyplot as plt

import DataGenerator as dg   # ваш DataGenerator из лабы №1


#  
# ПУНКТ 2: Функции для основных вычислений
#  

def sigmoid(Z):
    """Активационная функция — сигмоида."""
    return 1 / (1 + np.exp(-Z))


def sigmoid_derivative(p):
    """
    Производная сигмоиды, выраженная через её значение p = σ(z):
        σ'(z) = σ(z) · (1 − σ(z)) = p · (1 − p)
    Это удобно: не нужно пересчитывать z.
    """
    return p * (1 - p)


#  
# ПУНКТ 3: Класс нейронной сети
#  

class NeuralNetwork:
    """
    Простейшая нейронная сеть:
        вход (n_in)  →  скрытый слой (n_neuro нейронов, сигмоида)
                     →  выход (1 нейрон, сигмоида)

    Методы:
        __init__     — инициализация весов
        feedforward  — прямой проход
        backprop     — обратное распространение ошибки
        train        — одна эпоха обучения
    """

    def __init__(self, x, y, n_neuro=4):
        self.input = x
        n_in = self.input.shape[1]          # число входов (признаков)

        # ---- инициализация весов случайными значениями [0, 1) ----
        # weights1: (n_in, n_neuro) — веса от входа к скрытому слою
        # weights2: (n_neuro, 1)    — веса от скрытого слоя к выходу
        self.weights1 = np.random.rand(n_in, n_neuro)
        self.weights2 = np.random.rand(n_neuro, 1)

        self.y = y                          # истинные метки
        self.output = np.zeros(y.shape)     # предсказания сети

    def feedforward(self):
        """
        Прямой проход:
        1. Взвешенная сумма + сигмоида → выход скрытого слоя.
        2. Взвешенная сумма + сигмоида → выход сети.
        """
        self.layer1 = sigmoid(np.dot(self.input, self.weights1))
        self.layer2 = sigmoid(np.dot(self.layer1, self.weights2))
        return self.layer2

    def backprop(self):
        """
        Обратное распространение ошибки.
        Вычисляем градиенты по весам и обновляем их.

        Функция потерь: MSE = (y − ŷ)².
        Градиент по выходному слою:
            d_weights2 = layer1.T · [2(y − ŷ) · σ'(ŷ)]
        Градиент по скрытому слою (по правилу цепочки):
            d_weights1 = input.T · [2(y − ŷ) · σ'(ŷ) · weights2.T · σ'(layer1)]
        """
        # --- градиент по weights2 ---
        d_weights2 = np.dot(
            self.layer1.T,
            2 * (self.y - self.output) * sigmoid_derivative(self.output)
        )

        # --- градиент по weights1 ---
        d_weights1 = np.dot(
            self.input.T,
            np.dot(
                2 * (self.y - self.output) * sigmoid_derivative(self.output),
                self.weights2.T
            ) * sigmoid_derivative(self.layer1)
        )

        # --- обновление весов ---
        self.weights1 += d_weights1
        self.weights2 += d_weights2

    def train(self, X, y):
        """
        Одна эпоха обучения:
        1. Прямой проход — вычисляем выход сети.
        2. Обратное распространение — обновляем веса.
        """
        self.output = self.feedforward()
        self.backprop()


#  
# ПУНКТ 4: Генерация данных
#  
if __name__ == "__main__":

    np.random.seed(7)   # воспроизводимость (вариант 7)

    # Параметры выборки: 3 признака, по 1000 объектов на класс
    mu0    = [0, 2, 3]
    mu1    = [3, 5, 1]
    sigma0 = [2, 1, 2]
    sigma1 = [1, 2, 1]

    N = 1000
    col = len(mu0)   # = 3 — число признаков

    mu    = [mu0, mu1]
    sigma = [sigma0, sigma1]

    # Генерируем данные из готового генератора лабы №1
    X, Y, class0, class1 = dg.norm_dataset(mu, sigma, N)

    # Превращаем метки из плоского массива (2N,) в столбец (2N, 1)
    Y = np.reshape(Y, (2 * N, 1)).astype(float)

    print(f"X: {X.shape}, Y: {Y.shape}")
    print(f"Класс 0: {class0.shape}, класс 1: {class1.shape}")


    #  
    # ПУНКТ 5: Инициализация и обучение сети
    #  
    NN = NeuralNetwork(X, Y)

    N_epoch = 50            # число эпох обучения
    losses  = []            # потери по эпохам
    accs    = []            # точность по эпохам

    print(f"\n{'Эпоха':>6} | {'MSE':>10} | {'Accuracy':>10}")
    print("-" * 34)

    for i in range(N_epoch):
        # --- потери до обновления весов (MSE) ---
        pred = NN.feedforward()
        loss = np.mean(np.square(Y - pred))

        # --- точность (порог 0.5) ---
        acc = np.mean((pred >= 0.5).astype(int) == Y.astype(int))

        losses.append(loss)
        accs.append(acc)

        print(f"{i:>6} | {loss:>10.5f} | {acc:>10.4f}")

        # --- один шаг обучения ---
        NN.train(X, Y)


    #  
    # ПУНКТ 6: Графики потерь и точности
    #  
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))

    axes[0].plot(losses, marker='o', color='navy')
    axes[0].set_title("Функция потерь (MSE) по эпохам")
    axes[0].set_xlabel("Эпоха")
    axes[0].set_ylabel("MSE")
    axes[0].grid(alpha=0.3)

    axes[1].plot(accs, marker='s', color='green')
    axes[1].set_title("Точность по эпохам")
    axes[1].set_xlabel("Эпоха")
    axes[1].set_ylabel("Accuracy")
    axes[1].grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig("lab4_train_curves.png", dpi=120)
    plt.show()


    #  
    # ПУНКТ 7: Итоговое предсказание
    #  
    pred_final = NN.feedforward()
    final_acc  = np.mean((pred_final >= 0.5).astype(int) == Y.astype(int))

    print(f"\nИтоговая точность на обучающей выборке: {final_acc:.4f}")
    print(f"Итоговые потери (MSE):                  {losses[-1]:.5f}")

    # -------- итоговые веса (для отчёта) --------
    print(f"\nФорма weights1: {NN.weights1.shape}")
    print(f"Форма weights2: {NN.weights2.shape}")
    print(f"\nweights1 (вход → скрытый слой):\n{NN.weights1}")
    print(f"\nweights2 (скрытый слой → выход):\n{NN.weights2}")