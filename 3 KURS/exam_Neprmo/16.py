import numpy as np
import matplotlib

matplotlib.use('Agg')
import matplotlib.pyplot as plt
import os

def f(X):
    x = X[0]
    y = X[1]
    v = (x**2 + y - 11)**2 + (x + y**2 - 7)**2
    return v
   
def df(X):
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = 2 * (x**2 + y - 11) * (2 * x) + 2 * (x + y**2 - 7)
    v[1] = 2 * (x**2 + y - 11) + 2 * (x + y**2 - 7) * (2 * y)
    return v

def gradient_search(x0, tol, alpha=0.01, max_iter=1000):
    """
    Градиентный спуск для минимизации функции
    """
    k = 0
    x_prev = np.array(x0, dtype=float)
    coords = [x_prev.copy()]
    f_values = [f(x_prev)]
    neval = 1  # f(x0) уже посчитана
    
    while k < max_iter:
        # Вычисляем антиградиент
        grad = df(x_prev)
        neval += 1
        
        # Шаг градиентного спуска
        x_new = x_prev - alpha * grad
        
        # Сохраняем координаты и значения функции
        coords.append(x_new.copy())
        f_values.append(f(x_new))
        neval += 1
        
        # Проверяем условие остановки
        delta_x = np.linalg.norm(x_new - x_prev)
        
        if delta_x < tol:
            break
            
        # Обновляем точку
        x_prev = x_new
        k += 1
    
    xmin = coords[-1]
    fmin = f_values[-1]
    
    return {
        'xmin': xmin,
        'fmin': fmin,
        'iterations': k,
        'neval': neval,
        'coords': coords,
        'f_values': f_values
    }

def contour_plot(ax, bounds=(-4, 4)):
    """
    Создание контурного графика функции Химмельблау
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
    levels = np.logspace(0, 4, 20)  # Логарифмическая шкала для лучшей видимости
    contour = ax.contour(xx, yy, F, levels=levels, linewidths=0.8, 
                        colors='gray', alpha=0.7)
    
    # Добавляем контурные метки
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

def gradient_draw(ax, coords, alpha):
    """
    Визуализация траектории градиентного спуска
    """
    f_size = 10
    
    # Рисуем траекторию
    for i in range(len(coords) - 1):
        x0 = coords[i]
        x1 = coords[i + 1]
        
        # Линия траектории
        ax.plot([x0[0], x1[0]], [x0[1], x1[1]], 'b-', lw=1.5, alpha=0.7)
        
        # Маркер шага
        ax.scatter(x0[0], x0[1], c='blue', s=30, marker='o', alpha=0.7)
        
        # Номер шага (подписываем не все, чтобы не загромождать)
        if i == 0 or i == len(coords)-2 or i % max(1, (len(coords)-1)//5) == 0:
            ax.text(x0[0] + 0.05, x0[1] + 0.05, str(i), 
                   fontsize=f_size-2, alpha=0.7, bbox=dict(boxstyle="round,pad=0.2", 
                                                           facecolor='white', alpha=0.7))
    
    # Начальная точка
    start_point = coords[0]
    ax.scatter(start_point[0], start_point[1], c='green', s=100, 
              marker='s', zorder=10, label='Начальная точка')
    ax.text(start_point[0] + 0.1, start_point[1] + 0.1, 'Start', 
           fontsize=f_size, weight='bold', bbox=dict(boxstyle="round,pad=0.3", 
                                                     facecolor='yellow', alpha=0.7))
    
    # Конечная точка
    end_point = coords[-1]
    ax.scatter(end_point[0], end_point[1], c='red', s=100, 
              marker='*', zorder=10, label='Конечная точка')
    ax.text(end_point[0] + 0.1, end_point[1] + 0.1, 'End', 
           fontsize=f_size, weight='bold', bbox=dict(boxstyle="round,pad=0.3", 
                                                     facecolor='pink', alpha=0.7))
    
    # Добавляем легенду
    ax.legend(loc='best', fontsize=9)

def create_visualization(coords, alpha, save_path='gradient_descent_himmelblau.png', bounds=(-4, 4)):
    """
    Создание полной визуализации градиентного спуска
    """
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7))
    fig.suptitle(f'Градиентный спуск для функции Химмельблау\n'
                f'α = {alpha}, шагов: {len(coords)-1}', fontsize=14)
    
    # Левый график: контурный график с траекторией
    contour_plot(ax1, bounds)
    gradient_draw(ax1, coords, alpha)
    ax1.set_xlim(bounds[0], bounds[1])
    ax1.set_ylim(bounds[0], bounds[1])
    ax1.set_aspect('equal', adjustable='box')
    ax1.set_title('Траектория градиентного спуска на контурном графике')
    
    # Правый график: сходимость функции
    f_values = [f(x) for x in coords]
    ax2.plot(range(len(coords)), f_values, 'b-', lw=2, marker='o', ms=4)
    ax2.set_xlabel('Номер итерации')
    ax2.set_ylabel('Значение функции f(x)')
    ax2.set_title('Сходимость значения функции')
    ax2.grid(True, alpha=0.3)
    ax2.set_yscale('log')  # Логарифмическая шкала для лучшей видимости
    
    # Добавляем сетку и настраиваем внешний вид
    for ax in [ax1, ax2]:
        ax.grid(True, alpha=0.3)
    
    plt.tight_layout()
    
    # Сохраняем график
    try:
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        print(f"✓ График сохранен: {os.path.abspath(save_path)}")
    except Exception as e:
        print(f"✗ Ошибка при сохранении графика: {e}")
        # Сохраняем в текущую директорию с другим именем
        alt_path = 'gradient_plot.png'
        plt.savefig(alt_path, dpi=150, bbox_inches='tight')
        print(f"✓ График сохранен как: {alt_path}")
        save_path = alt_path
    
    plt.close(fig)
    return save_path

def compare_learning_rates(x0, tol, max_iter=500):
    """
    Сравнение разных скоростей обучения
    """
    learning_rates = [0.001, 0.005, 0.01, 0.02, 0.05]
    
    fig, axes = plt.subplots(2, 3, figsize=(15, 10))
    fig.suptitle('Сравнение градиентного спуска с разными скоростями обучения', 
                fontsize=16)
    
    axes = axes.flatten()
    
    for idx, alpha in enumerate(learning_rates):
        if idx >= len(axes):
            break
            
        ax = axes[idx]
        
        # Запускаем градиентный спуск
        result = gradient_search(x0, tol, alpha=alpha, max_iter=max_iter)
        
        # Контурный график
        contour_plot(ax, bounds=(-4, 4))
        
        # Рисуем траекторию
        for i in range(len(result['coords']) - 1):
            x0_pt = result['coords'][i]
            x1_pt = result['coords'][i + 1]
            ax.plot([x0_pt[0], x1_pt[0]], [x0_pt[1], x1_pt[1]], 'b-', lw=1.5, alpha=0.7)
        
        # Начальная и конечная точки
        ax.scatter(result['coords'][0][0], result['coords'][0][1], 
                  c='green', s=80, marker='s', label='Start')
        ax.scatter(result['coords'][-1][0], result['coords'][-1][1], 
                  c='red', s=80, marker='*', label='End')
        
        ax.set_xlim(-4, 4)
        ax.set_ylim(-4, 4)
        ax.set_aspect('equal', adjustable='box')
        ax.set_title(f'α = {alpha}\nИтераций: {result["iterations"]}\n'
                    f'f(x) = {result["fmin"]:.2e}')
        ax.legend(fontsize=8)
    
    # Скрываем лишние оси
    for idx in range(len(learning_rates), len(axes)):
        axes[idx].set_visible(False)
    
    plt.tight_layout()
    
    save_path = 'learning_rate_comparison.png'
    try:
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        print(f"✓ График сравнения сохранен: {os.path.abspath(save_path)}")
    except Exception as e:
        print(f"✗ Ошибка при сохранении графика сравнения: {e}")
    
    plt.close(fig)
    return save_path

def print_trajectory_info(coords):
    """
    Вывод информации о траектории
    """
    print("\n" + "=" * 70)
    print("ТРАЕКТОРИЯ ГРАДИЕНТНОГО СПУСКА")
    print("=" * 70)
    
    print(f"Всего шагов: {len(coords)-1}")
    print(f"Длина пути: {sum(np.linalg.norm(coords[i+1]-coords[i]) for i in range(len(coords)-1)):.4f}")
    
    print("\nПервые 5 шагов:")
    for i, point in enumerate(coords[:6]):  # Берем 6 точек = 5 шагов
        print(f"Шаг {i}: x = [{point[0]:.6f}, {point[1]:.6f}], f(x) = {f(point):.6f}")
    
    if len(coords) > 10:
        print("\n...")
        print("Последние 3 шага:")
        for i in range(len(coords)-3, len(coords)):
            point = coords[i]
            print(f"Шаг {i}: x = [{point[0]:.6f}, {point[1]:.6f}], f(x) = {f(point):.6f}")
    
    print(f"\nНачальная точка: [{coords[0][0]:.6f}, {coords[0][1]:.6f}]")
    print(f"Конечная точка:  [{coords[-1][0]:.6f}, {coords[-1][1]:.6f}]")
    print(f"Изменение координат: [{coords[-1][0]-coords[0][0]:.6f}, {coords[-1][1]-coords[0][1]:.6f}]")

if __name__ == "__main__":
    # Начальная точка
    x0 = [2, 3]
    tol = 1e-4
    alpha = 0.01
    
    print("=" * 70)
    print("ГРАДИЕНТНЫЙ СПУСК ДЛЯ ФУНКЦИИ ХИММЕЛЬБЛАУ")
    print("f(x,y) = (x² + y - 11)² + (x + y² - 7)²")
    print("=" * 70)
    
    # Запускаем градиентный спуск
    print("\nЗапуск градиентного спуска...")
    result = gradient_search(x0, tol, alpha=alpha, max_iter=1000)
    
    print(f"\n✓ ОПТИМИЗАЦИЯ ЗАВЕРШЕНА")
    print(f"  Начальная точка: [{x0[0]}, {x0[1]}]")
    print(f"  Найденный минимум: [{result['xmin'][0]:.6f}, {result['xmin'][1]:.6f}]")
    print(f"  Значение функции в минимуме: {result['fmin']:.10f}")
    print(f"  Количество итераций: {result['iterations']}")
    print(f"  Количество вычислений функции: {result['neval']}")
    print(f"  Скорость обучения (α): {alpha}")
    print(f"  Точность (tol): {tol}")
    
    # Создаем визуализацию
    print("\nСоздание визуализации...")
    main_plot_path = create_visualization(
        result['coords'], 
        alpha, 
        save_path='16_gradient_descent_himmelblau.png',
        bounds=(-4, 4)
    )
    
    # Выводим информацию о траектории
    print_trajectory_info(result['coords'])
    
    # Выводим HTML для отображения
    print("\n" + "=" * 70)
    print("ВИЗУАЛИЗАЦИЯ РЕЗУЛЬТАТОВ")
    print("=" * 70)
    
    if os.path.exists(main_plot_path):
        print(f'<img width="900px" src="{main_plot_path}">')
    else:
        print("⚠ Основной график не найден")
    
    # Дополнительно: сравнение разных скоростей обучения
    print("\nСоздание графика сравнения разных скоростей обучения...")
    compare_path = compare_learning_rates(x0, tol)
    
    if os.path.exists(compare_path):
        print(f'<img width="900px" src="{compare_path}">')
    else:
        print("⚠ График сравнения не найден")
    
    print("\n" + "=" * 70)
    print("ВСЕ ГРАФИКИ СОХРАНЕНЫ В ТЕКУЩЕЙ ДИРЕКТОРИИ")
    print("=" * 70)
    
    # # Показываем список созданных файлов
    # current_dir = os.getcwd()
    # print(f"\nТекущая директория: {current_dir}")
    # print("\nСозданные файлы:")
    # for file in os.listdir(current_dir):
    #     if file.endswith('.png'):
    #         file_path = os.path.join(current_dir, file)
    #         size = os.path.getsize(file_path) / 1024  # в КБ
    #         print(f"  • {file} ({size:.1f} KB)")