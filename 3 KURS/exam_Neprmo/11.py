import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import random


def f(x):
    return (x - 3) ** 2 - 3 * x + x ** 2 - 40


def fibonacci_search(interval, tol, delta=1e-5):
    a, b = interval
    coord_history = []  # Будем хранить полную историю
    neval = 0

    # Вычисление чисел Фибоначчи
    fib = [1, 1]
    while fib[-1] < (b - a) / tol:
        fib.append(fib[-1] + fib[-2])
    n = len(fib) - 1
    
    print(f"Число итераций (n) = {n}")
    print(f"Числа Фибоначчи: {fib}")

    x_left = a + fib[n - 2] / fib[n] * (b - a)
    x_right = a + fib[n - 1] / fib[n] * (b - a)

    f_left = f(x_left)
    f_right = f(x_right)
    neval += 2

    # Сохраняем начальное состояние
    coord_history.append({
        "a": a,
        "b": b,
        "x_left": x_left,
        "x_right": x_right,
        "f_left": f_left,
        "f_right": f_right,
        "iteration": 1,
        "n": n,
        "k": 1,
        "fib_n": fib[n],
        "fib_n_minus_1": fib[n-1],
        "fib_n_minus_2": fib[n-2]
    })

    for k in range(1, n - 1):
        if f_left < f_right:
            b = x_right
            x_right = x_left
            f_right = f_left
            x_left = a + fib[n - k - 2] / fib[n - k] * (b - a)
            f_left = f(x_left)
        else:
            a = x_left
            x_left = x_right
            f_left = f_right
            x_right = a + fib[n - k - 1] / fib[n - k] * (b - a)
            f_right = f(x_right)

        neval += 1

        # Сохраняем текущее состояние
        coord_history.append({
            "a": a,
            "b": b,
            "x_left": x_left,
            "x_right": x_right,
            "f_left": f_left,
            "f_right": f_right,
            "iteration": k + 1,
            "n": n,
            "k": k + 1,
            "fib_n_minus_k": fib[n - k],
            "fib_n_minus_k_minus_1": fib[n - k - 1],
            "fib_n_minus_k_minus_2": fib[n - k - 2] if n - k - 2 >= 0 else 0
        })

    # Финальное уточнение
    mu = x_left + delta
    if f_left > f(mu):
        xmin = (x_left + b) / 2
    else:
        xmin = (a + mu) / 2
    fmin = f(xmin)
    neval += 1

    # Сохраняем финальное состояние
    coord_history.append({
        "a": a,
        "b": b,
        "x_left": x_left,
        "x_right": x_right,
        "f_left": f_left,
        "f_right": f_right,
        "xmin": xmin,
        "fmin": fmin,
        "iteration": n,
        "delta": delta,
        "is_final": True
    })

    print(f"xmin = {xmin:.10f}, Fmin = {fmin:.10f}")
    print(f"Количество вычислений функции: {neval}")
    print(f"Количество итераций: {n}")

    answer_ = [xmin, fmin, neval, coord_history]
    return answer_


def draw_fibonacci_interval(a, b, x_left, x_right, ax, color, iteration, fib_info=None):
    h = (b - a) / 100
    x_vals = np.arange(a, b + h, h)
    y_vals = [f(x) for x in x_vals]

    # Основной график функции
    ax.plot(x_vals, y_vals, lw=1.5, c=color)

    # Отмечаем границы интервала
    ax.scatter([a, b], [f(a), f(b)], marker='o', c=color, s=80, zorder=5)
    ax.plot([a, b], [0, 0], c=(0, 0, 0), lw=2)

    # Отмечаем точки Фибоначчи
    y_left = f(x_left)
    y_right = f(x_right)
    ax.plot([x_left, x_left], [0, y_left], lw=1.5, c=color, ls='--', alpha=0.7)
    ax.plot([x_right, x_right], [0, y_right], lw=1.5, c=color, ls='--', alpha=0.7)
    ax.scatter([x_left, x_right], [y_left, y_right], marker='s', c=color, s=100, zorder=5)

    # Подписи точек
    ax.text(x_left, y_left + 2, f'L={x_left:.4f}\nf(L)={y_left:.4f}',
            ha='center', fontsize=9, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgreen", alpha=0.8))
    ax.text(x_right, y_right + 2, f'R={x_right:.4f}\nf(R)={y_right:.4f}',
            ha='center', fontsize=9, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightcoral", alpha=0.8))
    ax.text(a, -5, f'a={a:.4f}', ha='center', fontsize=9,
            bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue", alpha=0.8))
    ax.text(b, -5, f'b={b:.4f}', ha='center', fontsize=9,
            bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue", alpha=0.8))

    # Заливка интервала
    ax.axvspan(a, b, alpha=0.2, color=color)

    # Показываем отношение Фибоначчи
    if fib_info and 'fib_n_minus_k' in fib_info and fib_info['fib_n_minus_k'] > 0:
        ratio = fib_info.get('fib_n_minus_k_minus_1', 0) / fib_info['fib_n_minus_k']
        ax.text((a + b) / 2, max(y_vals) + 5,
                f"F_{{{fib_info.get('k', iteration)}}} / F_{{{fib_info.get('k', iteration)+1}}} = {ratio:.4f}",
                ha='center', fontsize=10, bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow", alpha=0.8))

    ax.set_xlim([-2.25, 10.25])
    ax.set_ylim([min(y_vals) - 10, max(y_vals) + 10])


def fib_search_visualization(interval, answer):
    xmin, fmin, neval, history = answer
    a_start, b_start = interval

    # Данные для визуализации
    h = (b_start - a_start) / 100
    x = np.arange(a_start, b_start + 0.2, h)
    y = [f(i) for i in x]
    miny = min(y)
    maxy = max(y)

    Nfigs = min(9, len(history))  # максимум 9 графиков

    print("\n" + "=" * 80)
    print("ВИЗУАЛИЗАЦИЯ МЕТОДА ФИБОНАЧЧИ:")
    print("=" * 80)

    # Создаем отдельные графики для каждой итерации
    for i in range(Nfigs):
        coord = history[i]

        fig, ax = plt.subplots(figsize=[12, 7])

        # Выбираем цвет для этой итерации
        color = (random.random() * 0.5, random.random() * 0.5 + 0.5, random.random() * 0.5)

        draw_fibonacci_interval(coord["a"], coord["b"], coord["x_left"], coord["x_right"],
                               ax, color, coord["iteration"], coord)

        # Определяем логику для следующего шага
        action = ""
        if i < len(history) - 1 and not coord.get("is_final", False):
            if coord["f_left"] < coord["f_right"]:
                action = f"f(L) < f(R) → b = R = {coord['x_right']:.4f}"
            else:
                action = f"f(L) ≥ f(R) → a = L = {coord['x_left']:.4f}"

        title = f"Метод Фибоначчи — Итерация {coord['iteration']}"
        if 'n' in coord:
            title += f" (k={coord.get('k', 1)} из n={coord['n']})"
        
        title += f"\nИнтервал: [{coord['a']:.6f}, {coord['b']:.6f}], Длина: {coord['b'] - coord['a']:.8f}"
        
        if action:
            title += f"\nСледующий шаг: {action}"
        
        ax.set_title(title)
        ax.set_xlabel('x')
        ax.set_ylabel('f(x)')
        ax.grid(True, alpha=0.3)

        # Добавляем информацию о числах Фибоначчи
        if 'fib_n_minus_k' in coord and coord['fib_n_minus_k'] > 0:
            fib_text = f"F_{{{coord.get('k', coord['iteration'])}}} = {coord['fib_n_minus_k']}\n"
            if 'fib_n_minus_k_minus_1' in coord:
                fib_text += f"F_{{{coord.get('k', coord['iteration'])-1}}} = {coord['fib_n_minus_k_minus_1']}\n"
            fib_text += f"Отношение: {coord.get('fib_n_minus_k_minus_1', 0)/coord['fib_n_minus_k']:.6f}"
            
            ax.text(0.02, 0.98, fib_text,
                   transform=ax.transAxes, verticalalignment='top',
                   bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgray", alpha=0.7))

        fname = f"fib_iter_{coord['iteration']}.png"
        plt.savefig(fname, dpi=100, bbox_inches='tight')
        plt.close()

        print(f'<img width="800px" src="/resources/{fname}">')
        print(f"Итерация {coord['iteration']}:")
        print(f"  Интервал: [{coord['a']:.8f}, {coord['b']:.8f}], длина = {coord['b'] - coord['a']:.10f}")
        print(f"  Точки: L = {coord['x_left']:.8f} (f={coord['f_left']:.8f}), "
              f"R = {coord['x_right']:.8f} (f={coord['f_right']:.8f})")
        if 'fib_n_minus_k' in coord:
            print(f"  F_{{{coord.get('k', coord['iteration'])}}} = {coord['fib_n_minus_k']}, "
                  f"F_{{{coord.get('k', coord['iteration'])-1}}} = {coord.get('fib_n_minus_k_minus_1', 'N/A')}")
        print("-" * 70)

    # Финальный график с результатом
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=[16, 6])

    # Левый график: весь процесс
    colors = plt.cm.rainbow(np.linspace(0, 1, len(history)))
    for i, coord in enumerate(history):
        if i < len(history) - 1:  # Не включаем финальный
            color = colors[i]
            ax1.axvspan(coord["a"], coord["b"], alpha=0.1, color=color)
            ax1.scatter([coord["x_left"], coord["x_right"]],
                       [coord["f_left"], coord["f_right"]],
                       color=color, s=50, alpha=0.6)
            if i < 5:  # Подписываем только первые 5 итераций
                ax1.text((coord["a"] + coord["b"]) / 2, miny - 15,
                        f"{i+1}", ha='center', fontsize=8)

    # Основной график функции
    ax1.plot(x, y, 'b-', lw=2, label='f(x)')
    ax1.scatter(xmin, fmin, color='red', s=200, marker='*',
               label=f'Найденный минимум\nx={xmin:.10f}\nf={fmin:.10f}')

    # Точный минимум
    exact_min = 2.25  # 4x - 9 = 0 → x = 9/4 = 2.25
    ax1.axvline(exact_min, color='green', linestyle='--', alpha=0.5,
               label=f'Точный минимум\nx={exact_min:.10f}')

    ax1.set_title(f"Весь процесс поиска ({len(history)} итераций)")
    ax1.set_xlabel('x')
    ax1.set_ylabel('f(x)')
    ax1.legend(loc='upper right')
    ax1.grid(True, alpha=0.3)

    # Правый график: детали последней итерации
    last_coord = history[-2] if len(history) > 1 else history[-1]  # Предпоследняя итерация
    draw_fibonacci_interval(last_coord["a"], last_coord["b"],
                           last_coord["x_left"], last_coord["x_right"],
                           ax2, 'blue', last_coord["iteration"], last_coord)

    ax2.scatter(xmin, fmin, color='red', s=200, marker='*', zorder=10,
               label=f'xmin={xmin:.10f}\nfmin={fmin:.10f}')
    ax2.axvline(exact_min, color='green', linestyle='--', alpha=0.5,
               label=f'Точный x={exact_min:.10f}')

    ax2.set_title(f"Предпоследняя итерация (k={last_coord.get('k', 'N/A')})\n"
                 f"Длина интервала: {last_coord['b'] - last_coord['a']:.10f}")
    ax2.legend(loc='upper right')

    plt.tight_layout()
    plt.savefig("fib_final.png", dpi=100, bbox_inches='tight')
    plt.close()

    print("\n" + "=" * 80)
    print("ФИНАЛЬНЫЙ РЕЗУЛЬТАТ:")
    print("=" * 80)
    print('<img width="1000px" src="/resources/fib_final.png">')
    
    # Информация о финальном уточнении
    final_coord = history[-1]
    print(f"\nФинальное уточнение с δ={final_coord.get('delta', 'N/A')}:")
    if 'xmin' in final_coord:
        print(f"Найденный минимум: x = {final_coord['xmin']:.12f}, f(x) = {final_coord['fmin']:.12f}")
    
    print(f"Точный минимум:    x = {exact_min:.12f}, f(x) = {f(exact_min):.12f}")
    print(f"Разница: Δx = {abs(xmin - exact_min):.15f}")
    print(f"Количество вычислений функции: {neval}")
    print(f"Количество итераций: {len(history)-1}")

    # Таблица итераций
    print("\n" + "=" * 110)
    print("ТАБЛИЦА ИТЕРАЦИЙ МЕТОДА ФИБОНАЧЧИ:")
    print("=" * 110)
    print(f"{'Итер':<6} {'a':<12} {'b':<12} {'L':<12} {'R':<12} {'f(L)':<12} {'f(R)':<12} {'Длина':<12} {'F_k/F_{k+1}':<12}")
    print("-" * 110)

    for i, coord in enumerate(history):
        if i < 12 and not coord.get("is_final", False):  # Показываем первые 12 итераций (не финальных)
            fib_ratio = ""
            if 'fib_n_minus_k' in coord and coord['fib_n_minus_k'] > 0:
                ratio = coord.get('fib_n_minus_k_minus_1', 0) / coord['fib_n_minus_k']
                fib_ratio = f"{ratio:.6f}"
            
            print(f"{coord['iteration']:<6} "
                  f"{coord['a']:<12.8f} "
                  f"{coord['b']:<12.8f} "
                  f"{coord['x_left']:<12.8f} "
                  f"{coord['x_right']:<12.8f} "
                  f"{coord['f_left']:<12.8f} "
                  f"{coord['f_right']:<12.8f} "
                  f"{coord['b'] - coord['a']:<12.8f} "
                  f"{fib_ratio:<12}")

    if len(history) > 12:
        print("... (показаны только первые 12 итераций)")
    
    # Информация о последовательности Фибоначчи
    first_coord = history[0]
    if 'fib_n' in first_coord:
        print(f"\nИспользованная последовательность Фибоначчи (F_n ≥ (b-a)/ε):")
        fib_seq = [1, 1]
        while len(fib_seq) <= first_coord['n']:
            fib_seq.append(fib_seq[-1] + fib_seq[-2])
        print(f"F = {fib_seq}")
        print(f"n = {first_coord['n']}, F_{first_coord['n']} = {first_coord['fib_n']}")
    print("=" * 110)


if __name__ == "__main__":
    print("=" * 80)
    print("МЕТОД ФИБОНАЧЧИ ДЛЯ ПОИСКА МИНИМУМА")
    print("=" * 80)
    print("Функция: f(x) = (x-3)² - 3x + x² - 40 = 2x² - 9x - 31")
    print("Интервал: [0, 10], Точность: 1e-8, δ = 1e-5")
    print("=" * 80)

    result = fibonacci_search([0, 10], 0.00000001)
    fib_search_visualization([0, 10], result)