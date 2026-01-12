import numpy as np
import matplotlib
import os
import sys
from numpy.linalg import norm

matplotlib.use('Agg')
import matplotlib.pyplot as plt

# --- Класс L-BFGS и вспомогательные функции ---
class LBFGS:
    """
    Метод L-BFGS (Limited-memory BFGS) для больших задач
    """
    def __init__(self, m=10):
        """
        Инициализация L-BFGS
        
        Параметры:
        m - количество пар (s, y) для хранения в памяти
        """
        self.m = m
        self.s_history = []
        self.y_history = []
        self.rho_history = []
    
    def two_loop_recursion(self, grad):
        """
        Двухцикловая процедура для вычисления направления поиска
        """
        q = grad.copy()
        n = len(grad)
        
        # Количество сохраненных пар
        k = len(self.s_history)
        
        # Прямой цикл
        alpha = np.zeros(k)
        for i in range(k-1, -1, -1):
            alpha[i] = self.rho_history[i] * np.dot(self.s_history[i], q)
            q = q - alpha[i] * self.y_history[i]
        
        # Начальная аппроксимация H0 (скалярная)
        if k > 0:
            H0 = np.dot(self.s_history[-1], self.y_history[-1]) / np.dot(self.y_history[-1], self.y_history[-1])
        else:
            H0 = 1.0
        
        r = H0 * q
        
        # Обратный цикл
        for i in range(k):
            beta = self.rho_history[i] * np.dot(self.y_history[i], r)
            r = r + (alpha[i] - beta) * self.s_history[i]
        
        return -r
    
    def optimize(self, f, grad_f, x0, max_iter=1000, tol=1e-6, alpha_init=1.0):
        """
        Оптимизация методом L-BFGS
        """
        x = np.array(x0, dtype=float)
        grad = grad_f(x)
        
        history = {
            'x': [x.copy()],
            'f': [f(x)],
            'grad_norm': [norm(grad)],
            'iterations': 0
        }
        
        for k in range(max_iter):
            # Вычисляем направление поиска
            p = self.two_loop_recursion(grad)
            
            # Простой линейный поиск с условием Армихо
            alpha = alpha_init
            for ls_iter in range(20):
                x_new = x + alpha * p
                f_new = f(x_new)
                
                # Условие Армихо
                if f_new <= f(x) + 1e-4 * alpha * np.dot(grad, p):
                    break
                alpha *= 0.5
            
            # Сохраняем старые значения
            x_old = x.copy()
            grad_old = grad.copy()
            
            # Делаем шаг
            x = x + alpha * p
            grad = grad_f(x)
            
            # Обновляем историю пар (s, y)
            s = x - x_old
            y = grad - grad_old
            
            s_dot_y = np.dot(s, y)
            
            if s_dot_y > 1e-10:
                rho = 1.0 / s_dot_y
                
                self.s_history.append(s)
                self.y_history.append(y)
                self.rho_history.append(rho)
                
                # Поддерживаем ограниченную память
                if len(self.s_history) > self.m:
                    self.s_history.pop(0)
                    self.y_history.pop(0)
                    self.rho_history.pop(0)
            
            # Сохраняем историю
            history['x'].append(x.copy())
            history['f'].append(f(x))
            history['grad_norm'].append(norm(grad))
            history['iterations'] = k + 1
            
            # Критерий остановки
            if norm(grad) < tol:
                break
        
        result = {
            'x_min': x,
            'f_min': f(x),
            'success': norm(grad) < tol,
            'iterations': history['iterations'],
            'grad_norm': norm(grad),
            'neval': (k + 1) * 2,
            'history': history
        }
        
        return result

# --- Тестовые функции ---
def fH(X):
    """Функция Химмельблау"""
    if isinstance(X, np.ndarray) and len(X.shape) > 1 and X.shape[0] == 2 and X.shape[1] == 1:
        x = X[0, 0]
        y = X[1, 0]
    else:
        x = X[0]
        y = X[1]
    v = (x**2 + y - 11)**2 + (x + y**2 - 7)**2
    return v

def dfH(X):
    """Градиент функции Химмельблау"""
    if isinstance(X, np.ndarray) and len(X.shape) > 1 and X.shape[0] == 2 and X.shape[1] == 1:
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
    if isinstance(X, np.ndarray) and len(X.shape) > 1 and X.shape[0] == 2 and X.shape[1] == 1:
        x = X[0, 0]
        y = X[1, 0]
    else:
        x = X[0]
        y = X[1]
    return (1 - x)**2 + 100 * (y - x**2)**2

def dfR(X):
    """Градиент функции Розенброка"""
    if isinstance(X, np.ndarray) and len(X.shape) > 1 and X.shape[0] == 2 and X.shape[1] == 1:
        x = X[0, 0]
        y = X[1, 0]
    else:
        x = X[0]
        y = X[1]
    
    v = np.array([0.0, 0.0])
    v[0] = -2*(1 - x) - 400*x*(y - x**2)
    v[1] = 200*(y - x**2)
    return v

# --- Основная функция L-BFGS с визуализацией ---
def lbfgssearch(f, df, x0, tol=1e-6, max_iter=1000, plot=True, save_path=None, 
                filename_prefix="LBFGS", title=None, alpha_init=1.0, m=10):
    """
    Метод L-BFGS (Limited-memory BFGS) с возможностью отрисовки.
    
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
    m : int
        Количество пар для хранения в памяти
    
    Returns:
    --------
    result : dict
        Словарь с результатами оптимизации
    """
    # Создаем оптимизатор L-BFGS
    lbfgs_solver = LBFGS(m=m)
    
    # Запускаем оптимизацию
    result = lbfgs_solver.optimize(f, df, x0, max_iter=max_iter, tol=tol, alpha_init=alpha_init)
    
    # Извлекаем данные для визуализации
    coords = result['history']['x']
    f_values = result['history']['f']
    grad_norms = result['history']['grad_norm']
    iterations = result['iterations']
    
    print(f"Запуск метода L-BFGS с параметрами:")
    print(f"  Начальная точка: [{x0[0]:.2f}, {x0[1]:.2f}]")
    print(f"  Точность: {tol}")
    print(f"  Максимальное число итераций: {max_iter}")
    print(f"  Память (m): {m}")
    print(f"  Начальная длина шага: {alpha_init}")
    
    print(f"\nРезультаты метода L-BFGS:")
    print(f"  x_min = [{result['x_min'][0]:.6f}, {result['x_min'][1]:.6f}], f_min = {result['f_min']:.6e}")
    print(f"  Итераций: {iterations}")
    print(f"  Норма градиента: {result['grad_norm']:.2e}")
    print(f"  Успех: {result['success']}")
    print(f"  Число точек в траектории: {len(coords)}")
    print(f"  Использовано пар в памяти: {len(lbfgs_solver.s_history)}")

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
            title = f'L-BFGS method (m={m}): {func_name} function'
        
        draw(coords, len(coords)-1, f, xlim=xlim, ylim=ylim, 
             save_path=save_path, filename_prefix=filename_prefix, 
             title=title, f_values=f_values, grad_norms=grad_norms, m=m)
    
    return result

# --- Функции для визуализации (адаптированные) ---
def contourPlot(ax, f, xlim=(-5, 5), ylim=(-5, 5)):
    """Отрисовка контурных линий функции"""
    x1 = np.arange(xlim[0], xlim[1] + 0.1, 0.1)
    m = len(x1)
    y1 = np.arange(ylim[0], ylim[1] + 0.1, 0.1)
    n = len(y1)

    [xx, yy] = np.meshgrid(x1, y1)

    F = np.zeros((n, m))

    for i in range(n):
        for j in range(m):
            X = [xx[i, j], yy[i, j]]
            F[i, j] = f(X)

    nlevels = 25
    contours = ax.contour(xx, yy, F, nlevels, linewidths=1, colors='gray', alpha=0.6)
    ax.clabel(contours, inline=True, fontsize=8)
    ax.set_xlabel('x')
    ax.set_ylabel('y')

def lbfgsDraw(ax, coords, nsteps):
    """Отрисовка траектории метода L-BFGS"""
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
    colors = plt.cm.coolwarm(np.linspace(0, 1, min(nsteps, len(flat_coords))))
    
    for i in range(min(nsteps, len(flat_coords)) - 1):
        x0 = flat_coords[i]
        x1 = flat_coords[i + 1]
        
        ax.plot([float(x0[0]), float(x1[0])],
                [float(x0[1]), float(x1[1])],
                lw=1.5, marker='o', ms=4, color=colors[i], alpha=0.8)
        
        # Подпись номера итерации (каждые 5 итераций или последние 3)
        if i % 5 == 0 or i >= len(flat_coords) - 4:
            mid_x = (float(x0[0]) + float(x1[0])) / 2
            mid_y = (float(x0[1]) + float(x1[1]) / 2)
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
         save_path=None, filename_prefix="LBFGS", title=None, 
         f_values=None, grad_norms=None, m=10):
    """
    Основная функция отрисовки для метода L-BFGS
    
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
    grad_norms : list
        Нормы градиента в точках траектории
    m : int
        Количество пар в памяти L-BFGS
    """
    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(18, 5))
    
    if title:
        fig.suptitle(title, fontsize=14)
    else:
        fig.suptitle(f'L-BFGS method (m={m})', fontsize=14)

    # --- Первый график: траектория и контуры ---
    ax1.set_xlim(xlim[0], xlim[1])
    ax1.set_ylim(ylim[0], ylim[1])
    ax1.set_aspect('equal', adjustable='box')
    
    # Контур функции
    contourPlot(ax1, f, xlim=xlim, ylim=ylim)
    
    # Точки и шаги
    lbfgsDraw(ax1, coords, nsteps)
    
    # Легенда
    ax1.legend(loc='upper right')
    ax1.grid(True, alpha=0.2, linestyle='--')
    ax1.set_title(f'Trajectory (m={m})')
    
    # Информационная панель
    if len(coords) > 0:
        first_point = coords[0].flatten() if hasattr(coords[0], 'flatten') else coords[0]
        last_point = coords[-1].flatten() if hasattr(coords[-1], 'flatten') else coords[-1]
        
        info_text = f"Points: {len(coords)}\nStart: [{float(first_point[0]):.3f}, {float(first_point[1]):.3f}]\nEnd: [{float(last_point[0]):.3f}, {float(last_point[1]):.3f}]"
        ax1.text(0.02, 0.98, info_text, transform=ax1.transAxes,
                fontsize=8, verticalalignment='top',
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

    # --- Третий график: сходимость градиента ---
    if grad_norms is not None and len(grad_norms) > 1:
        iterations = list(range(len(grad_norms)))
        ax3.semilogy(iterations, grad_norms, 'g-', linewidth=2, marker='s', markersize=4)
        ax3.set_xlabel('Iteration')
        ax3.set_ylabel('Gradient norm (log scale)')
        ax3.set_title('Gradient Norm Convergence')
        ax3.grid(True, alpha=0.3)
        ax3.set_xlim(0, len(grad_norms)-1)
        
        # Добавляем горизонтальную линию для порога сходимости
        ax3.axhline(y=1e-6, color='r', linestyle='--', alpha=0.5, label='Tol: 1e-6')
        ax3.legend()
    
    plt.tight_layout()
    
    # Определяем имя файла
    if filename_prefix:
        name = f"plot_{filename_prefix}.png"
    else:
        name = "plot_LBFGS.png"

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
    print("ТЕСТИРОВАНИЕ МЕТОДА L-BFGS (LIMITED-MEMORY BFGS)")
    print("="*70)
    
    # Тест 1: Функция Химмельблау с разной памятью
    print("\nТест 1: Функция Химмельблау, разный размер памяти")
    
    for m in [3, 5, 10]:
        print(f"\n--- L-BFGS с памятью m={m} ---")
        result1 = lbfgssearch(
            fH, dfH, 
            [1.5, 1.5], 
            tol=1e-6,
            max_iter=200,
            plot=True,
            save_path="lbfgs_results",
            filename_prefix=f"LBFGS_Himmelblau_m{m}",
            title=f"L-BFGS method (m={m}): Himmelblau function",
            alpha_init=1.0,
            m=m
        )
    
    # Тест 2: Функция Розенброка
    print("\n\nТест 2: Функция Розенброка")
    result2 = lbfgssearch(
        fR, dfR,
        [-1.2, 1.0],
        tol=1e-6,
        max_iter=500,
        plot=True,
        save_path="lbfgs_results",
        filename_prefix="LBFGS_Rosenbrock",
        title="L-BFGS method: Rosenbrock function",
        alpha_init=1.0,
        m=10
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
    result3 = lbfgssearch(
        fH, dfH,
        [-2, 2],
        tol=1e-6,
        max_iter=200,
        plot=True,
        save_path="lbfgs_results",
        filename_prefix="LBFGS_Himmelblau_-2_2",
        title="L-BFGS method: Himmelblau function (start [-2, 2])",
        alpha_init=1.0,
        m=10
    )
    
    print("\n" + "="*60)
    print("РЕЗУЛЬТАТЫ ТЕСТА 3 (Химмельблау другая точка):")
    print("="*60)
    print(f"Точка минимума: [{result3['x_min'][0]:.6f}, {result3['x_min'][1]:.6f}]")
    print(f"Значение функции: {result3['f_min']:.6e}")
    print(f"Итераций: {result3['iterations']}")
    print(f"Норма градиента: {result3['grad_norm']:.2e}")
    print("="*70)