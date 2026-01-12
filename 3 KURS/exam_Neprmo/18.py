import numpy as np
import matplotlib.pyplot as plt
import os

def f(x):
    return x ** 2 - 10 * np.cos(0.3 * np.pi * x) - 20

def df(x):
    return 2 * x + 3 * np.pi * np.sin(0.3 * np.pi * x)

def ddf(x):
    return 2 + 0.9 * (np.pi ** 2) * np.cos(0.3 * np.pi * x)

def line_search(x, direction, grad, c=0.5, rho=0.5, max_alpha=1.0):
    """
    Простой линейный поиск для определения демпфирующего коэффициента
    по условию достаточного убывания (Армихо)
    
    Параметры:
    x - текущая точка
    direction - направление поиска
    grad - градиент в текущей точке
    c - параметр достаточного убывания (0 < c < 1)
    rho - коэффициент уменьшения шага (0 < rho < 1)
    max_alpha - максимально допустимый шаг
    """
    alpha = max_alpha
    
    # Проверяем несколько значений альфа
    for _ in range(10):
        x_new = x + alpha * direction
        f_x = f(x)
        f_new = f(x_new)
        
        # Проверка условия Армихо (достаточного убывания)
        if f_new <= f_x + c * alpha * grad * direction:
            return alpha
        alpha *= rho
    
    return max(alpha, 1e-8)  # возвращаем минимальное значение

def damped_newton_search(tol, x0, max_iter=1000, use_adaptive_damping=True, c1=1e-4, beta=0.5):
    """
    Демпфированный метод Ньютона для одномерной оптимизации
    с адаптивным выбором длины шага
    
    Параметры:
    tol - требуемая точность (критерий остановки по градиенту)
    x0 - начальная точка
    max_iter - максимальное количество итераций
    use_adaptive_damping - использовать адаптивный выбор шага или фиксированный
    c1 - параметр для условия Вульфа
    beta - коэффициент уменьшения шага
    """
    coords = [x0]
    neval = 0
    k = 0
    x_prev = x0
    alpha = 1.0  # начальный шаг (демпфирующий коэффициент)
    alpha_history = []
    condition_history = []
    
    # Начальные вычисления
    df_xprev = df(x_prev)
    neval += 1
    
    while k < max_iter and np.abs(df_xprev) > tol:
        # Вычисляем гессиан
        ddf_xprev = ddf(x_prev)
        neval += 1
        
        # Проверяем, что гессиан положительный (для минимизации)
        hessian_positive = True
        if ddf_xprev <= 1e-10:
            # Если гессиан неположительный, добавляем демпфирование
            ddf_xprev = max(ddf_xprev, 1e-6)
            alpha = 0.1  # уменьшаем шаг для стабилизации
            hessian_positive = False
        
        # Вычисляем направление Ньютона
        newton_direction = df_xprev / ddf_xprev
        
        if use_adaptive_damping:
            # Используем адаптивный линейный поиск для выбора шага
            if hessian_positive:
                # Вариант 1: линейный поиск с условием Армихо
                alpha = line_search(x_prev, -newton_direction, df_xprev, c=c1)
            else:
                # Вариант 2: при неположительном гессиане используем фиксированный малый шаг
                alpha = 0.1
        else:
            # Фиксированный шаг (можно адаптировать на основе прогресса)
            if k > 0 and condition_history[-1] == 'good':
                alpha = min(alpha * 1.2, 1.0)
            elif k > 0 and condition_history[-1] == 'bad':
                alpha = max(alpha * 0.8, 0.1)
        
        # Обновляем точку
        x = x_prev - alpha * newton_direction
        
        # Вычисляем градиент в новой точке
        df_x = df(x)
        neval += 1
        
        # Сохраняем историю
        coords.append(x)
        alpha_history.append(alpha)
        
        # Проверяем качество шага
        f_prev = f(x_prev)
        f_curr = f(x)
        expected_decrease = c1 * alpha * df_xprev * newton_direction
        
        if f_curr <= f_prev - expected_decrease:
            condition_history.append('good')
        else:
            condition_history.append('bad')
        
        # Обновляем переменные для следующей итерации
        x_prev = x
        df_xprev = df_x
        k += 1
    
    # Финальные вычисления
    xmin = x_prev
    fmin = f(xmin)
    neval += 1
    
    if k >= max_iter:
        print(f"Достигнуто максимальное количество итераций ({max_iter})")
    else:
        print(f"Оптимизация завершена за {k} итераций")
    
    answer_ = {
        'xmin': xmin,
        'fmin': fmin,
        'neval': neval,
        'iterations': k,
        'coords': coords,
        'alpha_history': alpha_history,
        'condition_history': condition_history
    }
    
    return answer_

def wolfe_step(xk, df_xk, ddf_xk, alpha_init=1.0, c1=1e-4, beta=0.5, alpha_min=1e-8):
    """
    Линейный поиск с использованием условия Вульфа
    """
    alpha = alpha_init
    step = alpha * df_xk / ddf_xk
    
    # Условие Вульфа: f(xk - αpk) <= f(xk) - c1 * α * df(xk) * pk
    while f(xk - step) > f(xk) - c1 * alpha * (df_xk ** 2) / ddf_xk and alpha > alpha_min:
        alpha *= beta
        step = alpha * df_xk / ddf_xk
    
    return step

def save_iteration_plots(result, folder_path, max_display_iter=10):
    """
    Создание и сохранение графиков для визуализации процесса оптимизации
    """
    if not os.path.exists(folder_path):
        os.makedirs(folder_path, exist_ok=True)
    
    coords = result['coords']
    alpha_history = result['alpha_history']
    condition_history = result['condition_history']
    
    # Создаем сетку для графика функции
    x_min = min(min(coords) - 2, -5)
    x_max = max(max(coords) + 2, 5)
    x_vals = np.linspace(x_min, x_max, 1000)
    y_vals = f(x_vals)
    
    # График 1: Траектория на графике функции
    fig1, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    
    # Левый график: функция и точки
    ax1.plot(x_vals, y_vals, label="f(x)", linewidth=2, color='blue')
    ax1.scatter(coords, [f(x) for x in coords], color='red', s=50, zorder=5)
    
    # Рисуем линии между точками
    for i in range(len(coords) - 1):
        ax1.plot([coords[i], coords[i+1]], [f(coords[i]), f(coords[i+1])], 
                'g--', alpha=0.5, linewidth=1)
    
    # Подписываем точки
    for i, (x, y) in enumerate(zip(coords, [f(x) for x in coords])):
        if i == 0:
            ax1.text(x, y + 3, f'Start\nx={x:.3f}', 
                    ha='center', va='bottom', fontsize=9,
                    bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.8))
        elif i == len(coords) - 1:
            ax1.text(x, y + 3, f'End\nx={x:.3f}', 
                    ha='center', va='bottom', fontsize=9,
                    bbox=dict(boxstyle='round,pad=0.3', facecolor='green', alpha=0.8))
        elif i % max(1, len(coords)//5) == 0:  # Подписываем каждую 5-ю точку
            ax1.text(x, y + 2, f'{i}', ha='center', va='bottom', fontsize=8)
    
    ax1.set_xlabel('x')
    ax1.set_ylabel('f(x)')
    ax1.set_title(f'Демпфированный метод Ньютона\nТраектория ({len(coords)-1} итераций)')
    ax1.grid(True, alpha=0.3)
    ax1.legend()
    
    # Правый график: сходимость и длина шага
    iterations = range(len(coords))
    f_values = [f(x) for x in coords]
    
    # Градиент (логарифмическая шкала)
    ax2.plot(iterations, [abs(df(x)) for x in coords], 
            'r-', linewidth=2, label='|f\'(x)|')
    
    # Значения функции
    ax2_twin = ax2.twinx()
    ax2_twin.plot(iterations, f_values, 'b--', linewidth=2, label='f(x)')
    
    ax2.set_xlabel('Итерация')
    ax2.set_ylabel('|f\'(x)|', color='red')
    ax2_twin.set_ylabel('f(x)', color='blue')
    ax2.set_title('Сходимость метода')
    ax2.grid(True, alpha=0.3)
    ax2.set_yscale('log')
    
    # Добавляем легенды
    ax2.legend(loc='upper left')
    ax2_twin.legend(loc='upper right')
    
    plt.tight_layout()
    plt.savefig(os.path.join(folder_path, 'newton_trajectory.png'), dpi=150)
    plt.close()
    
    # График 2: Изменение длины шага и качество итераций
    if alpha_history:
        fig2, (ax3, ax4) = plt.subplots(1, 2, figsize=(15, 6))
        
        # Левый график: длина шага
        ax3.plot(range(1, len(alpha_history) + 1), alpha_history, 
                'go-', linewidth=2, markersize=6)
        ax3.set_xlabel('Итерация')
        ax3.set_ylabel('Длина шага (α)')
        ax3.set_title('Изменение длины шага')
        ax3.grid(True, alpha=0.3)
        
        # Цветовая маркировка качества итераций
        if condition_history:
            for i, condition in enumerate(condition_history):
                color = 'green' if condition == 'good' else 'red'
                ax3.plot(i+1, alpha_history[i], 'o', color=color, markersize=8)
        
        # Правый график: гессиан
        ax4.plot(range(len(coords)), [ddf(x) for x in coords], 
                'm-', linewidth=2, label='f\'\'(x)')
        ax4.axhline(y=0, color='gray', linestyle='--', alpha=0.5)
        ax4.set_xlabel('Итерация')
        ax4.set_ylabel('f\'\'(x)')
        ax4.set_title('Значение второй производной (гессиана)')
        ax4.grid(True, alpha=0.3)
        ax4.legend()
        
        plt.tight_layout()
        plt.savefig(os.path.join(folder_path, 'newton_step_info.png'), dpi=150)
        plt.close()
    
    # График 3: Отдельные итерации для анимации/презентации
    for i in range(min(max_display_iter, len(coords))):
        fig3, ax = plt.subplots(figsize=(10, 6))
        
        ax.plot(x_vals, y_vals, label="f(x)", linewidth=2)
        ax.scatter(coords[:i+1], [f(x) for x in coords[:i+1]], 
                  color="red", s=80, zorder=5)
        
        # Рисуем линии до текущей точки
        for j in range(i):
            ax.plot([coords[j], coords[j+1]], [f(coords[j]), f(coords[j+1])], 
                   'g--', alpha=0.7, linewidth=1)
        
        # Подписываем текущую точку
        if i < len(coords) - 1:
            current_alpha = alpha_history[i] if i < len(alpha_history) else 1.0
            ax.text(coords[i], f(coords[i]) + 2, 
                   f'Итерация {i}\nα={current_alpha:.3f}', 
                   ha='center', va='bottom', fontsize=10,
                   bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.8))
        
        ax.set_xlabel('x')
        ax.set_ylabel('f(x)')
        ax.set_title(f'Демпфированный метод Ньютона — итерация {i}')
        ax.grid(True, alpha=0.3)
        ax.legend()
        
        plt.tight_layout()
        plt.savefig(os.path.join(folder_path, f'iteration_{i:03d}.png'), dpi=150)
        plt.close()
    
    print(f"✓ Создано {min(max_display_iter, len(coords))} графиков итераций")
    print(f"✓ Созданы общие графики траектории и сходимости")

def print_detailed_results(result):
    """
    Вывод детальной информации о результатах оптимизации
    """
    print("\n" + "=" * 80)
    print("РЕЗУЛЬТАТЫ ОПТИМИЗАЦИИ - ДЕМПФИРОВАННЫЙ МЕТОД НЬЮТОНА")
    print("=" * 80)
    
    coords = result['coords']
    alpha_history = result['alpha_history']
    
    print(f"\nКонечные результаты:")
    print(f"  Найденный минимум: x = {result['xmin']:.10f}")
    print(f"  Значение функции: f(x) = {result['fmin']:.10f}")
    print(f"  Градиент в точке: f'(x) = {df(result['xmin']):.2e}")
    print(f"  Гессиан в точке: f''(x) = {ddf(result['xmin']):.4f}")
    print(f"  Количество итераций: {result['iterations']}")
    print(f"  Количество вычислений функции/производных: {result['neval']}")
    
    if alpha_history:
        print(f"  Средняя длина шага: {np.mean(alpha_history):.4f}")
        print(f"  Минимальная длина шага: {np.min(alpha_history):.4f}")
        print(f"  Максимальная длина шага: {np.max(alpha_history):.4f}")
    
    print(f"\nИстория итераций (первые и последние 3):")
    print("Ит. |       x       |      f(x)     |    f'(x)    |    f''(x)   |    α    ")
    print("-" * 80)
    
    # Первые 4 итерации
    for i in range(min(4, len(coords))):
        x_val = coords[i]
        alpha_val = alpha_history[i-1] if i > 0 and i-1 < len(alpha_history) else 1.0
        print(f"{i:3d} | {x_val:12.6f} | {f(x_val):12.6f} | {df(x_val):11.6f} | {ddf(x_val):10.4f} | {alpha_val:7.4f}")
    
    if len(coords) > 7:
        print("...")
        # Последние 4 итерации
        for i in range(max(4, len(coords)-4), len(coords)):
            x_val = coords[i]
            alpha_val = alpha_history[i-1] if i > 0 and i-1 < len(alpha_history) else 1.0
            print(f"{i:3d} | {x_val:12.6f} | {f(x_val):12.6f} | {df(x_val):11.6f} | {ddf(x_val):10.4f} | {alpha_val:7.4f}")
    
    # Проверка условий оптимальности
    print(f"\nПроверка условий оптимальности:")
    final_grad = df(result['xmin'])
    final_hess = ddf(result['xmin'])
    print(f"  |f'(x*)| = {abs(final_grad):.2e} {'<=' if abs(final_grad) < 1e-6 else '>'} 1e-6")
    print(f"  f''(x*) = {final_hess:.4f} {'>' if final_hess > 0 else '<='} 0")
    
    if abs(final_grad) < 1e-6 and final_hess > 0:
        print("  ✓ Условия оптимальности выполнены (локальный минимум)")
    else:
        print("  ⚠ Условия оптимальности не полностью выполнены")

if __name__ == "__main__":
    # Параметры оптимизации
    x0 = 1.0
    tol = 1e-8
    max_iter = 100
    folder = "./newton_damped_results"  # Относительный путь
    
    print("=" * 80)
    print("ДЕМПФИРОВАННЫЙ МЕТОД НЬЮТОНА ДЛЯ ОДНОМЕРНОЙ ОПТИМИЗАЦИИ")
    print("=" * 80)
    print(f"Функция: f(x) = x² - 10*cos(0.3πx) - 20")
    print(f"Начальная точка: x0 = {x0}")
    print(f"Точность: tol = {tol}")
    print(f"Максимальное число итераций: {max_iter}")
    print("\nЗапуск оптимизации...")
    
    # Запуск демпфированного метода Ньютона
    result = damped_newton_search(tol, x0, max_iter=max_iter, 
                                 use_adaptive_damping=True, c1=1e-4, beta=0.5)
    
    # Вывод результатов
    print_detailed_results(result)
    
    # Создание графиков
    print("\nСоздание визуализации...")
    save_iteration_plots(result, folder_path=folder, max_display_iter=10)
    
    # Вывод HTML для отображения графиков
    print("\n" + "=" * 80)
    print("ВИЗУАЛИЗАЦИЯ РЕЗУЛЬТАТОВ")
    print("=" * 80)
    
    if os.path.exists(os.path.join(folder, 'newton_trajectory.png')):
        print('\n<img width="900px" src="newton_trajectory.png">')
    
    if os.path.exists(os.path.join(folder, 'newton_step_info.png')):
        print('\n<img width="900px" src="newton_step_info.png">')
    
    print(f"\n✓ Все графики сохранены в папке: {os.path.abspath(folder)}")