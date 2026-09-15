import numpy as np
import matplotlib.pyplot as plt

# если DataGenerator.py в той же папке
from DataGenerator import nonlinear_dataset_7

if __name__ == "__main__":
    import matplotlib.pyplot as plt

    X, Y, c0, c1 = nonlinear_dataset_7(N=1500, seed=42)

    plt.figure(figsize=(6, 6))
    plt.scatter(c0[:, 0], c0[:, 1], s=8, alpha=0.6, label="class0")
    plt.scatter(c1[:, 0], c1[:, 1], s=8, alpha=0.6, label="class1")
    plt.axis("equal")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.title("nonlinear_dataset_7")
    plt.savefig('nonlinear_dataset_7.png', dpi=120)
    plt.show()

    import numpy as np
    import matplotlib.pyplot as plt

    from DataGenerator import nonlinear_dataset_7

    # ---------- 1. Генерация данных ----------
    N = 1000
    X, Y, class0, class1 = nonlinear_dataset_7(N=N, seed=42)

    col = X.shape[1]   # число признаков = 2

    # ---------- 2. Разделение на train / test ----------
    trainCount = round(0.7 * N * 2)
    Xtrain = X[:trainCount]
    Xtest  = X[trainCount:]
    Ytrain = Y[:trainCount]
    Ytest  = Y[trainCount:]

    print(f"Всего объектов: {len(X)}, train: {len(Xtrain)}, test: {len(Xtest)}")
    print(f"Форма class0: {class0.shape}, class1: {class1.shape}")

    # ---------- 3. Гистограммы распределения для всех признаков ----------
    names = ['Признак 1 (ось X)', 'Признак 2 (ось Y)']

    for i in range(col):
        plt.figure(figsize=(7, 4))
        plt.hist(class0[:, i], bins='auto', alpha=0.6,
                label='class0 (верхний-левый угол)',
                density=True, color='blue', edgecolor='black')
        plt.hist(class1[:, i], bins='auto', alpha=0.6,
                label='class1 (нижний-правый угол)',
                density=True, color='hotpink', edgecolor='black')

        # средние значения по признаку — для наглядности
        plt.axvline(class0[:, i].mean(), color='gray', linestyle='--', linewidth=1.5)
        plt.axvline(class1[:, i].mean(), color='orange', linestyle='--', linewidth=1.5)

        plt.title(f'Гистограмма: {names[i]}')
        plt.xlabel(names[i])
        plt.ylabel('Плотность (density)')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.savefig(f'my_hist_{i+1}.png', dpi=120)
        plt.show()

    # ---------- 4. Диаграмма рассеяния: признак 1 vs признак 2 ----------
    plt.figure(figsize=(6, 6))
    plt.scatter(class0[:, 0], class0[:, 1], marker=".", alpha=0.5,
                s=15, label='class0 (верхний-левый угол)', color='blue')
    plt.scatter(class1[:, 0], class1[:, 1], marker=".", alpha=0.5,
                s=15, label='class1 (нижний-правый угол)', color='hotpink')

    plt.title('Диаграмма рассеяния: признак 1 vs признак 2')
    plt.xlabel('Признак 1')
    plt.ylabel('Признак 2')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.axis('equal')      # обязательно — иначе углы будут «скошены»
    plt.tight_layout()
    plt.savefig('my_scatter_1_2.png', dpi=120)
    plt.show()