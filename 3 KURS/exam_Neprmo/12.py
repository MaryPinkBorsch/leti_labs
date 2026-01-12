import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import random


def f(x):
    return x**2 - 10 * np.cos(0.3 * np.pi * x) - 20

def df(x):
    return 2*x + 3* np.pi*np.sin(0.3 * np.pi * x)

def ddf(x):
    return 2 + 0.9* np.pi* np.pi*np.cos(0.3 * np.pi * x)


def nsearch(tol, x0, interval=None):
    if interval is None:
        # Определяем автоматический интервал для визуализации
        interval = [x0 - 3, x0 + 3]
    
    coords = [x0]
    df_values = [df(x0)]
    ddf_values = [ddf(x0)]
    neval = 0
    k = 1
    x_prev = x0
    
    while True:
        df_xprev = df(x_prev)
        ddf_xprev = ddf(x_prev)
        x = x_prev - (df_xprev/ddf_xprev)
        df_x = df(x)
        neval += 3
        
        coords.append(x)
        df_values.append(df_x)
        ddf_values.append(ddf(x))
        
        if ((np.abs(df_x)) < tol):
            xmin = x
            fmin = f(xmin)
            break
        else:
            k += 1
            x_prev = x
    
    answer_ = [xmin, fmin, neval, coords, df_values, ddf_values]
    print(f"xmin = {xmin:.10f}, Fmin = {fmin:.10f}")
    print(f"Количество вычислений: {neval}")
    print(f"Количество итераций: {k}")
    
    return answer_, interval


def draw_derivative_plot(a, b, ax, color, iteration=None):
    h = (b - a) / 200
    x_vals = np.arange(a, b + h, h)
    y_vals = [df(x) for x in x_vals]
    
    ax.plot(x_vals, y_vals, lw=1.5, c=color, label="f'(x)")
    ax.axhline(y=0, color='black', linestyle='-', alpha=0.3, linewidth=0.8)
    
    ax.set_xlabel('x')
    ax.set_ylabel("f'(x)")
    ax.grid(True, alpha=0.3)
    ax.set_xlim([a, b])
    
    # Автоматическое масштабирование Y
    y_min, y_max = min(y_vals), max(y_vals)
    padding = (y_max - y_min) * 0.1
    ax.set_ylim([y_min - padding, y_max + padding])
    
    return x_vals, y_vals


def draw_function_plot(a, b, ax, color):
    h = (b - a) / 200
    x_vals = np.arange(a, b + h, h)
    y_vals = [f(x) for x in x_vals]
    
    ax.plot(x_vals, y_vals, lw=1.5, c=color, label="f(x)")
    ax.set_xlabel('x')
    ax.set_ylabel("f(x)")
    ax.grid(True, alpha=0.3)
    ax.set_xlim([a, b])
    
    return x_vals, y_vals


def draw_newton_step(coord_prev, coord_new, ax_der, ax_func, iteration, a, b):
    # На графике производной
    x_tangent = np.linspace(a, b, 100)
    slope = ddf(coord_prev)
    intercept = df(coord_prev)
    y_tangent = slope * (x_tangent - coord_prev) + intercept
    
    # Касательная на графике производной
    ax_der.plot(x_tangent, y_tangent, 'r--', alpha=0.7, linewidth=1, 
               label=f'Касательная (итер {iteration})')
    
    # Старая точка
    ax_der.scatter(coord_prev, df(coord_prev), color='red', s=80, marker='o', zorder=5)
    ax_der.text(coord_prev, df(coord_prev), f'  x_{iteration-1}',
               fontsize=10, verticalalignment='bottom')
    
    # Новая точка (пересечение с осью X)
    ax_der.scatter(coord_new, 0, color='blue', s=100, marker='*', zorder=5)
    ax_der.plot([coord_new, coord_new], [0, df(coord_new)], 'b:', alpha=0.5)
    ax_der.text(coord_new, 0, f'  x_{iteration}',
               fontsize=10, verticalalignment='top')
    
    # На графике функции
    ax_func.scatter(coord_prev, f(coord_prev), color='red', s=80, marker='o', zorder=5)
    ax_func.text(coord_prev, f(coord_prev), f'  x_{iteration-1}',
                fontsize=10, verticalalignment='bottom')
    
    ax_func.scatter(coord_new, f(coord_new), color='blue', s=100, marker='*', zorder=5)
    ax_func.text(coord_new, f(coord_new), f'  x_{iteration}',
                fontsize=10, verticalalignment='top')
    
    # Соединяем точки на графике функции
    ax_func.plot([coord_prev, coord_new], [f(coord_prev), f(coord_new)], 
                'g-', alpha=0.5, linewidth=1)


def newton_visualization(answer, interval, tol):
    xmin, fmin, neval, coords, df_values, ddf_values = answer
    a, b = interval
    
    print("\n" + "=" * 80)
    print("ВИЗУАЛИЗАЦИЯ МЕТОДА НЬЮТОНА:")
    print("=" * 80)
    
    # Определяем оптимальные границы для графиков
    all_points = coords + [a, b]
    x_min_plot = min(all_points) - 0.5
    x_max_plot = max(all_points) + 0.5
    
    Nfigs = min(8, len(coords))  # максимум 8 графиков
    
    # Создаем отдельные графики для каждой итерации
    for i in range(1, Nfigs):
        if i >= len(coords):
            break
            
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=[12, 10])
        
        # Выбираем цвет
        color = (random.random() * 0.7, random.random() * 0.7, random.random() * 0.7)
        
        # График производной (верхний)
        x_vals, y_vals = draw_derivative_plot(x_min_plot, x_max_plot, ax1, color, i)
        ax1.set_title(f"Метод Ньютона — Итерация {i}")
        
        # График функции (нижний)
        draw_function_plot(x_min_plot, x_max_plot, ax2, color)
        
        # Рисуем шаг Ньютона
        if i > 0:
            draw_newton_step(coords[i-1], coords[i], ax1, ax2, i, x_min_plot, x_max_plot)
        
        # Добавляем информацию о шаге
        step_info = (f"x_{i-1} = {coords[i-1]:.6f}\n"
                    f"f'(x_{i-1}) = {df_values[i-1]:.6f}\n"
                    f"f''(x_{i-1}) = {ddf_values[i-1]:.6f}\n"
                    f"x_{i} = x_{i-1} - f'(x_{i-1})/f''(x_{i-1})\n"
                    f"     = {coords[i-1]:.6f} - {df_values[i-1]:.6f}/{ddf_values[i-1]:.6f}\n"
                    f"     = {coords[i]:.6f}")
        
        ax1.text(0.02, 0.98, step_info,
                transform=ax1.transAxes, verticalalignment='top',
                bbox=dict(boxstyle="round,pad=0.3", facecolor="lightyellow", alpha=0.8))
        
        plt.tight_layout()
        fname = f"newton_iter_{i}.png"
        plt.savefig(fname, dpi=100, bbox_inches='tight')
        plt.close()
        
        print(f'<img width="800px" src="/resources/{fname}">')
        print(f"Итерация {i}:")
        print(f"  x_{i-1} = {coords[i-1]:.8f}, f'(x_{i-1}) = {df_values[i-1]:.8f}, f''(x_{i-1}) = {ddf_values[i-1]:.8f}")
        print(f"  x_{i} = {coords[i]:.8f}, f'(x_{i}) = {df_values[i]:.8f}")
        print(f"  Δx = {abs(coords[i] - coords[i-1]):.10f}")
        print("-" * 70)
    
    # Финальный комбинированный график
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=[16, 12])
    
    # 1. Весь процесс на графике производной
    x_vals, y_vals = draw_derivative_plot(x_min_plot, x_max_plot, ax1, 'blue')
    ax1.set_title("Весь процесс на графике производной")
    
    # Отмечаем все точки
    for i in range(len(coords)-1):
        ax1.scatter(coords[i], df_values[i], color='red', s=60, alpha=0.7)
        ax1.text(coords[i], df_values[i], f' {i}', fontsize=9)
        
        # Показываем касательные
        if i < 5:  # Только первые 5 касательных для читаемости
            x_t = np.linspace(x_min_plot, x_max_plot, 50)
            y_t = ddf(coords[i]) * (x_t - coords[i]) + df_values[i]
            ax1.plot(x_t, y_t, '--', alpha=0.3, linewidth=0.8)
    
    ax1.scatter(coords[-1], df_values[-1], color='green', s=100, marker='*', label='Финальная точка')
    ax1.legend()
    
    # 2. Весь процесс на графике функции
    x_vals, y_vals = draw_function_plot(x_min_plot, x_max_plot, ax2, 'blue')
    ax2.set_title("Весь процесс на графике функции")
    
    for i in range(len(coords)-1):
        ax2.scatter(coords[i], f(coords[i]), color='red', s=60, alpha=0.7)
        ax2.text(coords[i], f(coords[i]), f' {i}', fontsize=9)
    
    ax2.scatter(coords[-1], f(coords[-1]), color='green', s=100, marker='*', label='Финальная точка')
    ax2.legend()
    
    # 3. Детали последней итерации
    if len(coords) >= 2:
        draw_derivative_plot(coords[-2] - 0.5, coords[-2] + 0.5, ax3, 'purple')
        ax3.set_title(f"Детали последней итерации")
        
        # Последний шаг Ньютона
        x_last = coords[-2]
        x_new = coords[-1]
        x_tangent = np.linspace(x_last - 0.5, x_last + 0.5, 50)
        y_tangent = ddf(x_last) * (x_tangent - x_last) + df(x_last)
        
        ax3.plot(x_tangent, y_tangent, 'r--', linewidth=2, label='Касательная')
        ax3.scatter([x_last, x_new], [df(x_last), 0], color=['red', 'blue'], s=100)
        ax3.text(x_last, df(x_last), f'x_{len(coords)-2}', fontsize=10)
        ax3.text(x_new, 0, f'x_{len(coords)-1}', fontsize=10)
        ax3.legend()
    
    # 4. Сходимость метода
    ax4.plot(range(len(coords)), coords, 'bo-', linewidth=2, markersize=6)
    ax4.axhline(y=xmin, color='green', linestyle='--', alpha=0.5, label=f'Предел: {xmin:.8f}')
    ax4.set_title("Сходимость последовательности x_k")
    ax4.set_xlabel('Номер итерации (k)')
    ax4.set_ylabel('x_k')
    ax4.grid(True, alpha=0.3)
    ax4.legend()
    
    # Добавляем таблицу с ошибками
    errors = [abs(coords[i] - xmin) for i in range(len(coords))]
    ax4_twin = ax4.twinx()
    ax4_twin.semilogy(range(len(errors)), errors, 'r--', alpha=0.7, label='Ошибка (лог)')
    ax4_twin.set_ylabel('|x_k - x*| (лог масштаб)')
    
    plt.tight_layout()
    plt.savefig("newton_final.png", dpi=100, bbox_inches='tight')
    plt.close()
    
    print("\n" + "=" * 80)
    print("ФИНАЛЬНЫЙ РЕЗУЛЬТАТ:")
    print("=" * 80)
    print('<img width="1000px" src="/resources/newton_final.png">')
    
    print(f"\nРезультат:")
    print(f"Найденный минимум: x = {xmin:.12f}, f(x) = {fmin:.12f}")
    print(f"f'(x_min) = {df(xmin):.12f} (должно быть близко к 0)")
    print(f"f''(x_min) = {ddf(xmin):.12f} (должно быть > 0 для минимума)")
    
    # Проверка условий оптимальности
    print(f"\nПроверка условий оптимальности:")
    print(f"|f'(x_min)| = {abs(df(xmin)):.2e} {'<' if abs(df(xmin)) < tol else '>='} tol = {tol}")
    print(f"f''(x_min) = {ddf(xmin):.6f} {'>' if ddf(xmin) > 0 else '<='} 0")
    
    # Таблица итераций
    print("\n" + "=" * 120)
    print("ТАБЛИЦА ИТЕРАЦИЙ МЕТОДА НЬЮТОНА:")
    print("=" * 120)
    print(f"{'k':<4} {'x_k':<15} {'f(x_k)':<15} {'f\'(x_k)':<15} {'f\'\'(x_k)':<15} {'Δx':<15} {'|f\'(x_k)|':<15}")
    print("-" * 120)
    
    for i in range(min(15, len(coords))):
        delta_x = abs(coords[i] - coords[i-1]) if i > 0 else 0
        print(f"{i:<4} {coords[i]:<15.8f} {f(coords[i]):<15.8f} "
              f"{df_values[i]:<15.8f} {ddf_values[i]:<15.8f} "
              f"{delta_x:<15.10f} {abs(df_values[i]):<15.10f}")
    
    if len(coords) > 15:
        print("... (показаны только первые 15 итераций)")
    print("=" * 120)


if __name__ == "__main__":
    print("=" * 80)
    print("МЕТОД НЬЮТОНА ДЛЯ ПОИСКА МИНИМУМА")
    print("=" * 80)
    print("Функция: f(x) = x² - 10·cos(0.3πx) - 20")
    print("Производная: f'(x) = 2x + 3π·sin(0.3πx)")
    print("Вторая производная: f''(x) = 2 + 0.9π²·cos(0.3πx)")
    print("Начальная точка: x0 = 1.3, Точность: 1e-8")
    print("=" * 80)
    
    result, interval = nsearch(0.00000001, 1.3, interval=[-2, 5])
    newton_visualization(result, interval, 0.00000001)