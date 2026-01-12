import numpy as np
import matplotlib
import os
import sys
from numpy.linalg import norm

matplotlib.use('Agg')
import matplotlib.pyplot as plt

# --- Функции для метода DFP ---
def fH(X):
    """Функция Химмельблау"""
    # Преобразуем X к правильному формату если нужно
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
    # Преобразуем X к правильному формату если нужно
    if isinstance(X, np.ndarray) and X.shape == (2, 1):
        x = X[0, 0]
        y = X[1, 0]
    else:
        x = X[0]
        y = X[1]
    
    v = np.array([0.0, 0.0])
    v[0] = 2 * (x**2 + y - 11) * (2 * x) + 2 * (x + y**2 - 7)
    v[1] = 2 * (x**2 + y - 11) + 2 * (x + y**2 - 7) * (2 * y)
    return v.reshape(-1, 1)  # Возвращаем как столбец 2x1

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

def Hnew(dk, yk, H):
    """
    Обновление матрицы H по формуле DFP (Davidon-Fletcher-Powell).
    
    Параметры:
    dk - вектор изменения переменных: dk = x_{k+1} - x_k
    yk - вектор изменения градиентов: yk = g_{k+1} - g_k
    H - текущая аппроксимация обратной матрицы Гессе
    
    Возвращает:
    H_new - обновленная матрица
    """
    # Преобразуем входные данные в массивы NumPy
    dk = np.asarray(dk, dtype=float).reshape(-1, 1)  # столбец
    yk = np.asarray(yk, dtype=float).reshape(-1, 1)  # столбец
    H = np.asarray(H, dtype=float)
    
    # Вычисляем скалярные произведения
    dk_T_yk = np.dot(dk.T, yk)[0, 0]  # скаляр
    yk_T_H_yk = np.dot(yk.T, np.dot(H, yk))[0, 0]  # скаляр
    
    # Проверяем, что dk_T_yk не слишком мал (для численной устойчивости)
    if abs(dk_T_yk) < 1e-15:
        return H  # возвращаем старую матрицу если знаменатель слишком мал
    
    # Вычисляем слагаемые для обновления
    term1 = np.dot(dk, dk.T) / dk_T_yk
    
    # Второе слагаемое
    H_yk = np.dot(H, yk)
    term2 = np.dot(H_yk, yk.T)
    term2 = np.dot(term2, H)
    if abs(yk_T_H_yk) > 1e-15:
        term2 = term2 / yk_T_H_yk
    else:
        term2 = np.zeros_like(H)
    
    # Обновляем матрицу H по формуле DFP
    H_new = H + term1 - term2
    
    # Обеспечиваем симметричность (из-за ошибок округления может потеряться)
    H_new = (H_new + H_new.T) / 2
    
    return H_new

def dfpsearch(f, df, x0, tol, plot=True, save_path=None, 
              filename_prefix="DFP", title=None,
              c1=None, c2=0.1, amax=3, kmax=1000):
    """
    Метод Дэвидона-Флетчера-Пауэла (DFP) с возможностью отрисовки.
    
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
    c1 : float
        Параметр условия Армихо (если None, используется tol)
    c2 : float
        Параметр условия кривизны Вольфе
    amax : float
        Максимальная длина шага
    kmax : int
        Максимальное число итераций
    """
    if c1 is None:
        c1 = tol
    
    # Инициализация
    x0 = np.array(x0, dtype=float).reshape(-1, 1)
    coords = [x0.copy()]
    k = 0
    neval = 0
    
    # Начальный градиент и матрица H
    g0 = df(x0)
    neval += 1
    gnew = g0.copy()
    
    # Начальная матрица H (единичная)
    n = len(x0)
    H = np.eye(n)
    
    print(f"Запуск метода DFP с параметрами:")
    print(f"  Начальная точка: [{x0[0,0]:.2f}, {x0[1,0]:.2f}]")
    print(f"  Точность: {tol}")
    print(f"  c1 = {c1}, c2 = {c2}, amax = {amax}")
    print(f"  Максимальное число итераций: {kmax}")
    
    # Основной цикл
    dk_norm = float('inf')
    
    while dk_norm >= tol and k < kmax:
        # Направление поиска
        p0 = np.dot(-H, gnew)
        
        # Поиск шага по Вольфе
        alpha = wolfesearch(f, df, x0, p0, amax, c1, c2)
        
        # Новое приближение
        x1 = x0 + alpha * p0
        dk = alpha * p0
        dk_norm = norm(dk)
        
        # Новый градиент
        gnew = df(x1)
        neval += 1
        
        # Разность градиентов
        yk = gnew - g0
        
        # Обновление матрицы H по формуле DFP
        H = Hnew(dk, yk, H)
        
        # Обновление переменных для следующей итерации
        g0 = gnew.copy()
        x0 = x1.copy()
        
        # Сохранение траектории
        coords.append(x0.copy())
        k += 1
        
        # Вывод прогресса каждые 10 итераций (ИСПРАВЛЕНА ЭТА СТРОКА)
        if k % 10 == 0:
            # Получаем значение функции как число с плавающей точкой
            f_val = float(f(x0))
            print(f"  Итерация {k}: ||dk|| = {dk_norm:.2e}, f(x) = {f_val:.2e}")
    
    # Финальные результаты
    xmin = x0
    fmin = float(f(xmin))  # Преобразуем к float для корректного форматирования
    neval += 1
    
    answer_ = [xmin, fmin, neval, coords, k, H]
    
    print(f"\nРезультаты метода DFP:")
    print(f"  xmin = [{xmin[0,0]:.6f}, {xmin[1,0]:.6f}], Fmin = {fmin:.6e}")
    print(f"  Вычислений градиента: {neval}")
    print(f"  Итераций: {k}")
    print(f"  Норма последнего шага: {dk_norm:.2e}")
    print(f"  Число точек в траектории: {len(coords)}")
    print(f"  Определитель финальной матрицы H: {np.linalg.det(H):.2e}")

    # Отрисовка траектории
    if plot:
        # Определяем подходящие пределы для графика
        # Для Химмельблау используем стандартные пределы
        xlim = (-5, 5)
        ylim = (-5, 5)
            
        # Определяем заголовок
        if title is None:
            title = f'DFP (Davidon-Fletcher-Powell) method'
        
        draw(coords, len(coords)-1, fH, xlim=xlim, ylim=ylim, 
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

    nlevels = 25
    contours = ax.contour(xx, yy, F, nlevels, linewidths=1, colors='gray', alpha=0.6)
    ax.clabel(contours, inline=True, fontsize=8)
    ax.set_xlabel('x')
    ax.set_ylabel('y')

def dfpDraw(ax, coords, nsteps):
    """Отрисовка траектории метода DFP"""
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
    colors = plt.cm.viridis(np.linspace(0, 1, min(nsteps, len(flat_coords))))
    
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
         save_path=None, filename_prefix="DFP", title=None):
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
        fig.suptitle('Davidon-Fletcher-Powell (DFP) method', fontsize=14)

    plt.xlim(xlim[0], xlim[1])
    plt.ylim(ylim[0], ylim[1])
    ax.set_aspect('equal', adjustable='box')
    
    # Контур функции
    contourPlot(ax, f, xlim=xlim, ylim=ylim)
    
    # Точки и шаги
    dfpDraw(ax, coords, nsteps)
    
    # Легенда
    ax.legend(loc='upper right')
    
    # Добавляем сетку
    ax.grid(True, alpha=0.2, linestyle='--')
    
    # Информационная панель
    if len(coords) > 0:
        first_point = flat_coords[0] if 'flat_coords' in locals() else (
            coords[0].flatten() if hasattr(coords[0], 'flatten') else coords[0]
        )
        last_point = flat_coords[-1] if 'flat_coords' in locals() else (
            coords[-1].flatten() if hasattr(coords[-1], 'flatten') else coords[-1]
        )
        
        info_text = f"Points: {len(coords)}\nStart: [{float(first_point[0]):.3f}, {float(first_point[1]):.3f}]\nEnd: [{float(last_point[0]):.3f}, {float(last_point[1]):.3f}]"
        ax.text(0.02, 0.98, info_text, transform=ax.transAxes,
                fontsize=10, verticalalignment='top',
                bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue", alpha=0.8))

    # Определяем имя файла
    if filename_prefix:
        name = f"plot_{filename_prefix}.png"
    else:
        name = "plot_DFP.png"

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
    print("ТЕСТИРОВАНИЕ МЕТОДА ДЭВИДОНА-ФЛЕТЧЕРА-ПАУЭЛЛА (DFP)")
    print("="*70)
    
    # Тест 1: Стандартные параметры
    print("\nТест 1: Стандартные параметры (начальная точка [1.5, 1.5])")
    result1 = dfpsearch(
        fH, dfH, 
        [1.5, 1.5], 
        1e-4,
        plot=True,
        save_path="dfp_results",
        filename_prefix="DFP_standard_1.5",
        title="DFP method: Standard parameters"
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ ТЕСТА 1:")
    print("="*60)
    print(f"Точка минимума: [{result1[0][0,0]:.6f}, {result1[0][1,0]:.6f}]")
    print(f"Значение функции: {result1[1]:.6e}")
    print(f"Вычислений градиента: {result1[2]}")
    print(f"Итераций: {result1[4]}")
    
    # Тест 2: Другая начальная точка
    print("\n\nТест 2: Другая начальная точка ([-2, 2])")
    result2 = dfpsearch(
        fH, dfH,
        [-2, 2],
        1e-4,
        plot=True,
        save_path="dfp_results",
        filename_prefix="DFP_standard_-2_2",
        title="DFP method: Start point [-2, 2]"
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ ТЕСТА 2:")
    print("="*60)
    print(f"Точка минимума: [{result2[0][0,0]:.6f}, {result2[0][1,0]:.6f}]")
    print(f"Значение функции: {result2[1]:.6e}")
    print(f"Вычислений градиента: {result2[2]}")
    print(f"Итераций: {result2[4]}")
    
    # Тест 3: Более строгий критерий
    print("\n\nТест 3: Высокая точность (tol=1e-6)")
    result3 = dfpsearch(
        fH, dfH,
        [1.5, 1.5],
        1e-6,
        plot=True,
        save_path="dfp_results",
        filename_prefix="DFP_high_tol",
        title="DFP method: High precision (tol=1e-6)"
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ ТЕСТА 3:")
    print("="*60)
    print(f"Точка минимума: [{result3[0][0,0]:.6f}, {result3[0][1,0]:.6f}]")
    print(f"Значение функции: {result3[1]:.6e}")
    print(f"Вычислений градиента: {result3[2]}")
    print(f"Итераций: {result3[4]}")
    print("="*70)