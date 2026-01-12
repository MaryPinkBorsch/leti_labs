import numpy as np
import matplotlib
import sys
from numpy.linalg import norm

matplotlib.use('Agg')
import matplotlib.pyplot as plt

# --- Функции для метода Левенберга-Марквардта ---
np.seterr(all='warn')

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

def lmsearch(f, df, x0, tol, plot=True, filename="plot_LM.png"):
    """Метод Левенберга-Марквардта с возможностью отрисовки"""
    x = np.array(x0).ravel()
    coords = [x.copy()]
    neval = 0
    alpha = 100    
    v = 3  
    k = 0
    kmax = 1000

    while True:
        gk = df(x)
        neval += 1
        Hk = H(x, tol, df)
        n = len(x)
        neval += 2 * n * n
        A = Hk + alpha * np.eye(n)
        step = np.linalg.solve(A, gk)
        x_new = x - step
        
        if f(x_new) < f(x):
            alpha = alpha / v
            x = x_new
            coords.append(x.copy())
        else:
            alpha = alpha * v
            
        if norm(step) < tol or k >= kmax:
            xmin = x
            fmin = f(xmin)
            break
        k += 1
    
    answer_ = [xmin, fmin, neval, coords]
    print(f"xmin = {xmin}, Fmin = {fmin}, evaluations = {neval}, iterations = {k}")
    
    # Отрисовка траектории
    if plot:
        draw(coords, f, filename=filename)
    
    return answer_

# --- Функции для визуализации (ваш исходный код) ---
def contourPlot(ax, f, xlim=(-2, 2), ylim=(-2, 2)):
    """Отрисовка контурных линий функции"""
    x1 = np.arange(xlim[0], xlim[1] + 0.1, 0.1)
    y1 = np.arange(ylim[0], ylim[1] + 0.1, 0.1)
    xx, yy = np.meshgrid(x1, y1)
    F = np.zeros_like(xx)

    for i in range(len(y1)):
        for j in range(len(x1)):
            F[i, j] = f([xx[i, j], yy[i, j]])

    ax.contour(xx, yy, F, 20, linewidths=1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')

def bbdDraw(ax, coords, nsteps):
    """Отрисовка траектории оптимизации"""
    fSize = 11

    for i in range(nsteps - 1):
        x0 = coords[i]
        x1 = coords[i + 1]
        ax.plot([x0[0], x1[0]], [x0[1], x1[1]], lw=1.2, marker='s', ms=3)

    last_point = coords[nsteps - 1]
    ax.text(last_point[0] + 0.1, last_point[1] + 0.1, str(nsteps - 1), fontsize=fSize)

    plt.scatter(last_point[0], last_point[1], marker='o', c='red', zorder=12)

def draw(coords, f, filename="plot_LM.png", max_iter=None):
    """Основная функция отрисовки"""
    nsteps = len(coords) if max_iter is None else min(len(coords), max_iter)
    fig, ax = plt.subplots(figsize=(8, 6))
    fig.suptitle('Levenberg-Marquardt method trajectory & contour')
    plt.xlim(-2, 2)  # Изменил пределы для функции Розенброка
    plt.ylim(-2, 2)
    ax.set_aspect('equal', adjustable='box')

    # Точки и шаги
    bbdDraw(ax, coords, nsteps)

    # Контур функции
    contourPlot(ax, f)

    fig.tight_layout()
    fig.savefig(filename, dpi=150)
    print(f"График сохранен: {filename}")
    plt.close(fig)

# --- Основная программа ---
if __name__ == "__main__":
    # Запуск метода Левенберга-Марквардта с отрисовкой
    result = lmsearch(fR, dfR, [-2, -2], 1e-4, plot=True, filename="rosenbrock_lm.png")
    
    # Вывод результатов
    print("\nРезультаты оптимизации:")
    print(f"Минимум в точке: {result[0]}")
    print(f"Значение функции: {result[1]:.6e}")
    print(f"Число вычислений: {result[2]}")
    print(f"Число итераций: {len(result[3]) - 1}")