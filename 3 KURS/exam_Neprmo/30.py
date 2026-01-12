import numpy as np
import matplotlib
import os
import sys
from numpy.linalg import norm

matplotlib.use('Agg')
import matplotlib.pyplot as plt

# --- Функции для метода BFGS ---
def fH(X):
    """Функция Химмельблау"""
    if isinstance(X, np.ndarray) and X.shape == (2, 1):
        x = X[0, 0]
        y = X[1, 0]
    else:
        x = X[0]
        y = X[1]
    v = (x**2 + y - 11)**2 + (x + y**2 - 7)**2
    return v

def dfH(X):
    """Градиент функции Химмельблау"""
    if isinstance(X, np.ndarray) and X.shape == (2, 1):
        x = X[0, 0]
        y = X[1, 0]
    else:
        x = X[0]
        y = X[1]
    
    v = np.array([0.0, 0.0])
    v[0] = 2 * (x**2 + y - 11) * (2 * x) + 2 * (x + y**2 - 7)
    v[1] = 2 * (x**2 + y - 11) + 2 * (x + y**2 - 7) * (2 * y)
    return v

def fR(X):
    """Функция Розенброка"""
    if isinstance(X, np.ndarray) and X.shape == (2, 1):
        x = X[0, 0]
        y = X[1, 0]
    else:
        x = X[0]
        y = X[1]
    return (1 - x)**2 + 100 * (y - x**2)**2

def dfR(X):
    """Градиент функции Розенброка"""
    if isinstance(X, np.ndarray) and X.shape == (2, 1):
        x = X[0, 0]
        y = X[1, 0]
    else:
        x = X[0]
        y = X[1]
    
    v = np.array([0.0, 0.0])
    v[0] = -2*(1 - x) - 400*x*(y - x**2)
    v[1] = 200*(y - x**2)
    return v

def wolfesearch_simple(f, df, xk, pk, amax=1.0, c1=1e-4, c2=0.9, max_ls_iter=20):
    """
    Упрощенный поиск шага по условиям Вольфе
    
    Parameters:
    -----------
    f : callable
        Целевая функция
    df : callable
        Градиент функции
    xk : array
        Текущая точка
    pk : array
        Направление поиска
    amax : float
        Максимальная длина шага
    c1 : float
        Параметр условия Армихо
    c2 : float
        Параметр условия кривизны
    max_ls_iter : int
        Максимальное число итераций линейного поиска
    
    Returns:
    --------
    alpha : float
        Найденная длина шага
    """
    alpha = amax
    fk = f(xk)
    gk = df(xk)
    slope = np.dot(gk, pk)
    
    for ls_iter in range(max_ls_iter):
        x_new = xk + alpha * pk
        f_new = f(x_new)
        g_new = df(x_new)
        
        # Условие Армихо (достаточное убывание)
        armijo = f_new <= fk + c1 * alpha * slope
        
        # Условие кривизны
        curvature_slope = np.dot(g_new, pk)
        curvature = curvature_slope >= c2 * slope
        
        if armijo and curvature:
            return alpha
        
        alpha *= 0.5  # уменьшаем шаг
    
    # Если не нашли подходящий шаг, возвращаем очень маленький
    return 1e-4

def bfgssearch(f, df, x0, tol=1e-6, max_iter=1000, plot=True, save_path=None, 
               filename_prefix="BFGS", title=None, alpha_init=1.0):
    """
    Метод BFGS (Broyden–Fletcher–Goldfarb–Shanno) с возможностью отрисовки.
    
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
    max_iter : int
        Максимальное число итераций
    plot : bool
        Включить отрисовку
    save_path : str
        Путь для сохранения графика
    filename_prefix : str
        Префикс имени файла
    title : str
        Заголовок графика
    alpha_init : float
        Начальная длина шага
    
    Returns:
    --------
    result : dict
        Словарь с результатами оптимизации
    """
    n = len(x0)
    x = np.array(x0, dtype=float)
    
    # Начальная аппроксимация обратной матрицы Гессе
    H = np.eye(n)
    
    # Вычисляем начальный градиент
    grad = df(x)
    grad_norm = norm(grad)
    
    # История для визуализации
    coords = [x.copy()]
    f_values = [f(x)]
    grad_norms = [grad_norm]
    
    # Параметры для условия Вольфе
    c1 = 1e-4
    c2 = 0.9
    
    print(f"Запуск метода BFGS с параметрами:")
    print(f"  Начальная точка: [{x[0]:.2f}, {x[1]:.2f}]")
    print(f"  Точность: {tol}")
    print(f"  Максимальное число итераций: {max_iter}")
    print(f"  Начальная длина шага: {alpha_init}")
    
    for k in range(max_iter):
        # Направление поиска
        p = -H @ grad
        
        # Линейный поиск с использованием условия Вольфе
        alpha = wolfesearch_simple(f, df, x, p, amax=alpha_init, c1=c1, c2=c2)
        
        # Сохраняем старые значения
        x_old = x.copy()
        grad_old = grad.copy()
        
        # Делаем шаг
        x = x + alpha * p
        grad = df(x)
        grad_norm = norm(grad)
        
        # Разности для обновления BFGS
        s = x - x_old
        y = grad - grad_old
        
        # Проверка условия обновления (избегаем численных проблем)
        s_dot_y = np.dot(s, y)
        
        if s_dot_y > 1e-10:
            rho = 1.0 / s_dot_y
            
            # Обновление BFGS для обратной матрицы Гессе
            I = np.eye(n)
            Hy = H @ y
            
            # Формула BFGS для обновления H (обратной матрицы Гессе)
            # H_{k+1} = (I - ρ s y^T) H_k (I - ρ y s^T) + ρ s s^T
            term1 = I - rho * np.outer(s, y)
            term2 = I - rho * np.outer(y, s)
            H = term1 @ H @ term2 + rho * np.outer(s, s)
        
        # Сохраняем историю
        coords.append(x.copy())
        f_values.append(f(x))
        grad_norms.append(grad_norm)
        
        # Вывод прогресса каждые 10 итераций
        if k % 10 == 0:
            f_val = float(f(x))
            print(f"  Итерация {k}: ||∇f|| = {grad_norm:.2e}, f(x) = {f_val:.2e}")
        
        # Критерий остановки
        if grad_norm < tol:
            print(f"\nСходимость достигнута на итерации {k+1}")
            break
    
    if k == max_iter - 1:
        print(f"\nДостигнуто максимальное количество итераций ({max_iter})")
    
    # Финальные результаты
    xmin = x
    fmin = float(f(xmin))
    
    result = {
        'x_min': xmin,
        'f_min': fmin,
        'success': grad_norm < tol,
        'iterations': k + 1,
        'grad_norm': grad_norm,
        'neval': (k + 1) * 2,  # Примерная оценка вычислений
        'coords': coords,
        'f_values': f_values,
        'grad_norms': grad_norms
    }
    
    print(f"\nРезультаты метода BFGS:")
    print(f"  x_min = [{xmin[0]:.6f}, {xmin[1]:.6f}], f_min = {fmin:.6e}")
    print(f"  Итераций: {result['iterations']}")
    print(f"  Норма градиента: {grad_norm:.2e}")
    print(f"  Успех: {result['success']}")
    print(f"  Число точек в траектории: {len(coords)}")

    # Отрисовка траектории
    if plot:
        # Определяем подходящие пределы для графика
        if f.__name__ == 'fH' or f is fH:
            xlim = (-5, 5)
            ylim = (-5, 5)
        elif f.__name__ == 'fR' or f is fR:
            xlim = (-2, 2)
            ylim = (-1, 3)
        else:
            # Автоматическое определение пределов
            all_x = [p[0] for p in coords]
            all_y = [p[1] for p in coords]
            x_margin = max(1.0, (max(all_x) - min(all_x)) * 0.2)
            y_margin = max(1.0, (max(all_y) - min(all_y)) * 0.2)
            xlim = (min(all_x) - x_margin, max(all_x) + x_margin)
            ylim = (min(all_y) - y_margin, max(all_y) + y_margin)
            
        # Определяем заголовок
        if title is None:
            func_name = "Himmelblau" if (f.__name__ == 'fH' or f is fH) else "Rosenbrock"
            title = f'BFGS method: {func_name} function'
        
        draw(coords, len(coords)-1, f, xlim=xlim, ylim=ylim, 
             save_path=save_path, filename_prefix=filename_prefix, 
             title=title, f_values=f_values)
    
    return result

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

    nlevels = 25
    contours = ax.contour(xx, yy, F, nlevels, linewidths=1, colors='gray', alpha=0.6)
    ax.clabel(contours, inline=True, fontsize=8)
    ax.set_xlabel('x')
    ax.set_ylabel('y')

def bfgsDraw(ax, coords, nsteps):
    """Отрисовка траектории метода BFGS"""
    fSize = 11
    
    if len(coords) == 0:
        return
    
    # Преобразуем все координаты к плоскому виду
    flat_coords = []
    for point in coords:
        if hasattr(point, 'flatten'):
            flat_point = point.flatten()
        elif isinstance(point, list) or isinstance(point, tuple):
            flat_point = np.array(point).flatten()
        else:
            flat_point = point
        flat_coords.append(flat_point)
    
    # Первая точка
    x0 = flat_coords[0]
    ax.text(float(x0[0]) + 0.03, float(x0[1]) + 0.1, "0", 
            fontsize=fSize, fontweight='bold', color='blue')
    ax.scatter(float(x0[0]), float(x0[1]), marker='s', s=80, 
               color='blue', zorder=10, label='Start')
    
    # Линии между точками
    colors = plt.cm.plasma(np.linspace(0, 1, min(nsteps, len(flat_coords))))
    
    for i in range(min(nsteps, len(flat_coords)) - 1):
        x0 = flat_coords[i]
        x1 = flat_coords[i + 1]
        
        ax.plot([float(x0[0]), float(x1[0])],
                [float(x0[1]), float(x1[1])],
                lw=1.5, marker='o', ms=4, color=colors[i], alpha=0.8)
        
        # Подпись номера итерации (каждые 5 итераций или последние 3)
        if i % 5 == 0 or i >= len(flat_coords) - 4:
            mid_x = (float(x0[0]) + float(x1[0])) / 2
            mid_y = (float(x0[1]) + float(x1[1])) / 2
            ax.text(mid_x + 0.05, mid_y + 0.05, str(i+1), 
                    fontsize=8, color=colors[i], alpha=0.8,
                    bbox=dict(boxstyle="round,pad=0.2", facecolor="white", alpha=0.7))

    # Последняя точка
    x_last = flat_coords[-1]
    ax.scatter(float(x_last[0]), float(x_last[1]), marker='*', s=150, 
               color='red', zorder=12, label='Final')
    ax.text(float(x_last[0]) + 0.1, float(x_last[1]) + 0.1, 
            f'{len(flat_coords)-1}', fontsize=fSize, 
            fontweight='bold', color='red')

def draw(coords, nsteps, f, xlim=(-5, 5), ylim=(-5, 5), 
         save_path=None, filename_prefix="BFGS", title=None, f_values=None):
    """
    Основная функция отрисовки для метода BFGS
    
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
    f_values : list
        Значения функции в точках траектории
    """
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))
    
    if title:
        fig.suptitle(title, fontsize=14)
    else:
        fig.suptitle('BFGS (Broyden–Fletcher–Goldfarb–Shanno) method', fontsize=14)

    # --- Первый график: траектория и контуры ---
    ax1.set_xlim(xlim[0], xlim[1])
    ax1.set_ylim(ylim[0], ylim[1])
    ax1.set_aspect('equal', adjustable='box')
    
    # Контур функции
    contourPlot(ax1, f, xlim=xlim, ylim=ylim)
    
    # Точки и шаги
    bfgsDraw(ax1, coords, nsteps)
    
    # Легенда
    ax1.legend(loc='upper right')
    ax1.grid(True, alpha=0.2, linestyle='--')
    ax1.set_title('Trajectory & Contour Plot')
    
    # Информационная панель
    if len(coords) > 0:
        first_point = coords[0].flatten() if hasattr(coords[0], 'flatten') else coords[0]
        last_point = coords[-1].flatten() if hasattr(coords[-1], 'flatten') else coords[-1]
        
        info_text = f"Points: {len(coords)}\nStart: [{float(first_point[0]):.3f}, {float(first_point[1]):.3f}]\nEnd: [{float(last_point[0]):.3f}, {float(last_point[1]):.3f}]"
        ax1.text(0.02, 0.98, info_text, transform=ax1.transAxes,
                fontsize=9, verticalalignment='top',
                bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue", alpha=0.8))

    # --- Второй график: сходимость функции ---
    if f_values is not None and len(f_values) > 1:
        iterations = list(range(len(f_values)))
        ax2.semilogy(iterations, f_values, 'b-', linewidth=2, marker='o', markersize=4)
        ax2.set_xlabel('Iteration')
        ax2.set_ylabel('Function value (log scale)')
        ax2.set_title('Function Value Convergence')
        ax2.grid(True, alpha=0.3)
        ax2.set_xlim(0, len(f_values)-1)
        
        # Добавляем горизонтальную линию для минимального значения
        min_f = min(f_values)
        ax2.axhline(y=min_f, color='r', linestyle='--', alpha=0.5, label=f'Min: {min_f:.2e}')
        ax2.legend()
    
    plt.tight_layout()
    
    # Определяем имя файла
    if filename_prefix:
        name = f"plot_{filename_prefix}.png"
    else:
        name = "plot_BFGS.png"

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
    print("="*70)
    print("ТЕСТИРОВАНИЕ МЕТОДА BFGS")
    print("="*70)
    
    # Тест 1: Функция Химмельблау
    print("\nТест 1: Функция Химмельблау (начальная точка [1.5, 1.5])")
    result1 = bfgssearch(
        fH, dfH, 
        [1.5, 1.5], 
        tol=1e-6,
        max_iter=200,
        plot=True,
        save_path="bfgs_results",
        filename_prefix="BFGS_Himmelblau_1.5",
        title="BFGS method: Himmelblau function",
        alpha_init=1.0
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ ТЕСТА 1 (Химмельблау):")
    print("="*60)
    print(f"Точка минимума: [{result1['x_min'][0]:.6f}, {result1['x_min'][1]:.6f}]")
    print(f"Значение функции: {result1['f_min']:.6e}")
    print(f"Итераций: {result1['iterations']}")
    print(f"Норма градиента: {result1['grad_norm']:.2e}")
    
    # Тест 2: Функция Розенброка
    print("\n\nТест 2: Функция Розенброка (начальная точка [-1.2, 1.0])")
    result2 = bfgssearch(
        fR, dfR,
        [-1.2, 1.0],
        tol=1e-6,
        max_iter=500,
        plot=True,
        save_path="bfgs_results",
        filename_prefix="BFGS_Rosenbrock_-1.2_1.0",
        title="BFGS method: Rosenbrock function",
        alpha_init=1.0
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ ТЕСТА 2 (Розенброк):")
    print("="*60)
    print(f"Точка минимума: [{result2['x_min'][0]:.6f}, {result2['x_min'][1]:.6f}]")
    print(f"Значение функции: {result2['f_min']:.6e}")
    print(f"Итераций: {result2['iterations']}")
    print(f"Норма градиента: {result2['grad_norm']:.2e}")
    
    # Тест 3: Функция Химмельблау с другой начальной точкой
    print("\n\nТест 3: Функция Химмельблау (начальная точка [-2, 2])")
    result3 = bfgssearch(
        fH, dfH,
        [-2, 2],
        tol=1e-6,
        max_iter=200,
        plot=True,
        save_path="bfgs_results",
        filename_prefix="BFGS_Himmelblau_-2_2",
        title="BFGS method: Himmelblau function (start [-2, 2])",
        alpha_init=1.0
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ ТЕСТА 3 (Химмельблау другая точка):")
    print("="*60)
    print(f"Точка минимума: [{result3['x_min'][0]:.6f}, {result3['x_min'][1]:.6f}]")
    print(f"Значение функции: {result3['f_min']:.6e}")
    print(f"Итераций: {result3['iterations']}")
    print(f"Норма градиента: {result3['grad_norm']:.2e}")
    print("="*70)