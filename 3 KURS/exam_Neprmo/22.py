import numpy as np
import matplotlib
import os
import sys
from numpy.linalg import norm

matplotlib.use('Agg')
import matplotlib.pyplot as plt

# --- Функции для метода BB и вспомогательные ---
def goldensectionsearch(f, interval, tol):
    """Поиск минимума методом золотого сечения"""
    a, b = interval
    neval = 0

    fi = (1 + np.sqrt(5)) / 2
    L = b - a

    x_left = b - L / fi
    x_right = a + L / fi

    f_right = f(x_right)
    f_left = f(x_left)
    neval += 2

    while (np.abs(L) > tol) and (np.abs(f_left - f_right) > tol):

        if f_left > f_right:
            a = x_left
            x_left, f_left = x_right, f_right
            L = b - a
            x_right = a + L / fi
            f_right = f(x_right)
        else:
            b = x_right
            x_right, f_right = x_left, f_left
            L = b - a
            x_left = b - L / fi
            f_left = f(x_left)
        neval += 1
        
    xmin = (a + b) / 2
    fmin = f(xmin)
    neval += 1

    answer_ = [xmin, fmin, neval]
    return answer_

def fR(X):
    """Функция Розенброка"""
    x = X[0]
    y = X[1]
    v = (1 - x) ** 2 + 100 * (y - x ** 2) ** 2
    return v

def dfR(X):
    """Градиент функции Розенброка"""
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = -2 * (1 - x) + 200 * (y - x ** 2) * (- 2 * x)
    v[1] = 200 * (y - x ** 2)
    return v

def fH(X):
    """Функция Химмельблау"""
    x = X[0]
    y = X[1]
    v = (x ** 2 + y - 11) ** 2 + (x + y ** 2 - 7) ** 2
    return v

def dfH(X):
    """Градиент функции Химмельблау"""
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = 2 * (x ** 2 + y - 11) * (2 * x) + 2 * (x + y ** 2 - 7)
    v[1] = 2 * (x ** 2 + y - 11) + 2 * (x + y ** 2 - 7) * (2 * y)
    return v

def bbsearch(f, df, x0, tol, plot=True, save_path=None, filename_prefix="BB", title=None):
    """
    Метод Barzilai-Borwein (BB) с возможностью отрисовки
    
    Parameters:
    -----------
    f : callable
        Целевая функция
    df : callable
        Градиент функции
    x0 : array-like
        Начальная точка
    tol : float
        Точность
    plot : bool
        Включить отрисовку
    save_path : str
        Путь для сохранения графика
    filename_prefix : str
        Префикс имени файла
    title : str
        Заголовок графика
    """
    kmax = 1000
    D = 0.1

    k, neval = 0, 0
    x_k = np.array(x0, dtype=float)
    coords = [x_k.copy()]

    # Первый шаг с использованием золотого сечения
    grad = df(x_k)
    neval += 1

    phi = lambda alpha: f(x_k - alpha * grad)
    alpha0, fmin, neval_g = goldensectionsearch(phi, [0, 1], tol)
    neval += neval_g

    x_prev = x_k.copy()
    x_k = x_k - alpha0 * grad
    coords.append(x_k.copy())

    deltaX = x_k - x_prev
    
    while norm(deltaX) >= tol and k < kmax:
        g = df(x_k)
        neval += 1

        deltaX = x_k - x_prev
        delta_g = g - grad

        # BB step size (первая формула)
        alph_bb = np.dot(deltaX.T, deltaX) / np.dot(deltaX.T, delta_g)
        
        # Стабилизирующий шаг
        alph_stab = D / norm(g)
        final_alph = min(alph_bb, alph_stab)

        x_prev = x_k.copy()
        x_k = x_k - final_alph * g
        grad = g.copy()

        coords.append(x_k.copy())
        deltaX = x_k - x_prev

        k += 1

    xmin = x_k
    fmin = f(xmin)
    neval += 1

    print(f"xmin = {xmin}, Fmin = {fmin:.6e}")
    print(f"Вычислений функции/градиента: {neval}, Итераций: {k}")

    # Отрисовка траектории
    if plot:
        # Определяем подходящие пределы для графика
        if f.__name__ == 'fH' or 'Химмельблау' in str(f.__name__):
            xlim = (-5, 5)
            ylim = (-5, 5)
        else:
            xlim = (-2, 2)
            ylim = (-2, 2)
            
        # Определяем заголовок
        if title is None:
            if f.__name__ == 'fH':
                title = 'Barzilai-Borwein method: Himmelblau function'
            elif f.__name__ == 'fR':
                title = 'Barzilai-Borwein method: Rosenbrock function'
            else:
                title = 'Barzilai-Borwein method trajectory & contour plot'
        
        draw(coords, len(coords)-1, f, xlim=xlim, ylim=ylim, 
             save_path=save_path, filename_prefix=filename_prefix, 
             title=title)
    
    answer_ = [xmin, fmin, neval, coords, k]
    return answer_

# --- Функции для визуализации (адаптированные) ---
def contourPlot(ax, f, xlim=(-5, 5), ylim=(-5, 5)):
    """Отрисовка контурных линий функции"""
    x1 = np.arange(xlim[0], xlim[1] + 0.1, 0.1)
    m = len(x1)
    y1 = np.arange(ylim[0], ylim[1] + 0.1, 0.1)
    n = len(y1)

    # делаем сетку
    [xx, yy] = np.meshgrid(x1, y1)

    # массивы для графиков функции
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

    if nsteps <= 0:
        return

    # Первая точка
    x0 = coords[0]
    x0_x = float(x0[0]) if hasattr(x0[0], '__len__') else float(x0[0])
    x0_y = float(x0[1]) if hasattr(x0[1], '__len__') else float(x0[1])
    ax.text(x0_x + 0.1, x0_y + 0.1, str(0), fontsize=fSize)

    # Линии между точками
    for i in range(min(nsteps, len(coords)) - 1):
        x0 = coords[i]
        x1 = coords[i + 1]
        
        # Преобразуем к скалярам
        x0_x = float(x0[0]) if hasattr(x0[0], '__len__') else float(x0[0])
        x0_y = float(x0[1]) if hasattr(x0[1], '__len__') else float(x0[1])
        x1_x = float(x1[0]) if hasattr(x1[0], '__len__') else float(x1[0])
        x1_y = float(x1[1]) if hasattr(x1[1], '__len__') else float(x1[1])
        
        ax.plot([x0_x, x1_x], [x0_y, x1_y], lw=1.2, marker='s', ms=3)

    # Последняя точка
    if len(coords) > 1:
        x1_last = coords[-1]
        x1_x = float(x1_last[0]) if hasattr(x1_last[0], '__len__') else float(x1_last[0])
        x1_y = float(x1_last[1]) if hasattr(x1_last[1], '__len__') else float(x1_last[1])
        
        ax.text(x1_x + 0.2, x1_y, str(len(coords)-1), fontsize=fSize)
        ax.scatter(x1_x, x1_y, marker='o', c='red', zorder=12)

def draw(coords, nsteps, f, xlim=(-5, 5), ylim=(-5, 5), 
         save_path=None, filename_prefix="BB", title=None):
    """
    Основная функция отрисовки
    
    Parameters:
    -----------
    coords : list
        Список координат точек
    nsteps : int
        Количество шагов для отрисовки
    f : callable
        Функция для контурного графика
    xlim, ylim : tuple
        Пределы осей
    save_path : str
        Путь для сохранения
    filename_prefix : str
        Префикс имени файла
    title : str
        Заголовок графика
    """
    fig, ax = plt.subplots(figsize=(8, 6))
    
    if title:
        fig.suptitle(title)
    else:
        fig.suptitle('Barzilai-Borwein method trajectory & contour plot')

    plt.xlim(xlim[0], xlim[1])
    plt.ylim(ylim[0], ylim[1])
    ax.set_aspect('equal', adjustable='box')

    # Точки и шаги
    bbdDraw(ax, coords, nsteps)
    
    # Контур функции
    contourPlot(ax, f, xlim=xlim, ylim=ylim)

    # Определяем имя файла
    if filename_prefix:
        name = f"plot_{filename_prefix}.png"
    else:
        name = "plot_BB.png"

    # Определяем полный путь для сохранения
    if save_path:
        # Создаем папку, если она не существует
        os.makedirs(save_path, exist_ok=True)
        full_path = os.path.join(save_path, name)
    else:
        # Используем текущую директорию
        full_path = os.path.join(os.getcwd(), name)

    # Сохраняем
    fig.savefig(full_path, dpi=150)
    plt.close(fig)

    print(f"График сохранён: {full_path}")

# --- Основная программа ---
if __name__ == "__main__":
    print("Тестирование метода Barzilai-Borwein...")
    print("\n1. Функция Химмельблау:")
    result_h = bbsearch(
        fH, 
        dfH, 
        [1, 1], 
        1e-4, 
        plot=True, 
        save_path="bb_results",
        filename_prefix="BB_Himmelblau",
        title="Barzilai-Borwein: Himmelblau function (start [1,1])"
    )
    
    print("\n" + "="*50)
    print("РЕЗУЛЬТАТЫ (Химмельблау):")
    print("="*50)
    print(f"Точка минимума: [{result_h[0][0]:.6f}, {result_h[0][1]:.6f}]")
    print(f"Значение функции: {result_h[1]:.6e}")
    print(f"Число вычислений: {result_h[2]}")
    print(f"Число итераций: {result_h[4]}")
    
    print("\n2. Функция Розенброка:")
    result_r = bbsearch(
        fR, 
        dfR, 
        [-2, -2], 
        1e-4, 
        plot=True, 
        save_path="bb_results",
        filename_prefix="BB_Rosenbrock",
        title="Barzilai-Borwein: Rosenbrock function (start [-2,-2])"
    )
    
    print("\n" + "="*50)
    print("РЕЗУЛЬТАТЫ (Розенброк):")
    print("="*50)
    print(f"Точка минимума: [{result_r[0][0]:.6f}, {result_r[0][1]:.6f}]")
    print(f"Значение функции: {result_r[1]:.6e}")
    print(f"Число вычислений: {result_r[2]}")
    print(f"Число итераций: {result_r[4]}")
    print("="*50)