import numpy as np
import matplotlib
import os
import sys
from numpy.linalg import norm

matplotlib.use('Agg')
import matplotlib.pyplot as plt

# --- Функции для метода Армихо ---
def fH(X):
    """Функция Химмельблау"""
    x = X[0]
    y = X[1]
    v = (x**2 + y - 11)**2 + (x + y**2 - 7)**2
    return v

def dfH(X):
    """Градиент функции Химмельблау"""
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = 2 * (x**2 + y - 11) * (2 * x) + 2 * (x + y**2 - 7)
    v[1] = 2 * (x**2 + y - 11) + 2 * (x + y**2 - 7) * (2 * y)
    return v

def armijosearch(f, df, x0, tol, plot=True, save_path=None, 
                  filename_prefix="Armijo", title=None, 
                  s=1.0, beta=0.5, c1=1e-4, kmax=1000):
    """
    Метод Армихо с поиском длины шага по условию достаточного уменьшения
    
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
    s : float
        Начальное значение шага
    beta : float
        Коэффициент уменьшения шага
    c1 : float
        Параметр условия Армихо (обычно маленькое значение, напр. 1e-4)
    kmax : int
        Максимальное число итераций
    """
    x = np.array(x0, dtype=float).ravel()
    coords = [x.copy()]
    neval = 0
    k = 0
    
    print(f"Запуск метода Армихо с параметрами:")
    print(f"  Начальный шаг s = {s}")
    print(f"  Коэффициент уменьшения beta = {beta}")
    print(f"  Параметр условия c1 = {c1}")
    print(f"  Максимальное число итераций kmax = {kmax}")
    
    while True:
        gk = df(x)
        neval += 1
        pk = -gk  # направление наискорейшего спуска
        
        # Поиск шага по условию Армихо
        alpha = s
        fx = f(x)
        neval += 1
        
        # Поиск подходящего шага
        armijo_iter = 0
        while True:
            x_new = x + alpha * pk
            fx_new = f(x_new)
            neval += 1
            
            # Проверка условия Армихо: f(x + αp) ≤ f(x) + c1·α·∇f(x)^T·p
            armijo_condition = fx_new <= fx + c1 * alpha * np.dot(gk, pk)
            
            if armijo_condition or armijo_iter > 50:  # ограничение на внутренние итерации
                break
                
            alpha = alpha * beta  # уменьшаем шаг
            armijo_iter += 1
        
        # Сохраняем новую точку
        coords.append(x_new.copy())
        
        # Проверка критерия остановки
        if norm(x_new - x) < tol or k >= kmax:
            xmin = x_new
            fmin = f(xmin)
            neval += 1
            break
            
        # Переходим к следующей итерации
        x = x_new
        k += 1
    
    answer_ = [xmin, fmin, neval, coords, k]
    
    print(f"\nРезультаты метода Армихо:")
    print(f"  xmin = {xmin}, Fmin = {fmin:.6e}")
    print(f"  Вычислений функции/градиента: {neval}")
    print(f"  Внешних итераций: {k}")
    print(f"  Длина траектории: {len(coords)} точек")

    # Отрисовка траектории
    if plot:
        # Определяем подходящие пределы для графика
        # Для Химмельблау используем более широкие пределы
        all_x = [p[0] for p in coords]
        all_y = [p[1] for p in coords]
        
        # Автоматически определяем пределы с небольшим запасом
        x_min, x_max = min(all_x), max(all_x)
        y_min, y_max = min(all_y), max(all_y)
        
        # Добавляем запас 20%
        x_margin = max(1.0, (x_max - x_min) * 0.2)
        y_margin = max(1.0, (y_max - y_min) * 0.2)
        
        xlim = (x_min - x_margin, x_max + x_margin)
        ylim = (y_min - y_margin, y_max + y_margin)
        
        # Ограничиваем разумными пределами
        xlim = (max(-6, xlim[0]), min(6, xlim[1]))
        ylim = (max(-6, ylim[0]), min(6, ylim[1]))
            
        # Определяем заголовок
        if title is None:
            title = f"Armijo method (c1={c1}, β={beta}, s={s})"
        
        draw(coords, f, xlim=xlim, ylim=ylim, 
             save_path=save_path, filename_prefix=filename_prefix, 
             title=title, start_point=x0)
    
    return answer_

# --- Функции для визуализации (адаптированные) ---
def contourPlot(ax, f, xlim=(-5, 5), ylim=(-5, 5)):
    """Отрисовка контурных линий функции"""
    x1 = np.arange(xlim[0], xlim[1]+0.1, 0.1)
    y1 = np.arange(ylim[0], ylim[1]+0.1, 0.1)
    xx, yy = np.meshgrid(x1, y1)
    F = np.zeros_like(xx)

    for i in range(len(y1)):
        for j in range(len(x1)):
            F[i, j] = f([xx[i,j], yy[i,j]])

    # Рисуем контуры с подписями
    contours = ax.contour(xx, yy, F, 20, linewidths=1, colors='gray', alpha=0.7)
    ax.clabel(contours, inline=True, fontsize=8, fmt='%1.0f')
    ax.set_xlabel('x')
    ax.set_ylabel('y')

def draw(coords, f, xlim=(-5, 5), ylim=(-5, 5), 
         save_path=None, filename_prefix="Armijo", title=None,
         start_point=None, filename="plot_armijo.png"):
    """
    Отрисовка траектории метода Армихо
    
    Parameters:
    -----------
    coords : list
        Список координат точек
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
    start_point : array-like
        Начальная точка (для подписи)
    filename : str
        Имя файла (устаревший параметр, используется filename_prefix)
    """
    fig, ax = plt.subplots(figsize=(10, 8))
    
    if title:
        fig.suptitle(title, fontsize=14)
    else:
        fig.suptitle("Armijo method trajectory & contour", fontsize=14)
    
    ax.set_aspect('equal', adjustable='box')
    plt.xlim(xlim[0], xlim[1])
    plt.ylim(ylim[0], ylim[1])
    
    # Рисуем линии между точками с разными цветами для лучшей видимости
    colors = plt.cm.rainbow(np.linspace(0, 1, len(coords)-1))
    
    for i in range(len(coords)-1):
        x0 = coords[i]
        x1 = coords[i+1]
        
        # Линия траектории
        ax.plot([x0[0], x1[0]], [x0[1], x1[1]], 
                lw=1.5, marker='o', markersize=5, 
                color=colors[i], alpha=0.7)
        
        # Подпись номера итерации
        mid_x = (x0[0] + x1[0]) / 2
        mid_y = (x0[1] + x1[1]) / 2
        ax.text(mid_x + 0.05, mid_y + 0.05, str(i), 
                fontsize=8, color=colors[i], alpha=0.7,
                bbox=dict(boxstyle="round,pad=0.1", facecolor="white", alpha=0.5))

    # Начальная точка
    x_start = coords[0]
    ax.scatter(x_start[0], x_start[1], marker='s', s=100, 
               color='blue', zorder=10, label='Start')
    ax.text(x_start[0] + 0.1, x_start[1] + 0.1, 'Start', 
            fontsize=10, color='blue', fontweight='bold')

    # Конечная точка
    x_end = coords[-1]
    ax.scatter(x_end[0], x_end[1], marker='*', s=200, 
               color='red', zorder=10, label='Final')
    ax.text(x_end[0] + 0.1, x_end[1] + 0.1, f'Final ({len(coords)-1})', 
            fontsize=10, color='red', fontweight='bold')

    # Контурный график
    contourPlot(ax, f, xlim=xlim, ylim=ylim)
    
    # Легенда
    ax.legend(loc='upper right')
    
    # Добавляем сетку
    ax.grid(True, alpha=0.3, linestyle='--')
    
    # Информация о траектории
    info_text = f"Points: {len(coords)}\nStart: [{coords[0][0]:.2f}, {coords[0][1]:.2f}]\nEnd: [{coords[-1][0]:.2f}, {coords[-1][1]:.2f}]"
    ax.text(0.02, 0.98, info_text, transform=ax.transAxes,
            fontsize=9, verticalalignment='top',
            bbox=dict(boxstyle="round,pad=0.3", facecolor="wheat", alpha=0.8))

    # Определяем имя файла
    if filename_prefix:
        name = f"plot_{filename_prefix}.png"
    else:
        name = "plot_Armijo.png"

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
    return full_path

# --- Основная программа ---
if __name__ == "__main__":
    print("="*60)
    print("ТЕСТИРОВАНИЕ МЕТОДА АРМИДЖО")
    print("="*60)
    
    # Тест 1: Стандартные параметры
    print("\nТест 1: Стандартные параметры (начальная точка [1.5, 1.5])")
    result1 = armijosearch(
        fH, dfH, 
        [1.5, 1.5], 
        1e-4,
        plot=True,
        save_path="armijo_results",
        filename_prefix="Armijo_standard_1.5",
        title="Armijo method: Standard parameters"
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ ТЕСТА 1:")
    print("="*60)
    print(f"Точка минимума: [{result1[0][0]:.6f}, {result1[0][1]:.6f}]")
    print(f"Значение функции: {result1[1]:.6e}")
    print(f"Вычислений: {result1[2]}, Итераций: {result1[4]}")
    
    # Тест 2: Другие параметры
    print("\n\nТест 2: Агрессивные параметры (больший шаг)")
    result2 = armijosearch(
        fH, dfH,
        [1.5, 1.5],
        1e-4,
        plot=True,
        save_path="armijo_results",
        filename_prefix="Armijo_aggressive_1.5",
        title="Armijo method: Aggressive parameters (s=2.0, β=0.8)",
        s=2.0,
        beta=0.8,
        c1=1e-3
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ ТЕСТА 2:")
    print("="*60)
    print(f"Точка минимума: [{result2[0][0]:.6f}, {result2[0][1]:.6f}]")
    print(f"Значение функции: {result2[1]:.6e}")
    print(f"Вычислений: {result2[2]}, Итераций: {result2[4]}")
    
    # Тест 3: Другая начальная точка
    print("\n\nТест 3: Другая начальная точка ([-1, -1])")
    result3 = armijosearch(
        fH, dfH,
        [-1, -1],
        1e-4,
        plot=True,
        save_path="armijo_results",
        filename_prefix="Armijo_standard_-1",
        title="Armijo method: Start point [-1, -1]"
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ ТЕСТА 3:")
    print("="*60)
    print(f"Точка минимума: [{result3[0][0]:.6f}, {result3[0][1]:.6f}]")
    print(f"Значение функции: {result3[1]:.6e}")
    print(f"Вычислений: {result3[2]}, Итераций: {result3[4]}")
    print("="*60)