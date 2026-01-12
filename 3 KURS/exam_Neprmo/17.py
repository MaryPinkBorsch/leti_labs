import numpy as np
import matplotlib

matplotlib.use('Agg')
import matplotlib.pyplot as plt
import os

def himmelblau(x):
    """Функция Химмельблау f(x,y) = (x^2 + y - 11)^2 + (x + y^2 - 7)^2"""
    return (x[0]**2 + x[1] - 11)**2 + (x[0] + x[1]**2 - 7)**2

def himmelblau_grad(x):
    """Градиент функции Химмельблау"""
    df_dx = 4 * x[0] * (x[0]**2 + x[1] - 11) + 2 * (x[0] + x[1]**2 - 7)
    df_dy = 2 * (x[0]**2 + x[1] - 11) + 4 * x[1] * (x[0] + x[1]**2 - 7)
    return np.array([df_dx, df_dy])

def gsearch(f, interval, tol, max_iter=100):
    """
    Одномерный поиск с помощью метода золотого сечения.
    
    Параметры:
    f - функция одного переменного
    interval - список [a, b], задающий начальный интервал
    tol - точность поиска
    max_iter - максимальное количество итераций
    
    Возвращает:
    alpha - точка минимума на интервале
    """
    a, b = interval[0], interval[1]
    
    # Коэффициент золотого сечения
    phi = (np.sqrt(5) - 1) / 2  # ≈ 0.618
    
    # Начальные точки
    x1 = b - phi * (b - a)
    x2 = a + phi * (b - a)
    
    f1 = f(x1)
    f2 = f(x2)
    neval = 2  # Счетчик вычислений функции
    
    for _ in range(max_iter):
        if (b - a) < tol:
            break
            
        if f1 < f2:
            # Минимум в [a, x2]
            b = x2
            x2 = x1
            f2 = f1
            x1 = b - phi * (b - a)
            f1 = f(x1)
            neval += 1
        else:
            # Минимум в [x1, b]
            a = x1
            x1 = x2
            f1 = f2
            x2 = a + phi * (b - a)
            f2 = f(x2)
            neval += 1
    
    return (a + b) / 2, neval

def sdsearch(f, df, x0, tol, max_iter=1000):
    """
    Метод наискорейшего спуска с одномерной минимизацией золотым сечением.
    
    Параметры:
    f - функция для минимизации
    df - градиент функции
    x0 - начальная точка
    tol - точность
    max_iter - максимальное количество итераций
    
    Возвращает:
    xmin - точка минимума
    fmin - значение функции в минимуме
    neval - количество вычислений градиента
    coords - история точек
    alphas - история значений alpha (длина шага)
    """
    x = x0.copy()
    coords = [x.copy()]
    alphas = []
    
    neval_grad = 0  # Счетчик вычислений градиента
    neval_f = 0     # Счетчик вычислений функции
    
    k = 0
    
    while k < max_iter:
        # Вычисляем градиент
        g = df(x)
        neval_grad += 1
        
        # Функция для одномерной минимизации вдоль направления антиградиента
        f1dim = lambda a: f(x - a * g)
        
        # Одномерная минимизация методом золотого сечения
        alpha, f_eval = gsearch(f1dim, [0, 1], tol/10)
        neval_f += f_eval
        
        # Шаг наискорейшего спуска
        x_new = x - alpha * g
        
        # Сохраняем историю
        coords.append(x_new.copy())
        alphas.append(alpha)
        
        # Проверка сходимости
        delta_x = np.linalg.norm(x_new - x)
        
        if delta_x < tol:
            break
            
        # Обновляем точку
        x = x_new.copy()
        k += 1
    
    xmin = x
    fmin = f(xmin)
    neval_f += 1
    
    answer = {
        'xmin': xmin,
        'fmin': fmin,
        'neval_grad': neval_grad,
        'neval_f': neval_f,
        'total_neval': neval_grad + neval_f,
        'iterations': k,
        'coords': coords,
        'alphas': alphas
    }
    
    return answer

def contour_plot(ax, f, bounds=(-4, 4)):
    """
    Создание контурного графика функции.
    
    Параметры:
    ax - ось для рисования
    f - функция двух переменных
    bounds - границы по x и y
    """
    # Подготовка сетки
    x_range = np.arange(bounds[0], bounds[1] + 0.1, 0.1)
    y_range = np.arange(bounds[0], bounds[1] + 0.1, 0.1)
    xx, yy = np.meshgrid(x_range, y_range)
    
    # Вычисляем значения функции
    F = np.zeros_like(xx)
    for i in range(xx.shape[0]):
        for j in range(xx.shape[1]):
            F[i, j] = f([xx[i, j], yy[i, j]])
    
    # Рисуем контуры
    levels = np.logspace(0, 4, 20)  # Логарифмическая шкала
    contour = ax.contour(xx, yy, F, levels=levels, 
                        linewidths=0.8, colors='gray', alpha=0.7)
    ax.clabel(contour, inline=True, fontsize=8)
    
    # Отмечаем известные минимумы функции Химмельблау
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
    
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.grid(True, alpha=0.3)
    
    return contour

def steepest_descent_draw(ax, coords, alphas, show_text=True):
    """
    Визуализация траектории метода наискорейшего спуска.
    
    Параметры:
    ax - ось для рисования
    coords - список координат точек
    alphas - список длин шагов
    show_text - показывать ли номера итераций
    """
    f_size = 9
    
    # Рисуем траекторию
    for i in range(len(coords) - 1):
        x0 = coords[i]
        x1 = coords[i + 1]
        
        # Линия траектории
        line = ax.plot([x0[0], x1[0]], [x0[1], x1[1]], 
                      'b-', lw=2, alpha=0.8, zorder=10)[0]
        
        # Маркер шага
        ax.scatter(x0[0], x0[1], c='blue', s=40, marker='o', alpha=0.8, zorder=11)
        
        # Номер шага и длина шага
        if show_text and i < len(alphas):
            # Позиция для текста - середина отрезка
            mid_x = (x0[0] + x1[0]) / 2
            mid_y = (x0[1] + x1[1]) / 2
            
            # Смещение текста в перпендикулярном направлении
            dx = x1[0] - x0[0]
            dy = x1[1] - x0[1]
            length = np.sqrt(dx*dx + dy*dy)
            
            if length > 0.01:  # Не показываем текст для очень коротких шагов
                # Перпендикулярное направление
                perp_dx = -dy * 0.15 / length
                perp_dy = dx * 0.15 / length
                
                text_x = mid_x + perp_dx
                text_y = mid_y + perp_dy
                
                ax.text(text_x, text_y, f"{i}\nα={alphas[i]:.3f}", 
                       fontsize=f_size-1, ha='center', va='center',
                       bbox=dict(boxstyle="round,pad=0.2", 
                                facecolor='white', alpha=0.9))
    
    # Начальная точка
    start_point = coords[0]
    ax.scatter(start_point[0], start_point[1], c='green', s=120, 
              marker='s', zorder=20, label='Начальная точка')
    ax.text(start_point[0] + 0.15, start_point[1] + 0.15, 'Start', 
           fontsize=f_size+1, weight='bold', 
           bbox=dict(boxstyle="round,pad=0.3", facecolor='yellow', alpha=0.9))
    
    # Конечная точка
    end_point = coords[-1]
    ax.scatter(end_point[0], end_point[1], c='red', s=150, 
              marker='*', zorder=20, label='Конечная точка')
    ax.text(end_point[0] + 0.15, end_point[1] + 0.15, 'End', 
           fontsize=f_size+1, weight='bold', 
           bbox=dict(boxstyle="round,pad=0.3", facecolor='pink', alpha=0.9))
    
    # Добавляем легенду
    ax.legend(loc='best', fontsize=9)

def create_steepest_descent_visualization(result, bounds=(-4, 4), save_path=None):
    """
    Создание полной визуализации метода наискорейшего спуска.
    
    Параметры:
    result - результат работы sdsearch()
    bounds - границы для графиков
    save_path - путь для сохранения
    """
    coords = result['coords']
    alphas = result['alphas']
    
    fig = plt.figure(figsize=(16, 10))
    
    # Контурный график с траекторией
    ax1 = plt.subplot(2, 2, 1)
    contour_plot(ax1, himmelblau, bounds)
    steepest_descent_draw(ax1, coords, alphas, show_text=True)
    ax1.set_xlim(bounds[0], bounds[1])
    ax1.set_ylim(bounds[0], bounds[1])
    ax1.set_aspect('equal', adjustable='box')
    ax1.set_title(f'Траектория метода наискорейшего спуска\n'
                 f'Итераций: {result["iterations"]}, α вычисляется золотым сечением')
    
    # График сходимости функции
    ax2 = plt.subplot(2, 2, 2)
    f_values = [himmelblau(x) for x in coords]
    ax2.plot(range(len(coords)), f_values, 'b-', lw=2, marker='o', ms=4)
    ax2.set_xlabel('Номер итерации')
    ax2.set_ylabel('Значение функции f(x)')
    ax2.set_title('Сходимость значения функции')
    ax2.grid(True, alpha=0.3)
    ax2.set_yscale('log')
    
    # График изменения длины шага (alpha)
    ax3 = plt.subplot(2, 2, 3)
    ax3.plot(range(len(alphas)), alphas, 'r-', lw=2, marker='s', ms=4)
    ax3.set_xlabel('Номер итерации')
    ax3.set_ylabel('Длина шага (α)')
    ax3.set_title('Изменение длины шага в процессе оптимизации')
    ax3.grid(True, alpha=0.3)
    
    # График изменения нормы градиента
    ax4 = plt.subplot(2, 2, 4)
    grad_norms = [np.linalg.norm(himmelblau_grad(x)) for x in coords]
    ax4.plot(range(len(coords)), grad_norms, 'g-', lw=2, marker='^', ms=4)
    ax4.set_xlabel('Номер итерации')
    ax4.set_ylabel('Норма градиента ||∇f(x)||')
    ax4.set_title('Изменение нормы градиента')
    ax4.grid(True, alpha=0.3)
    ax4.set_yscale('log')
    
    plt.suptitle(f'Метод наискорейшего спуска для функции Химмельблау\n'
                f'Начальная точка: [{coords[0][0]:.2f}, {coords[0][1]:.2f}], '
                f'Конечная точка: [{coords[-1][0]:.3f}, {coords[-1][1]:.3f}], '
                f'f(x_min) = {result["fmin"]:.6f}', 
                fontsize=14)
    
    plt.tight_layout()
    
    # Сохраняем график
    if save_path:
        try:
            plt.savefig(save_path, dpi=150, bbox_inches='tight')
            print(f"✓ График сохранен: {os.path.abspath(save_path)}")
        except Exception as e:
            print(f"✗ Ошибка при сохранении графика: {e}")
            save_path = 'steepest_descent_plot.png'
            plt.savefig(save_path, dpi=150, bbox_inches='tight')
            print(f"✓ График сохранен как: {save_path}")
    else:
        save_path = 'steepest_descent_plot.png'
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        print(f"✓ График сохранен: {save_path}")
    
    plt.close(fig)
    return save_path

def print_iteration_info(result):
    """
    Вывод подробной информации о итерациях.
    """
    coords = result['coords']
    alphas = result['alphas']
    
    print("\n" + "=" * 80)
    print("ДЕТАЛЬНАЯ ИНФОРМАЦИЯ ОБ ИТЕРАЦИЯХ")
    print("=" * 80)
    
    print(f"\nВсего итераций: {result['iterations']}")
    print(f"Вычислений градиента: {result['neval_grad']}")
    print(f"Вычислений функции (включая 1D поиск): {result['neval_f']}")
    print(f"Всего вычислений: {result['total_neval']}")
    
    print("\nПервые 5 итераций:")
    print("Ит. |         x         |         y         |    f(x,y)    |    α    |  ||∇f||  ")
    print("-" * 85)
    
    for i in range(min(6, len(coords))):  # Показываем 6 точек = 5 итераций
        x = coords[i]
        f_val = himmelblau(x)
        grad_norm = np.linalg.norm(himmelblau_grad(x))
        
        if i == 0:
            print(f"{i:3d} | {x[0]:15.6f} | {x[1]:15.6f} | {f_val:12.6f} | {'---':>7} | {grad_norm:9.6f}")
        elif i <= len(alphas):
            print(f"{i:3d} | {x[0]:15.6f} | {x[1]:15.6f} | {f_val:12.6f} | {alphas[i-1]:7.4f} | {grad_norm:9.6f}")
    
    if len(coords) > 10:
        print("\n...")
        print("\nПоследние 3 итерации:")
        print("Ит. |         x         |         y         |    f(x,y)    |    α    |  ||∇f||  ")
        print("-" * 85)
        
        for i in range(max(0, len(coords)-3), len(coords)):
            x = coords[i]
            f_val = himmelblau(x)
            grad_norm = np.linalg.norm(himmelblau_grad(x))
            
            if i == 0:
                print(f"{i:3d} | {x[0]:15.6f} | {x[1]:15.6f} | {f_val:12.6f} | {'---':>7} | {grad_norm:9.6f}")
            elif i <= len(alphas):
                print(f"{i:3d} | {x[0]:15.6f} | {x[1]:15.6f} | {f_val:12.6f} | {alphas[i-1]:7.4f} | {grad_norm:9.6f}")
    
    print(f"\nНачальная точка:  [{coords[0][0]:.6f}, {coords[0][1]:.6f}]")
    print(f"Конечная точка:   [{coords[-1][0]:.6f}, {coords[-1][1]:.6f}]")
    print(f"Изменение координат: Δx = {coords[-1][0]-coords[0][0]:.6f}, Δy = {coords[-1][1]-coords[0][1]:.6f}")
    print(f"Длина пути: {sum(np.linalg.norm(coords[i+1]-coords[i]) for i in range(len(coords)-1)):.4f}")

if __name__ == "__main__":
    # Параметры оптимизации
    x0 = np.array([0.0, 0.0])  # Начальная точка
    tol = 1e-6  # Точность
    
    print("=" * 80)
    print("МЕТОД НАИСКОРЕЙШЕГО СПУСКА ДЛЯ ФУНКЦИИ ХИММЕЛЬБЛАУ")
    print("=" * 80)
    print("f(x,y) = (x² + y - 11)² + (x + y² - 7)²")
    print(f"Начальная точка: [{x0[0]}, {x0[1]}]")
    print(f"Точность: {tol}")
    print("\nЗапуск оптимизации...")
    
    # Запуск метода наискорейшего спуска
    result = sdsearch(himmelblau, himmelblau_grad, x0, tol, max_iter=1000)
    
    print(f"\n✓ ОПТИМИЗАЦИЯ ЗАВЕРШЕНА")
    print(f"  Найденный минимум: [{result['xmin'][0]:.6f}, {result['xmin'][1]:.6f}]")
    print(f"  Значение функции в минимуме: {result['fmin']:.10f}")
    print(f"  Количество итераций: {result['iterations']}")
    print(f"  Вычислений градиента: {result['neval_grad']}")
    print(f"  Вычислений функции: {result['neval_f']}")
    print(f"  Всего вычислений: {result['total_neval']}")
    
    # Выводим информацию об итерациях
    print_iteration_info(result)
    
    # Создаем визуализацию
    print("\nСоздание визуализации...")
    plot_path = create_steepest_descent_visualization(
        result, 
        bounds=(-4, 4), 
        save_path='17_steepest_descent_himmelblau.png'
    )
    
    # Выводим HTML для отображения
    print("\n" + "=" * 80)
    print("ВИЗУАЛИЗАЦИЯ РЕЗУЛЬТАТОВ")
    print("=" * 80)
    
    if os.path.exists(plot_path):
        print(f'<img width="900px" src="{plot_path}">')
    else:
        print("⚠ График не найден")
    
    print("\n" + "=" * 80)
    print("ВСЕ ГРАФИКИ СОХРАНЕНЫ В ТЕКУЩЕЙ ДИРЕКТОРИИ")
    print("=" * 80)
    
  