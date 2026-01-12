import numpy as np
import matplotlib
import os
import sys
from numpy.linalg import norm

matplotlib.use('Agg')
import matplotlib.pyplot as plt

np.seterr(all='warn')

# --- Функции для метода Нестерова-Немировского ---
def fR(X):
    """Функция Розенброка"""
    x = X[0]
    y = X[1]
    v = (1 - x)**2 + 100*(y - x**2)**2
    return v

def dfR(X):
    """Градиент функции Розенброка"""
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = -2 * (1 - x) + 200 * (y - x**2)*(- 2 * x)
    v[1] = 200 * (y - x**2)
    return v

def H(X, tol, df):
    """Численное вычисление гессиана"""
    n = len(X)
    h = 0.1 * tol
    Hk = np.zeros((n, n))

    for i in range(n):
        for j in range(n):
            ej = np.zeros(n)
            ej[j] = 1.0
            df_plus = df(X + h * ej)[i]
            df_minus = df(X - h * ej)[i]
            Hk[i, j] = (df_plus - df_minus) / (2 * h)

    return Hk

def NNsearch(f, df, x0, tol, plot=True, save_path=None, filename_prefix="NN"):
    """Метод Нестерова-Немировского с возможностью отрисовки"""
    kmax = 1000
    x_k = x0.copy()
    coords = [x_k.copy()]
    neval = 0
    k = 0
    deltaX = np.ones_like(x_k) * np.inf

    while (norm(deltaX) >= tol) and (k < kmax):
        g0 = df(x_k)
        neval += 1

        H0 = H(x_k, tol, df)
        neval += 2 * len(x0)

        v = np.linalg.solve(H0, g0)
        lam = np.sqrt(abs((g0.T @ v).item()))
        
        if lam <= 0.25:
            alpha = 1.0
        else:
            alpha = 1.0 / (1.0 + lam)
            
        dx = -alpha * v
        x_new = x_k + dx
        deltaX = x_new - x_k
        x_k = x_new
        coords.append(x_k.copy())
        k += 1

    xmin = x_k
    fmin = f(xmin)
    answer_ = [xmin, fmin, neval, coords, k]
    
    print(f"xmin = {xmin}, Fmin = {fmin:.6e}")
    print(f"Вычислений функции/градиента: {neval}, Итераций: {k}")

    # Отрисовка траектории
    if plot:
        draw(coords, len(coords)-1, f, save_path=save_path, filename_prefix=filename_prefix)
    
    return answer_

# --- Функции для визуализации (адаптированные) ---
def contourPlot(ax, f, xlim=(-2, 2), ylim=(-2, 2)):
    """Отрисовка контурных линий функции"""
    x1 = np.arange(xlim[0], xlim[1] + 0.1, 0.1)
    m = len(x1)
    y1 = np.arange(ylim[0], ylim[1] + 0.1, 0.1)
    n = len(y1)

    # делаем сетку
    [xx, yy] = np.meshgrid(x1, y1)

    # массивы для графиков функции и ее производных по x и y
    F = np.zeros((n, m))

    # вычисляем рельеф поверхности
    for i in range(n):
        for j in range(m):
            X = [xx[i, j], yy[i, j]]
            F[i, j] = f(X)

    nlevels = 20
    ax.contour(xx, yy, F, nlevels, linewidths=1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')

def bbdDraw(ax, coords, nsteps):
    """Отрисовка траектории оптимизации"""
    fSize = 11

    # Преобразуем координаты к скалярам для безопасности
    x0 = coords[0]
    x0_x = float(x0[0]) if hasattr(x0[0], '__len__') else float(x0[0])
    x0_y = float(x0[1]) if hasattr(x0[1], '__len__') else float(x0[1])
    ax.text(x0_x + 0.1, x0_y + 0.1, str(0), fontsize=fSize)

    for i in range(nsteps - 1):
        x0 = coords[i]
        x1 = coords[i + 1]

        # Преобразуем к скалярам
        x0_x = float(x0[0]) if hasattr(x0[0], '__len__') else float(x0[0])
        x0_y = float(x0[1]) if hasattr(x0[1], '__len__') else float(x0[1])
        x1_x = float(x1[0]) if hasattr(x1[0], '__len__') else float(x1[0])
        x1_y = float(x1[1]) if hasattr(x1[1], '__len__') else float(x1[1])

        ax.plot([x0_x, x1_x], [x0_y, x1_y], lw=1.2, marker='s', ms=3)

    # Последняя точка
    x1_last = coords[-1]
    x1_x = float(x1_last[0]) if hasattr(x1_last[0], '__len__') else float(x1_last[0])
    x1_y = float(x1_last[1]) if hasattr(x1_last[1], '__len__') else float(x1_last[1])

    ax.text(x1_x + 0.2, x1_y, str(nsteps), fontsize=fSize)
    ax.scatter(x1_x, x1_y, marker='o', c='red', zorder=12)

def draw(coords, nsteps, f, save_path=None, filename_prefix="NN"):
    """
    coords - список координат
    nsteps - количество шагов
    f - функция для контурного графика
    save_path - путь для сохранения (если None, сохраняет в текущей директории)
    filename_prefix - префикс для имени файла
    """
    fig, ax = plt.subplots(figsize=(8, 6))
    fig.suptitle('Нестеров-Немировский method trajectory & contour plot')
    plt.xlim(-2, 2)  # Изменено для функции Розенброка
    plt.ylim(-2, 2)
    plt.gca().set_aspect('equal', adjustable='box')
    
    # Точки и шаги
    bbdDraw(ax, coords, nsteps)
    
    # Контур функции
    contourPlot(ax, f)

    # Определяем имя файла
    if filename_prefix:
        name = f"plot_{filename_prefix}.png"
    else:
        name = "plot_NN.png"

    # Определяем полный путь для сохранения
    if save_path:
        # Создаем папку, если она не существует
        os.makedirs(save_path, exist_ok=True)
        full_path = os.path.join(save_path, name)
    else:
        full_path = name

    # Сохраняем
    fig.savefig(full_path, dpi=150)
    print(f"График сохранён: {full_path}")

    # Закрываем фигуру, чтобы освободить память
    plt.close(fig)

# --- Основная программа ---
if __name__ == "__main__":
    # Запуск метода Нестерова-Немировского с отрисовкой
    print("Запуск метода Нестерова-Немировского...")
    result = NNsearch(
        fR, 
        dfR, 
        [-2, -2], 
        1e-4, 
        plot=True, 
        save_path="results",
        filename_prefix="Nesterov_Nemirovsky"
    )
    
    # Вывод подробных результатов
    print("\n" + "="*50)
    print("РЕЗУЛЬТАТЫ ОПТИМИЗАЦИИ (Нестеров-Немировский):")
    print("="*50)
    print(f"Точка минимума: [{result[0][0]:.6f}, {result[0][1]:.6f}]")
    print(f"Значение функции: {result[1]:.6e}")
    print(f"Число вычислений градиента/гессиана: {result[2]}")
    print(f"Число итераций: {result[4]}")
    print(f"Число точек в траектории: {len(result[3])}")
    print("="*50)