import numpy as np
import matplotlib

matplotlib.use('Agg')
import matplotlib.pyplot as plt
import os

# Устанавливаем обработку предупреждений
import warnings
warnings.filterwarnings('ignore')

def fH(X):
    """Функция Химмельблау"""
    x = X[0]
    y = X[1]
    return (x ** 2 + y - 11) ** 2 + (x + y ** 2 - 7) ** 2

def dfH(X):
    """Градиент функции Химмельблау"""
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = 2 * (x ** 2 + y - 11) * (2 * x) + 2 * (x + y ** 2 - 7)
    v[1] = 2 * (x ** 2 + y - 11) + 2 * (x + y ** 2 - 7) * (2 * y)
    return v

def fR(X):
    """Функция Розенброка"""
    x = X[0]
    y = X[1]
    return (1 - x) ** 2 + 100 * (y - x ** 2) ** 2

def dfR(X):
    """Градиент функции Розенброка"""
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = -2 * (1 - x) + 200 * (y - x ** 2) * (- 2 * x)
    v[1] = 200 * (y - x ** 2)
    return v

def numerical_hessian(x0, df, f, tol=1e-4):
    """
    Численное вычисление гессиана методом конечных разностей
    с защитой от переполнения
    """
    n = len(x0)
    H = np.eye(n, dtype=np.float64)  # Начинаем с единичной матрицы
    delta = tol
    
    # Ограничиваем координаты для предотвращения переполнения
    x0_safe = np.clip(x0, -1e10, 1e10)
    
    try:
        grad_center = df(x0_safe)
    except Exception:
        return H
    
    for i in range(n):
        ei = np.zeros(n)
        ei[i] = 1.0
        
        try:
            # Безопасное вычисление точек
            x_forward = x0_safe + delta * ei
            x_backward = x0_safe - delta * ei
            
            # Ограничиваем значения
            x_forward = np.clip(x_forward, -1e10, 1e10)
            x_backward = np.clip(x_backward, -1e10, 1e10)
            
            grad_forward = df(x_forward)
            grad_backward = df(x_backward)
            
            # Вычисляем вторую производную с защитой от переполнения
            numerator = grad_forward[i] - 2*grad_center[i] + grad_backward[i]
            if np.abs(numerator) > 1e100:
                H[i, i] = 2.0
            else:
                H[i, i] = numerator / (delta**2)
                
                # Ограничиваем значение
                H[i, i] = np.clip(H[i, i], -1e10, 1e10)
        except Exception:
            H[i, i] = 2.0
        
        for j in range(i+1, n):
            ej = np.zeros(n)
            ej[j] = 1.0
            
            try:
                # Безопасное вычисление точек для смешанной производной
                points = [
                    x0_safe + delta * ei + delta * ej,
                    x0_safe + delta * ei - delta * ej,
                    x0_safe - delta * ei + delta * ej,
                    x0_safe - delta * ei - delta * ej
                ]
                
                # Ограничиваем все точки
                points = [np.clip(p, -1e10, 1e10) for p in points]
                
                # Вычисляем значения функции
                f_vals = []
                for p in points:
                    try:
                        f_val = f(p)
                        # Ограничиваем значение функции
                        f_val = np.clip(f_val, -1e10, 1e10)
                        f_vals.append(f_val)
                    except Exception:
                        f_vals.append(0.0)
                
                # Вычисляем смешанную производную
                numerator = f_vals[0] - f_vals[1] - f_vals[2] + f_vals[3]
                if np.abs(numerator) > 1e100:
                    mixed_deriv = 0.0
                else:
                    mixed_deriv = numerator / (4 * delta**2)
                    mixed_deriv = np.clip(mixed_deriv, -1e10, 1e10)
                
                H[i, j] = mixed_deriv
                H[j, i] = mixed_deriv
            except Exception:
                H[i, j] = 0.0
                H[j, i] = 0.0
    
    return H

def newton_search(f, df, x0, tol=1e-6, max_iter=30, alpha=0.5, adaptive_alpha=True):
    """
    Устойчивый метод Ньютона с защитой от переполнения
    """
    x_k = np.array(x0, dtype=np.float64).copy()
    coords = [x_k.copy()]
    neval = 0
    k = 0
    alpha_history = []
    f_history = []
    grad_norm_history = []
    
    # Ограничиваем начальную точку
    x_k = np.clip(x_k, -100.0, 100.0)
    
    try:
        f_current = f(x_k)
        f_current = np.clip(f_current, -1e10, 1e10)
        f_history.append(f_current)
    except Exception:
        f_history.append(0.0)
    
    print("  Прогресс оптимизации:")
    
    while k < max_iter:
        try:
            # Вычисляем градиент
            g_k = df(x_k)
            grad_norm = np.linalg.norm(g_k)
            
            # Ограничиваем градиент
            grad_norm = np.clip(grad_norm, 0.0, 1e10)
            grad_norm_history.append(grad_norm)
            neval += 1
        except Exception:
            print(f"    Итерация {k}: ошибка при вычислении градиента")
            break
        
        # Проверка сходимости
        if grad_norm < tol and k > 0:
            print(f"    Итерация {k}: достигнута требуемая точность")
            break
        
        # Вычисляем гессиан
        try:
            H_k = numerical_hessian(x_k, df, f, tol/10)
            neval += 4
        except Exception as e:
            print(f"    Итерация {k}: ошибка при вычислении гессиана")
            H_k = np.eye(len(x0))
        
        # Решаем систему уравнений
        try:
            # Добавляем регуляризацию для устойчивости
            H_reg = H_k + 1e-6 * np.eye(len(x0))
            d_k = np.linalg.solve(H_reg, -g_k)
            
            # Ограничиваем направление
            d_norm = np.linalg.norm(d_k)
            if d_norm > 100.0:
                d_k = d_k * 100.0 / d_norm
        except Exception:
            # Используем градиентное направление при ошибке
            d_k = -g_k / max(grad_norm, 1e-8)
        
        # Выбираем длину шага
        if adaptive_alpha and k > 0:
            try:
                # Простой линейный поиск
                alpha_current = 0.5
                for _ in range(5):
                    x_test = x_k + alpha_current * d_k
                    x_test = np.clip(x_test, -100.0, 100.0)
                    f_test = f(x_test)
                    f_test = np.clip(f_test, -1e10, 1e10)
                    
                    if f_test < f_current or alpha_current < 0.01:
                        break
                    alpha_current *= 0.5
            except Exception:
                alpha_current = 0.1
        else:
            alpha_current = alpha
        
        # Ограничиваем длину шага
        alpha_current = min(alpha_current, 1.0)
        alpha_history.append(alpha_current)
        
        # Обновляем точку
        x_new = x_k + alpha_current * d_k
        
        # Ограничиваем новую точку
        x_new = np.clip(x_new, -100.0, 100.0)
        
        # Сохраняем историю
        coords.append(x_new.copy())
        
        try:
            f_new = f(x_new)
            f_new = np.clip(f_new, -1e10, 1e10)
            f_history.append(f_new)
        except Exception:
            f_new = np.inf
            f_history.append(f_new)
        
        # Выводим информацию о прогрессе
        if k % 5 == 0 or k == max_iter - 1:
            print(f"    Итерация {k}: f(x) = {f_new:.6e}, ||∇f|| = {grad_norm:.6e}, α = {alpha_current:.4f}")
        
        # Проверяем сходимость
        delta_x = np.linalg.norm(x_new - x_k)
        if k > 0 and len(f_history) > 1:
            delta_f = abs(f_history[-1] - f_history[-2])
        else:
            delta_f = np.inf
        
        if delta_x < tol and delta_f < tol and k > 0:
            print(f"    Итерация {k}: сходимость достигнута")
            break
        
        # Обновляем переменные
        x_k = x_new.copy()
        f_current = f_new
        k += 1
    
    if k >= max_iter:
        print(f"    Достигнуто максимальное количество итераций ({max_iter})")
    
    # Финальные вычисления
    xmin = x_k.copy()
    try:
        fmin = f(xmin)
        fmin = np.clip(fmin, -1e10, 1e10)
        final_grad_norm = np.linalg.norm(df(xmin))
        final_grad_norm = np.clip(final_grad_norm, 0.0, 1e10)
    except Exception:
        fmin = np.inf
        final_grad_norm = np.inf
    
    neval += 1
    
    result = {
        'xmin': xmin,
        'fmin': fmin,
        'neval': neval,
        'iterations': k,
        'coords': coords,
        'alpha_history': alpha_history,
        'f_history': f_history,
        'grad_norm': final_grad_norm,
        'grad_norm_history': grad_norm_history
    }
    
    return result

def contour_plot(ax, f, bounds=(-4, 4)):
    """
    Создание контурного графика функции с защитой от переполнения
    """
    # Подготовка сетки
    x_range = np.arange(bounds[0], bounds[1] + 0.1, 0.1)
    y_range = np.arange(bounds[0], bounds[1] + 0.1, 0.1)
    xx, yy = np.meshgrid(x_range, y_range)
    
    # Вычисляем значения функции
    F = np.zeros_like(xx, dtype=np.float64)
    for i in range(xx.shape[0]):
        for j in range(xx.shape[1]):
            try:
                val = f([xx[i, j], yy[i, j]])
                # Ограничиваем значение функции
                val = np.clip(val, -1e6, 1e6)
                F[i, j] = val
            except Exception:
                F[i, j] = np.nan
    
    # Заменяем inf и очень большие значения
    F = np.where(np.isinf(F), np.nan, F)
    F = np.where(np.abs(F) > 1e6, np.nan, F)
    
    # Рисуем контуры
    try:
        valid_values = F[~np.isnan(F)]
        if len(valid_values) > 10:
            # Используем квантили для уровней
            min_val = np.percentile(valid_values, 5)
            max_val = np.percentile(valid_values, 95)
            if max_val - min_val > 1e-10:
                levels = np.linspace(min_val, max_val, 10)
                contour = ax.contour(xx, yy, F, levels=levels, 
                                    linewidths=0.8, colors='gray', alpha=0.7)
                ax.clabel(contour, inline=True, fontsize=8)
    except Exception:
        pass
    
    # Отмечаем минимумы для Химмельблау
    if f.__name__ == 'fH':
        minima = [
            [3.0, 2.0],
            [-2.805118, 3.131312],
            [-3.779310, -3.283186],
            [3.584428, -1.848126]
        ]
        for i, min_point in enumerate(minima):
            ax.scatter(min_point[0], min_point[1], c='green', s=50, 
                      marker='*', zorder=5, alpha=0.7,
                      label='Минимумы Химмельблау' if i == 0 else None)
    
    # Отмечаем минимум для Розенброка
    elif f.__name__ == 'fR':
        ax.scatter(1.0, 1.0, c='green', s=50, 
                  marker='*', zorder=5, alpha=0.7,
                  label='Минимум Розенброка')
    
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.grid(True, alpha=0.3, linestyle='--', linewidth=0.5)
    
    return ax

def newton_trajectory_draw(ax, coords, alpha_history=None):
    """
    Визуализация траектории метода Ньютона с защитой от переполнения
    """
    if len(coords) < 2:
        return ax
    
    f_size = 9
    max_points = min(len(coords), 20)  # Ограничиваем количество отображаемых точек
    
    # Рисуем траекторию
    for i in range(max_points - 1):
        if i >= len(coords) - 1:
            break
            
        x0 = coords[i]
        x1 = coords[i + 1]
        
        # Проверяем координаты на корректность
        if (np.any(np.isinf(x0)) or np.any(np.isnan(x0)) or 
            np.any(np.isinf(x1)) or np.any(np.isnan(x1))):
            continue
        
        # Проверяем разность координат
        dx = x1[0] - x0[0]
        dy = x1[1] - x0[1]
        
        # Ограничиваем разность координат для предотвращения переполнения
        if abs(dx) > 100 or abs(dy) > 100:
            continue
        
        # Вычисляем длину с защитой от переполнения
        try:
            length_sq = dx*dx + dy*dy
            if length_sq > 1e10:  # Очень большая длина
                continue
            length = np.sqrt(length_sq)
        except Exception:
            continue
        
        # Линия траектории
        line_width = 2.0 if i == 0 else 1.5
        ax.plot([x0[0], x1[0]], [x0[1], x1[1]], 
               'b-', lw=line_width, alpha=0.8, zorder=10)
        
        # Маркер шага (только для первых нескольких точек)
        if i < 10:
            marker_size = 60 if i == 0 else 40
            ax.scatter(x0[0], x0[1], c='blue', s=marker_size, 
                      marker='o', alpha=0.8, zorder=11)
    
    # Начальная точка
    if len(coords) > 0:
        start_point = coords[0]
        if not (np.any(np.isinf(start_point)) or np.any(np.isnan(start_point))):
            ax.scatter(start_point[0], start_point[1], c='green', s=150, 
                      marker='s', zorder=20, label='Начальная точка', edgecolor='black')
            ax.text(start_point[0] + 0.2, start_point[1] + 0.2, 'Start', 
                   fontsize=f_size+1, weight='bold', 
                   bbox=dict(boxstyle="round,pad=0.4", facecolor='yellow', alpha=0.9))
    
    # Конечная точка
    if len(coords) > 0:
        end_point = coords[-1]
        if not (np.any(np.isinf(end_point)) or np.any(np.isnan(end_point))):
            ax.scatter(end_point[0], end_point[1], c='red', s=200, 
                      marker='*', zorder=20, label='Конечная точка', edgecolor='black')
            ax.text(end_point[0] + 0.2, end_point[1] + 0.2, 'End', 
                   fontsize=f_size+1, weight='bold', 
                   bbox=dict(boxstyle="round,pad=0.4", facecolor='pink', alpha=0.9))
    
    # Добавляем легенду
    if len(coords) > 0:
        ax.legend(loc='best', fontsize=9)
    
    return ax

def create_simple_visualization(result, function_name, function, bounds=(-4, 4), save_path=None):
    """
    Упрощенная визуализация для предотвращения ошибок
    """
    coords = result.get('coords', [])
    
    # Ограничиваем координаты для отображения
    display_coords = []
    for point in coords:
        if (not np.any(np.isinf(point)) and not np.any(np.isnan(point)) and
            abs(point[0]) <= 100 and abs(point[1]) <= 100):
            display_coords.append(point)
    
    if len(display_coords) == 0:
        print("  Нет корректных координат для визуализации")
        return None
    
    # Создаем простой график
    fig, ax = plt.subplots(figsize=(10, 8))
    
    # Контурный график
    contour_plot(ax, function, bounds)
    
    # Траектория
    if len(display_coords) > 1:
        # Просто линии между точками
        x_vals = [p[0] for p in display_coords]
        y_vals = [p[1] for p in display_coords]
        ax.plot(x_vals, y_vals, 'b-', lw=2, alpha=0.8, zorder=10, label='Траектория')
        
        # Начальная и конечная точки
        ax.scatter(x_vals[0], y_vals[0], c='green', s=150, 
                  marker='s', zorder=20, label='Начало')
        ax.scatter(x_vals[-1], y_vals[-1], c='red', s=150, 
                  marker='*', zorder=20, label='Конец')
    
    ax.set_xlim(bounds[0], bounds[1])
    ax.set_ylim(bounds[0], bounds[1])
    ax.set_aspect('equal', adjustable='box')
    ax.set_title(f'Метод Ньютона: {function_name}\n'
                f'Итераций: {result.get("iterations", 0)}, '
                f'f(x) = {result.get("fmin", 0):.6e}')
    ax.legend(loc='best')
    ax.grid(True, alpha=0.3)
    
    plt.tight_layout()
    
    # Сохраняем график
    if save_path is None:
        save_path = f'newton_{function_name.lower()}.png'
    
    try:
        plt.savefig(save_path, dpi=100, bbox_inches='tight')
        print(f"  ✓ График сохранен: {os.path.abspath(save_path)}")
    except Exception as e:
        print(f"  ✗ Ошибка при сохранении графика: {e}")
        return None
    
    plt.close(fig)
    return save_path

def print_newton_results(result, function_name, x0):
    """
    Вывод результатов оптимизации
    """
    print("\n" + "=" * 60)
    print(f"РЕЗУЛЬТАТЫ: {function_name.upper()}")
    print("=" * 60)
    
    print(f"Начальная точка: [{x0[0]:.2f}, {x0[1]:.2f}]")
    
    if 'xmin' in result and not np.any(np.isinf(result['xmin'])) and not np.any(np.isnan(result['xmin'])):
        print(f"Найденный минимум: [{result['xmin'][0]:.8f}, {result['xmin'][1]:.8f}]")
        print(f"Значение функции: {result['fmin']:.10f}")
        print(f"Норма градиента: {result.get('grad_norm', 0):.2e}")
    else:
        print("Минимум не найден (возможно, расходимость)")
    
    print(f"Итераций: {result.get('iterations', 0)}")
    print(f"Вычислений: {result.get('neval', 0)}")

def main():
    """
    Основная функция
    """
    print("=" * 70)
    print("МЕТОД НЬЮТОНА ДЛЯ МНОГОМЕРНОЙ ОПТИМИЗАЦИИ")
    print("=" * 70)
    
    # Создаем папку для результатов
    output_dir = "newton_results"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir, exist_ok=True)
    
    print("\n" + "=" * 70)
    print("ТЕСТ 1: ФУНКЦИЯ ХИММЕЛЬБЛАУ")
    print("=" * 70)
    
    # Используем несколько начальных точек для Химмельблау
    test_points_h = [
        [0.0, 0.0],    # Центр
        [1.0, 1.0],    # Близко к минимуму
        [-2.0, 2.0]    # Другой район
    ]
    
    for idx, x0 in enumerate(test_points_h):
        print(f"\nТест {idx+1}: начальная точка [{x0[0]}, {x0[1]}]")
        result = newton_search(fH, dfH, x0, tol=1e-4, max_iter=20, alpha=0.3)
        print_newton_results(result, f"Химмельблау (тест {idx+1})", x0)
        
        # Визуализация только если есть корректные точки
        if len(result.get('coords', [])) > 1:
            save_path = os.path.join(output_dir, f'newton_himmelblau_{idx+1}.png')
            create_simple_visualization(result, f"Химмельблау {idx+1}", fH, 
                                       bounds=(-5, 5), save_path=save_path)
    
    print("\n" + "=" * 70)
    print("ТЕСТ 2: ФУНКЦИЯ РОЗЕНБРОКА")
    print("=" * 70)
    
    # Для Розенброка используем точки ближе к минимуму
    test_points_r = [
        [0.5, 0.5],    # Близко к минимуму
        [0.0, 0.0],    # Начало координат
        [-0.5, 1.0]    # Другая точка
    ]
    
    for idx, x0 in enumerate(test_points_r):
        print(f"\nТест {idx+1}: начальная точка [{x0[0]}, {x0[1]}]")
        result = newton_search(fR, dfR, x0, tol=1e-4, max_iter=15, alpha=0.2)
        print_newton_results(result, f"Розенброка (тест {idx+1})", x0)
        
        if len(result.get('coords', [])) > 1:
            save_path = os.path.join(output_dir, f'newton_rosenbrock_{idx+1}.png')
            create_simple_visualization(result, f"Розенброка {idx+1}", fR, 
                                       bounds=(-2, 2), save_path=save_path)
    
    print("\n" + "=" * 70)
    print("ИТОГИ")
    print("=" * 70)
    
    print("\nСозданные файлы:")
    for file in os.listdir(output_dir):
        if file.endswith('.png'):
            print(f"  • {file}")
    
    print(f"\nВсе файлы сохранены в: {os.path.abspath(output_dir)}")

if __name__ == "__main__":
    try:
        main()
        print("\n✓ Программа успешно завершена")
    except KeyboardInterrupt:
        print("\n✗ Программа прервана пользователем")
    except Exception as e:
        print(f"\n✗ Критическая ошибка: {e}")
        print("Программа завершена с ошибкой")