import numpy as np
import matplotlib.pyplot as plt
import os

def f(x):
    return x**2 - 10*np.cos(0.3*np.pi*x) - 20

def g(x):
    return 2*x + 3*np.pi*np.sin(0.3*np.pi*x)

def muller_simple(f, x0, x1, x2, tol=1e-6, max_iter=50):
    """
    Упрощенная и более устойчивая версия метода Мюллера
    """
    neval = 0
    coords = []  # Для визуализации: (x0, x1, x2, x_new)
    
    # Вычисляем значения функции
    f0 = f(x0); neval += 1
    f1 = f(x1); neval += 1
    f2 = f(x2); neval += 1
    
    coords.append([x0, x1, x2, None])
    
    for k in range(max_iter):
        # Сортируем точки по x для стабильности
        points = sorted([(x0, f0), (x1, f1), (x2, f2)], key=lambda p: p[0])
        x0, f0 = points[0]
        x1, f1 = points[1]
        x2, f2 = points[2]
        
        # Проверяем сходимость
        if abs(x2 - x0) < tol:
            break
        
        # Используем метод параболической интерполяции
        h1 = x1 - x0
        h2 = x2 - x1
        
        if abs(h1) < 1e-12 or abs(h2) < 1e-12:
            # Точки слишком близки
            x_new = (x0 + x2) / 2.0
        else:
            # Разности первого порядка
            f01 = (f1 - f0) / h1
            f12 = (f2 - f1) / h2
            
            # Вторая разность (пропорциональна коэффициенту a)
            f012 = (f12 - f01) / (x2 - x0)
            
            # Коэффициенты параболы
            a = f012
            b = f01 - a * (x0 + x1)
            c = f0 - a * x0**2 - b * x0
            
            # Находим вершину параболы
            if abs(a) > 1e-12:
                x_new = -b / (2 * a)
                # Проверяем, что точка внутри интервала
                if x_new < min(x0, x1, x2) or x_new > max(x0, x1, x2):
                    # Если вершина вне интервала, используем смещение
                    x_new = x1 + (x2 - x1) / 1.618
            else:
                # Парабола вырождается в прямую
                if abs(b) > 1e-12:
                    x_new = -c / b
                else:
                    x_new = (x0 + x2) / 2.0
        
        # Вычисляем значение в новой точке
        f_new = f(x_new); neval += 1
        
        # Обновляем точки: заменяем худшую точку
        points = [(x0, f0), (x1, f1), (x2, f2), (x_new, f_new)]
        worst_idx = np.argmax([f0, f1, f2, f_new])
        
        if worst_idx < 3:  # Если худшая точка среди трех старых
            if worst_idx == 0:
                x0, f0 = x_new, f_new
            elif worst_idx == 1:
                x1, f1 = x_new, f_new
            else:
                x2, f2 = x_new, f_new
        
        coords.append([x0, x1, x2, x_new])
        
        # Проверка сходимости
        f_values = [f0, f1, f2]
        if max(f_values) - min(f_values) < tol:
            break
    
    # Находим точку с минимальным значением функции
    points = [(x0, f0), (x1, f1), (x2, f2)]
    xmin, fmin = min(points, key=lambda p: p[1])
    
    answer_ = [xmin, fmin, neval, coords]
    return answer_

def save_iteration_plot(x0, x1, x2, x_new, k, folder_path):
    """
    Сохраняет графики для текущей итерации
    """
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)
    
    xg = np.linspace(-2, 5, 600)
    
    def lagrange_parabola(x):
        """
        Парабола, построенная по трем точкам (x0,g(x0)), (x1,g(x1)), (x2,g(x2))
        """
        # Интерполяционная формула Лагранжа
        L0 = ((x - x1)*(x - x2)) / ((x0 - x1)*(x0 - x2))
        L1 = ((x - x0)*(x - x2)) / ((x1 - x0)*(x1 - x2))
        L2 = ((x - x0)*(x - x1)) / ((x2 - x0)*(x2 - x1))
        return g(x0)*L0 + g(x1)*L1 + g(x2)*L2
    
    # Область для построения параболы
    xp = np.linspace(min(x0, x1, x2) - 0.5, max(x0, x1, x2) + 0.5, 300)
    parabola = lagrange_parabola(xp)
    
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    
    # Левый график: производная и парабола
    axes[0].plot(xg, g(xg), label="g(x) = f'(x)", linewidth=2)
    axes[0].scatter([x0, x1, x2], [g(x0), g(x1), g(x2)], 
                   color="red", s=80, label="Опорные точки")
    axes[0].plot(xp, parabola, linestyle="--", 
                color="green", linewidth=2, label="Парабола Мюллера")
    
    if x_new is not None:
        axes[0].scatter(x_new, 0, color="black", s=100, 
                       zorder=5, label=r"$x_{k+1}$")
    
    axes[0].axhline(0, color="gray", linewidth=0.8)
    axes[0].set_title(f"g(x) = f'(x), итерация k={k}")
    axes[0].legend(loc='best')
    axes[0].grid(True, alpha=0.3)
    axes[0].set_xlabel("x")
    axes[0].set_ylabel("g(x)")
    
    # Правый график: функция f(x)
    axes[1].plot(xg, f(xg), label="f(x)", linewidth=2, color="blue")
    
    # Отмечаем точки на графике функции
    axes[1].scatter([x0, x1, x2], [f(x0), f(x1), f(x2)], 
                   color="red", s=80, label="Точки параболы")
    
    if x_new is not None:
        axes[1].scatter(x_new, f(x_new), color="black", s=100, 
                       zorder=5, label=r"$x_{k+1}$")
    
    axes[1].set_title("f(x)")
    axes[1].legend(loc='best')
    axes[1].grid(True, alpha=0.3)
    axes[1].set_xlabel("x")
    axes[1].set_ylabel("f(x)")
    
    plt.tight_layout()
    
    # Сохраняем график
    filename = os.path.join(folder_path, f"14_muller_iteration_{k:02d}.png")
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    plt.close()
    
    return filename

if __name__ == "__main__":
    save_folder = r"/home/masha/leti_labs/3 KURS/exam_Neprmo"
    
    # Используем ту же функцию для поиска минимума f(x)
    # Ваш код ищет корни g(x) = f'(x), что эквивалентно поиску минимума f(x)
    print("Поиск минимума функции f(x) = x^2 - 10*cos(0.3πx) - 20")
    print("(эквивалентно поиску корней g(x) = f'(x))\n")
    
    # Начальные точки (такие же как в оригинале)
    x0, x1, x2 = 5.0, -2.0, 1.5
    
    # Запускаем упрощенный метод Мюллера для f(x)
    answer_ = muller_simple(f, x0, x1, x2, tol=1e-8, max_iter=50)
    xmin, fmin, neval, coords = answer_
    
    print(f"Результаты:")
    print(f"x_min = {xmin:.10f}")
    print(f"f(x_min) = {fmin:.10f}")
    print(f"Количество вычислений функции: {neval}")
    print(f"Количество итераций: {len(coords) - 1}\n")
    
    print("Промежуточные точки (x0, x1, x2, x_new):")
    for k, (x0k, x1k, x2k, x_newk) in enumerate(coords):
        print(f"k={k:2d}: x0={x0k:10.6f}, x1={x1k:10.6f}, "
              f"x2={x2k:10.6f}, x_new={str(x_newk) if x_newk is not None else 'None':>15}")
        
        # Сохраняем график для каждой итерации (кроме начальной без x_new)
        if x_newk is not None:
            save_iteration_plot(x0k, x1k, x2k, x_newk, k, save_folder)
    
    print(f"\nГрафики сохранены в папке: {save_folder}")
    
    # Дополнительная проверка: вычисляем производную в найденной точке
    g_xmin = g(xmin)
    print(f"\nПроверка: g(x_min) = f'(x_min) = {g_xmin:.10e}")
    print(f"|f'(x_min)| < 1e-6: {abs(g_xmin) < 1e-6}")