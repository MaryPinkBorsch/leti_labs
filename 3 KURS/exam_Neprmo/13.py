import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import random


def f(x):
    return x**2 - 10 * np.cos(0.3 * np.pi * x) - 20

def df(x):
    return 2*x + 3* np.pi*np.sin(0.3 * np.pi * x)


def ssearch(interval, tol):
    k_max = 1000
    k = 0
    a, b = interval
    history = []  # Будем хранить полную историю итераций
    df_A = df(a)
    df_B = df(b)
    
    # Начальная точка
    x = b - df_B * (b - a) / (df_B - df_A)
    neval = 2
    
    # Сохраняем начальное состояние
    history.append({
        "iteration": 0,
        "a": a,
        "b": b,
        "x": x,
        "df_a": df_A,
        "df_b": df_B,
        "df_x": df(x),
        "step_type": "initial"
    })
    
    while (np.abs(df_B) > tol) and (np.abs(b - a) > tol) and (k < k_max):
        k += 1
        
        # Вычисляем новую точку по методу секущих
        x = b - df_B * (b - a) / (df_B - df_A)
        df_X = df(x)
        neval += 1
        
        # Обновляем границы
        if df_X >= 0:
            b = x
            df_B = df_X
            update = "b = x (df(x) ≥ 0)"
        else:
            a = x
            df_A = df_X
            update = "a = x (df(x) < 0)"
        
        # Сохраняем текущее состояние
        history.append({
            "iteration": k,
            "a": a,
            "b": b,
            "x": x,
            "df_a": df_A,
            "df_b": df_B,
            "df_x": df_X,
            "update": update,
            "secant_slope": (df_B - df_A) / (b - a) if b != a else None
        })
    
    xmin = x
    fmin = f(xmin)
    neval += 1
    
    # Сохраняем финальное состояние
    history.append({
        "iteration": k + 1,
        "a": a,
        "b": b,
        "x": xmin,
        "df_a": df_A,
        "df_b": df_B,
        "df_x": df(xmin),
        "fmin": fmin,
        "is_final": True,
        "converged": np.abs(df(xmin)) <= tol
    })
    
    print(f"xmin = {xmin:.10f}, Fmin = {fmin:.10f}")
    print(f"Количество вычислений: {neval}")
    print(f"Количество итераций: {k}")
    
    answer_ = [xmin, fmin, neval, history]
    return answer_


def draw_derivative_with_secant(a, b, x_prev1, x_prev2, x_new, df_prev1, df_prev2, ax, 
                               iteration, interval_for_plot=None):
    """
    Рисует график производной и секущую
    """
    if interval_for_plot is None:
        # Автоматически определяем интервал для отображения
        all_points = [a, b, x_prev1, x_prev2, x_new]
        x_min_plot = min(all_points) - 0.5
        x_max_plot = max(all_points) + 0.5
    else:
        x_min_plot, x_max_plot = interval_for_plot
    
    h = (x_max_plot - x_min_plot) / 200
    x_vals = np.arange(x_min_plot, x_max_plot + h, h)
    df_vals = [df(x) for x in x_vals]
    
    # График производной
    color_der = (0.2, 0.4, 0.8)
    ax.plot(x_vals, df_vals, lw=2, c=color_der, label="f'(x)")
    ax.axhline(y=0, color='black', linestyle='-', alpha=0.3, linewidth=0.8)
    
    # Точки, через которые проводится секущая
    ax.scatter([x_prev1, x_prev2], [df_prev1, df_prev2], 
              color=['red', 'orange'], s=100, zorder=5)
    ax.text(x_prev1, df_prev1, f' x_{iteration-2}', 
           fontsize=10, verticalalignment='bottom')
    ax.text(x_prev2, df_prev2, f' x_{iteration-1}', 
           fontsize=10, verticalalignment='bottom')
    
    # Секущая линия
    if x_prev2 != x_prev1:
        # Уравнение секущей: y = m*(x - x_prev2) + df_prev2
        m = (df_prev2 - df_prev1) / (x_prev2 - x_prev1)
        secant_x = np.linspace(x_min_plot, x_max_plot, 100)
        secant_y = m * (secant_x - x_prev2) + df_prev2
        
        ax.plot(secant_x, secant_y, 'g--', lw=2, alpha=0.8, label='Секущая')
        
        # Показываем пересечение с осью X
        if m != 0:
            x_zero = x_prev2 - df_prev2 / m
            ax.scatter(x_zero, 0, color='blue', s=150, marker='*', zorder=6)
            ax.text(x_zero, 0, f' x_{iteration}', 
                   fontsize=10, verticalalignment='top')
            
            # Вертикальная линия к новой точке
            ax.plot([x_zero, x_zero], [0, df(x_zero)], 'b:', alpha=0.5)
    
    # Новая точка на графике производной
    if iteration > 0:
        df_new = df(x_new)
        ax.scatter(x_new, df_new, color='purple', s=100, marker='s', zorder=5)
    
    ax.set_xlabel('x')
    ax.set_ylabel("f'(x)")
    ax.set_title(f"Метод секущих — Итерация {iteration}")
    ax.grid(True, alpha=0.3)
    ax.legend(loc='best')
    
    # Автоматическое масштабирование Y
    y_vals = df_vals + [df_prev1, df_prev2, df(x_new) if iteration > 0 else 0]
    y_min, y_max = min(y_vals), max(y_vals)
    padding = (y_max - y_min) * 0.1
    ax.set_ylim([y_min - padding, y_max + padding])
    
    return x_min_plot, x_max_plot


def draw_function_with_points(a, b, x_prev1, x_prev2, x_new, ax, iteration):
    """
    Рисует график функции с отмеченными точками
    """
    x_min_plot, x_max_plot = a - 0.5, b + 0.5
    h = (x_max_plot - x_min_plot) / 200
    x_vals = np.arange(x_min_plot, x_max_plot + h, h)
    f_vals = [f(x) for x in x_vals]
    
    # График функции
    color_func = (0.8, 0.2, 0.4)
    ax.plot(x_vals, f_vals, lw=2, c=color_func, label="f(x)")
    
    # Отмечаем точки
    ax.scatter([x_prev1, x_prev2, x_new], 
              [f(x_prev1), f(x_prev2), f(x_new)], 
              color=['red', 'orange', 'purple'], s=80, zorder=5)
    
    ax.text(x_prev1, f(x_prev1), f' x_{iteration-2}', 
           fontsize=10, verticalalignment='bottom')
    ax.text(x_prev2, f(x_prev2), f' x_{iteration-1}', 
           fontsize=10, verticalalignment='bottom')
    ax.text(x_new, f(x_new), f' x_{iteration}', 
           fontsize=10, verticalalignment='top')
    
    ax.set_xlabel('x')
    ax.set_ylabel("f(x)")
    ax.set_title(f"Соответствующие точки на графике функции")
    ax.grid(True, alpha=0.3)
    ax.legend(loc='best')


def secant_method_visualization(answer, initial_interval):
    xmin, fmin, neval, history = answer
    
    print("\n" + "=" * 80)
    print("ВИЗУАЛИЗАЦИЯ МЕТОДА СЕКУЩИХ:")
    print("=" * 80)
    
    # Определяем общие границы для всех графиков
    all_x = [h["a"] for h in history] + [h["b"] for h in history] + [h["x"] for h in history]
    x_min_plot = min(all_x) - 1
    x_max_plot = max(all_x) + 1
    
    Nfigs = min(8, len(history) - 1)  # максимум 8 графиков (исключая финальный)
    
    # Создаем отдельные графики для каждой итерации
    for i in range(1, Nfigs + 1):
        if i >= len(history):
            break
            
        h_prev = history[i-1] if i > 0 else history[0]
        h_curr = history[i]
        
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=[12, 10])
        
        # Для первых двух итераций нужны специальные обработки
        if i == 1:
            # Первая итерация: используем a и b как предыдущие точки
            x_prev1, x_prev2 = h_curr["a"], h_curr["b"]
            df_prev1, df_prev2 = h_curr["df_a"], h_curr["df_b"]
        else:
            # Последующие итерации: используем предыдущие точки из истории
            x_prev1 = history[i-2]["x"] if i >= 2 else history[i-1]["a"]
            x_prev2 = history[i-1]["x"]
            df_prev1 = df(x_prev1)
            df_prev2 = df(x_prev2)
        
        # Верхний график: производная и секущая
        x_min_g, x_max_g = draw_derivative_with_secant(
            h_curr["a"], h_curr["b"], 
            x_prev1, x_prev2, h_curr["x"],
            df_prev1, df_prev2, ax1, i,
            interval_for_plot=(x_min_plot, x_max_plot)
        )
        
        # Добавляем информацию о шаге
        if x_prev2 != x_prev1:
            m = (df_prev2 - df_prev1) / (x_prev2 - x_prev1)
            step_info = (f"Точки: x_{i-2}={x_prev1:.6f}, x_{i-1}={x_prev2:.6f}\n"
                        f"Значения: f'(x_{i-2})={df_prev1:.6f}, f'(x_{i-1})={df_prev2:.6f}\n"
                        f"Наклон секущей: m = (f'(x_{i-1}) - f'(x_{i-2}))/(x_{i-1} - x_{i-2})\n"
                        f"             = ({df_prev2:.6f} - {df_prev1:.6f})/({x_prev2:.6f} - {x_prev1:.6f})\n"
                        f"             = {m:.6f}\n"
                        f"Новая точка: x_{i} = x_{i-1} - f'(x_{i-1})/m\n"
                        f"               = {x_prev2:.6f} - {df_prev2:.6f}/{m:.6f}\n"
                        f"               = {h_curr['x']:.6f}")
        else:
            step_info = "Точки совпадают, наклон не определен"
        
        ax1.text(0.02, 0.98, step_info,
                transform=ax1.transAxes, verticalalignment='top',
                bbox=dict(boxstyle="round,pad=0.3", facecolor="lightyellow", alpha=0.8))
        
        # Нижний график: функция
        draw_function_with_points(
            h_curr["a"], h_curr["b"], 
            x_prev1, x_prev2, h_curr["x"], ax2, i
        )
        
        plt.tight_layout()
        fname = f"secant_iter_{i}.png"
        plt.savefig(fname, dpi=100, bbox_inches='tight')
        plt.close()
        
        print(f'<img width="800px" src="/resources/{fname}">')
        print(f"Итерация {i}:")
        print(f"  Интервал: [{h_curr['a']:.8f}, {h_curr['b']:.8f}]")
        print(f"  Предыдущие точки: x_{i-2}={x_prev1:.8f}, x_{i-1}={x_prev2:.8f}")
        print(f"  Новая точка: x_{i}={h_curr['x']:.8f}, f'(x_{i})={h_curr['df_x']:.8f}")
        if 'update' in h_curr:
            print(f"  Обновление: {h_curr['update']}")
        print("-" * 70)
    
    # Финальный комбинированный график
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=[16, 12])
    
    # 1. Весь процесс на графике производной
    x_vals = np.linspace(x_min_plot, x_max_plot, 400)
    df_vals = [df(x) for x in x_vals]
    ax1.plot(x_vals, df_vals, 'b-', lw=2, label="f'(x)")
    ax1.axhline(y=0, color='black', linestyle='-', alpha=0.3)
    
    # Отмечаем все точки
    for i, h in enumerate(history[:-1]):  # Исключаем финальный
        if i < 10:  # Показываем только первые 10 точек для читаемости
            ax1.scatter(h["x"], h["df_x"], color='red', s=60, alpha=0.7)
            ax1.text(h["x"], h["df_x"], f' {i}', fontsize=9)
    
    ax1.scatter(history[-1]["x"], history[-1]["df_x"], 
               color='green', s=150, marker='*', label='Финальная точка')
    ax1.set_title("Весь процесс на графике производной")
    ax1.set_xlabel('x')
    ax1.set_ylabel("f'(x)")
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # 2. Весь процесс на графике функции
    f_vals = [f(x) for x in x_vals]
    ax2.plot(x_vals, f_vals, 'b-', lw=2, label="f(x)")
    
    for i, h in enumerate(history[:-1]):
        if i < 10:
            ax2.scatter(h["x"], f(h["x"]), color='red', s=60, alpha=0.7)
            ax2.text(h["x"], f(h["x"]), f' {i}', fontsize=9)
    
    ax2.scatter(history[-1]["x"], history[-1]["fmin"], 
               color='green', s=150, marker='*', label='Финальная точка')
    ax2.set_title("Весь процесс на графике функции")
    ax2.set_xlabel('x')
    ax2.set_ylabel("f(x)")
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    # 3. Детали последней итерации
    if len(history) >= 3:
        last_h = history[-2]  # Предпоследняя итерация
        prev1_h = history[-3] if len(history) >= 3 else history[-2]
        
        # Узкий интервал вокруг последних точек
        last_points = [last_h["x"], prev1_h["x"], history[-1]["x"]]
        x_min_last = min(last_points) - 0.2
        x_max_last = max(last_points) + 0.2
        
        x_last = np.linspace(x_min_last, x_max_last, 200)
        df_last = [df(x) for x in x_last]
        
        ax3.plot(x_last, df_last, 'b-', lw=2)
        ax3.axhline(y=0, color='black', linestyle='-', alpha=0.3)
        
        # Секущая
        m_last = (last_h["df_x"] - prev1_h["df_x"]) / (last_h["x"] - prev1_h["x"])
        secant_x = np.linspace(x_min_last, x_max_last, 100)
        secant_y = m_last * (secant_x - last_h["x"]) + last_h["df_x"]
        
        ax3.plot(secant_x, secant_y, 'g--', lw=2, alpha=0.8, label='Секущая')
        ax3.scatter([prev1_h["x"], last_h["x"]], 
                   [prev1_h["df_x"], last_h["df_x"]], 
                   color=['red', 'orange'], s=100)
        ax3.scatter(history[-1]["x"], 0, color='blue', s=150, marker='*')
        
        ax3.set_title("Детали последней итерации")
        ax3.set_xlabel('x')
        ax3.set_ylabel("f'(x)")
        ax3.legend()
        ax3.grid(True, alpha=0.3)
    
    # 4. Сходимость метода
    iterations = [h["iteration"] for h in history[:-1]]
    x_values = [h["x"] for h in history[:-1]]
    df_values = [abs(h["df_x"]) for h in history[:-1]]
    
    ax4.plot(iterations, x_values, 'bo-', linewidth=2, markersize=6, label='x_k')
    ax4.axhline(y=xmin, color='green', linestyle='--', alpha=0.5, 
               label=f'Предел: {xmin:.8f}')
    ax4.set_title("Сходимость последовательности x_k")
    ax4.set_xlabel('Номер итерации (k)')
    ax4.set_ylabel('x_k')
    ax4.legend(loc='upper right')
    ax4.grid(True, alpha=0.3)
    
    # Вторная ось для ошибки
    ax4_twin = ax4.twinx()
    ax4_twin.semilogy(iterations, df_values, 'r--', alpha=0.7, label="|f'(x_k)|")
    ax4_twin.set_ylabel("|f'(x_k)| (лог масштаб)")
    ax4_twin.legend(loc='center right')
    
    plt.tight_layout()
    plt.savefig("secant_final.png", dpi=100, bbox_inches='tight')
    plt.close()
    
    print("\n" + "=" * 80)
    print("ФИНАЛЬНЫЙ РЕЗУЛЬТАТ:")
    print("=" * 80)
    print('<img width="1000px" src="/resources/secant_final.png">')
    
    print(f"\nРезультат:")
    print(f"Найденный минимум: x = {xmin:.12f}, f(x) = {fmin:.12f}")
    print(f"f'(x_min) = {df(xmin):.12f} (должно быть близко к 0)")
    
    # Проверка сходимости
    final_h = history[-1]
    print(f"\nПроверка сходимости:")
    print(f"|f'(x_min)| = {abs(df(xmin)):.2e} {'<' if abs(df(xmin)) <= 1e-8 else '>='} tol = 1e-8")
    print(f"Длина интервала: {abs(final_h['b'] - final_h['a']):.2e}")
    
    # Таблица итераций
    print("\n" + "=" * 120)
    print("ТАБЛИЦА ИТЕРАЦИЙ МЕТОДА СЕКУЩИХ:")
    print("=" * 120)
    print(f"{'k':<4} {'x_k':<15} {'f(x_k)':<15} {'f\'(x_k)':<15} {'a':<15} {'b':<15} {'Длина инт.':<15} {'Обновление':<20}")
    print("-" * 120)
    
    for i, h in enumerate(history[:-1]):  # Исключаем финальную запись
        if i < 12:  # Показываем только первые 12 итераций
            interval_len = abs(h['b'] - h['a'])
            update_info = h.get('update', 'начальная')
            print(f"{h['iteration']:<4} {h['x']:<15.8f} {f(h['x']):<15.8f} "
                  f"{h['df_x']:<15.8f} {h['a']:<15.8f} {h['b']:<15.8f} "
                  f"{interval_len:<15.8f} {update_info:<20}")
    
    if len(history) > 12:
        print("... (показаны только первые 12 итераций)")
    
    # Финальная строка
    final_h = history[-1]
    print(f"{final_h['iteration']:<4} {final_h['x']:<15.8f} {final_h['fmin']:<15.8f} "
          f"{final_h['df_x']:<15.8f} {final_h['a']:<15.8f} {final_h['b']:<15.8f} "
          f"{abs(final_h['b'] - final_h['a']):<15.8f} {'финальный':<20}")
    print("=" * 120)


if __name__ == "__main__":
    print("=" * 80)
    print("МЕТОД СЕКУЩИХ ДЛЯ ПОИСКА МИНИМУМА")
    print("=" * 80)
    print("Функция: f(x) = x² - 10·cos(0.3πx) - 20")
    print("Производная: f'(x) = 2x + 3π·sin(0.3πx)")
    print("Интервал: [-2, 7], Точность: 1e-8")
    print("=" * 80)
    
    result = ssearch([-2, 7], 0.00000001)
    secant_method_visualization(result, [-2, 7])