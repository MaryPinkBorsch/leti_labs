import numpy as np
import matplotlib
import os
import sys
from numpy.linalg import norm

matplotlib.use('Agg')
import matplotlib.pyplot as plt

# --- Функции для метода Полка-Рибьера ---
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

def wolfesearch(f, df, xk, pk, amax, c1, c2):
    """
    Поиск длины шага по Вольфе для нелинейной оптимизации.
    """
    
    # Инициализация
    alpha = amax
    alpha_low = 0.0
    alpha_high = amax
    
    # Вычисление значений в начальной точке
    fk = f(xk)
    gk = df(xk)
    
    # Производная по направлению
    slope = np.dot(gk.flatten(), pk.flatten())
    
    # Максимальное количество итераций
    max_iter = 50
    
    for i in range(max_iter):
        # Вычисление значения функции и градиента в новой точке
        x_new = xk + alpha * pk
        f_new = f(x_new)
        g_new = df(x_new)
        
        # Проверка условия Армихо (достаточного уменьшения)
        armijo = f_new <= fk + c1 * alpha * slope
        
        # Проверка условия кривизны
        curvature_slope = np.dot(g_new.flatten(), pk.flatten())
        curvature = curvature_slope >= c2 * slope
        
        if not armijo:
            # Условие Армихо не выполнено - уменьшаем шаг
            alpha_high = alpha
            alpha = 0.5 * (alpha_low + alpha_high)
        elif not curvature:
            # Условие кривизны не выполнено - увеличиваем нижнюю границу
            alpha_low = alpha
            if alpha_high >= amax:
                alpha = 2.0 * alpha
            else:
                alpha = 0.5 * (alpha_low + alpha_high)
        else:
            # Оба условия выполнены
            return alpha
            
        # Ограничение alpha сверху
        alpha = min(alpha, amax)
    
    # Если не удалось найти подходящий шаг, возвращаем последний alpha
    return alpha

def prsearch(f, df, x0, tol, plot=True, save_path=None, filename_prefix="PR", title=None):
    """
    Метод Полка-Рибьера (Polak-Ribiere) с возможностью отрисовки
    
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
    c1 = tol
    c2 = 0.1
    amax = 3

    xk = np.array(x0, dtype=float)
    gk = df(xk)
    neval = 1
    pk = -gk

    coords = [xk.copy()]
    k = 0

    while (norm(gk) >= tol) and (k < kmax):
        alpha = wolfesearch(f, df, xk, pk, amax, c1, c2)

        xk1 = xk + alpha * pk
        gk1 = df(xk1)
        neval += 1
        
        # Вычисление beta по формуле Полка-Рибьера
        # beta = (gk1^T * (gk1 - gk)) / (gk^T * gk)
        g_diff = gk1 - gk
        numerator = np.dot(gk1.flatten(), g_diff.flatten())
        denominator = np.dot(gk.flatten(), gk.flatten())
        beta = max(0, numerator / denominator)  # Обычная модификация для обеспечения сходимости

        # Новое направление
        pk = -gk1 + beta * pk

        # Обновление переменных
        xk = xk1
        gk = gk1
        coords.append(xk.copy())
        k += 1

    xmin = xk
    fmin = f(xmin)
    
    answer_ = [xmin, fmin, neval, coords, k]
    
    print(f"xmin = {xmin}, Fmin = {fmin:.6e}")
    print(f"Вычислений градиента: {neval}, Итераций: {k}")

    # Отрисовка траектории
    if plot:
        # Определяем подходящие пределы для графика
        xlim = (-5, 5)
        ylim = (-5, 5)
            
        # Определяем заголовок
        if title is None:
            title = 'Polak-Ribiere conjugate gradient method'
        
        draw(coords, len(coords)-1, f, xlim=xlim, ylim=ylim, 
             save_path=save_path, filename_prefix=filename_prefix, 
             title=title)
    
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
    contours = ax.contour(xx, yy, F, nlevels, linewidths=1)
    ax.clabel(contours, inline=True, fontsize=8)
    ax.set_xlabel('x')
    ax.set_ylabel('y')

def prDraw(ax, coords, nsteps):
    """Отрисовка траектории метода Полка-Рибьера"""
    fSize = 11
    
    if len(coords) == 0:
        return
        
    # первая точка
    x0 = coords[0]
    x0x = float(x0[0]) if hasattr(x0[0], '__len__') else float(x0[0])
    x0y = float(x0[1]) if hasattr(x0[1], '__len__') else float(x0[1])
    ax.text(x0x + 0.03, x0y + 0.1, "0", fontsize=fSize)
    ax.scatter(x0x, x0y, marker='o', c='blue', zorder=10)

    x1x, x1y = x0x, x0y
    
    for i in range(min(nsteps, len(coords)) - 1):
        x0 = coords[i]
        x1 = coords[i + 1]

        x0x = float(x0[0]) if hasattr(x0[0], '__len__') else float(x0[0])
        x0y = float(x0[1]) if hasattr(x0[1], '__len__') else float(x0[1])
        x1x = float(x1[0]) if hasattr(x1[0], '__len__') else float(x1[0])
        x1y = float(x1[1]) if hasattr(x1[1], '__len__') else float(x1[1])

        ax.plot([x0x, x1x], [x0y, x1y], lw=1.2, marker='s', ms=3)
        ax.text(x0x + 0.05, x0y - 0.15, str(i), fontsize=8, alpha=0.7)

    # последняя точка
    if len(coords) > 1:
        ax.scatter(x1x, x1y, marker='o', c='red', s=50, zorder=12)
        ax.text(x1x + 0.1, x1y + 0.1, str(len(coords)-1), fontsize=fSize, 
                bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow", alpha=0.7))

def draw(coords, nsteps, f, xlim=(-5, 5), ylim=(-5, 5), 
         save_path=None, filename_prefix="PR", title=None):
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
    fig, ax = plt.subplots(figsize=(10, 8))
    
    if title:
        fig.suptitle(title, fontsize=14)
    else:
        fig.suptitle('Polak-Ribiere conjugate gradient method', fontsize=14)

    plt.xlim(xlim[0], xlim[1])
    plt.ylim(ylim[0], ylim[1])
    ax.set_aspect('equal', adjustable='box')
    
    # Контур функции
    contourPlot(ax, f, xlim=xlim, ylim=ylim)
    
    # Точки и шаги
    prDraw(ax, coords, nsteps)
    
    # Добавляем легенду
    from matplotlib.patches import Patch
    legend_elements = [
        Patch(facecolor='blue', edgecolor='black', label='Start point'),
        Patch(facecolor='red', edgecolor='black', label='End point'),
        Patch(facecolor='none', edgecolor='black', linestyle='-', label='Trajectory')
    ]
    ax.legend(handles=legend_elements, loc='upper right')

    # Определяем имя файла
    if filename_prefix:
        name = f"plot_{filename_prefix}.png"
    else:
        name = "plot_Polak_Ribiere.png"

    # Определяем полный путь для сохранения
    if save_path:
        # Создаем папку, если она не существует
        os.makedirs(save_path, exist_ok=True)
        full_path = os.path.join(save_path, name)
    else:
        # Используем текущую директорию
        full_path = os.path.join(os.getcwd(), name)

    # Сохраняем
    fig.savefig(full_path, dpi=150, bbox_inches='tight')
    plt.close(fig)

    print(f"График сохранён: {full_path}")
    
    # Для встраивания в HTML (опционально)
    # ad = f'<img width="900px" src="{full_path}">'
    # print(ad)

# --- Основная программа ---
if __name__ == "__main__":
    print("Тестирование метода Полка-Рибьера...")
    print("\n1. Функция Химмельблау (начальная точка [1.5, 1.5]):")
    
    result = prsearch(
        fH, 
        dfH, 
        [1.5, 1.5], 
        1e-4, 
        plot=True, 
        save_path="pr_results",
        filename_prefix="PR_Himmelblau_1.5",
        title="Polak-Ribiere: Himmelblau function (start [1.5, 1.5])"
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ МЕТОДА ПОЛКА-РИБЬЕРА:")
    print("="*60)
    print(f"Точка минимума: [{result[0][0]:.6f}, {result[0][1]:.6f}]")
    print(f"Значение функции: {result[1]:.6e}")
    print(f"Число вычислений градиента: {result[2]}")
    print(f"Число итераций: {result[4]}")
    print(f"Число точек в траектории: {len(result[3])}")
    print("="*60)
    
    # Тестирование с другой начальной точкой
    print("\n2. Функция Химмельблау (начальная точка [-1, -1]):")
    
    result2 = prsearch(
        fH, 
        dfH, 
        [-1, -1], 
        1e-4, 
        plot=True, 
        save_path="pr_results",
        filename_prefix="PR_Himmelblau_-1",
        title="Polak-Ribiere: Himmelblau function (start [-1, -1])"
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ (вторая точка):")
    print("="*60)
    print(f"Точка минимума: [{result2[0][0]:.6f}, {result2[0][1]:.6f}]")
    print(f"Значение функции: {result2[1]:.6e}")
    print(f"Число вычислений градиента: {result2[2]}")
    print(f"Число итераций: {result2[4]}")
    print("="*60)