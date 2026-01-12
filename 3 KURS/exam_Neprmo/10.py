import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import random


def f(x):
    return (x - 3) ** 2 - 3 * x + x ** 2 - 40


def gsearch(interval, tol):
    a, b = interval
    coords = []
    F_const = (1 + np.sqrt(5)) / 2

    x1 = b - (b - a) / F_const
    x2 = a + (b - a) / F_const
    f1 = f(x1)
    f2 = f(x2)
    neval = 2

    # Сохраняем начальное состояние
    coords.append({
        "a": a,
        "b": b,
        "x1": x1,
        "x2": x2,
        "f1": f1,
        "f2": f2,
        "iteration": 1
    })

    iteration = 1
    while (np.abs(b - a) > tol) and np.abs(f2 - f1) > tol:
        if f1 > f2:
            a = x1
            x1 = x2
            x2 = a + (b - a) / F_const
            f1 = f2
            f2 = f(x2)
            neval += 1
        else:
            b = x2
            x2 = x1
            x1 = b - (b - a) / F_const
            f2 = f1
            f1 = f(x1)
            neval += 1

        iteration += 1
        coords.append({
            "a": a,
            "b": b,
            "x1": x1,
            "x2": x2,
            "f1": f1,
            "f2": f2,
            "iteration": iteration
        })

    xmin = (a + b) / 2
    fmin = f(xmin)
    neval += 1

    # Добавляем финальное состояние
    coords.append({
        "a": a,
        "b": b,
        "x1": x1,
        "x2": x2,
        "f1": f1,
        "f2": f2,
        "xmin": xmin,
        "fmin": fmin,
        "iteration": iteration + 1
    })

    print(f"xmin = {xmin:.10f}, Fmin = {fmin:.10f}")
    print(f"Количество вычислений функции: {neval}")
    print(f"Количество итераций: {iteration}")

    answer_ = [xmin, fmin, neval, coords]
    return answer_


def draw_interval(a, b, x1, x2, ax, color, iteration):
    h = (b - a) / 100
    x_vals = np.arange(a, b + h, h)
    y_vals = [f(x) for x in x_vals]

    # Основной график функции
    ax.plot(x_vals, y_vals, lw=1.5, c=color)

    # Отмечаем границы интервала
    ax.scatter([a, b], [f(a), f(b)], marker='o', c=color, s=80, zorder=5)
    ax.plot([a, b], [0, 0], c=(0, 0, 0), lw=2)

    # Отмечаем точки золотого сечения
    y1 = f(x1)
    y2 = f(x2)
    ax.plot([x1, x1], [0, y1], lw=1.5, c=color, ls='--', alpha=0.7)
    ax.plot([x2, x2], [0, y2], lw=1.5, c=color, ls='--', alpha=0.7)
    ax.scatter([x1, x2], [y1, y2], marker='s', c=color, s=100, zorder=5)

    # Подписи точек
    ax.text(x1, y1 + 2, f'x1={x1:.4f}\nf1={y1:.4f}',
            ha='center', fontsize=9, bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow", alpha=0.8))
    ax.text(x2, y2 + 2, f'x2={x2:.4f}\nf2={y2:.4f}',
            ha='center', fontsize=9, bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow", alpha=0.8))
    ax.text(a, -5, f'a={a:.4f}', ha='center', fontsize=9, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue", alpha=0.8))
    ax.text(b, -5, f'b={b:.4f}', ha='center', fontsize=9, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue", alpha=0.8))

    # Заливка интервала
    ax.axvspan(a, b, alpha=0.2, color=color)

    ax.set_xlim([-2.25, 10.25])
    ax.set_ylim([min(y_vals) - 10, max(y_vals) + 10])


def gs2slides(interval, answer):
    xmin, fmin, neval, coords_history = answer
    a_start, b_start = interval

    # Данные для визуализации
    h = (b_start - a_start) / 100
    x = np.arange(a_start, b_start + 0.2, h)
    y = [f(i) for i in x]
    miny = min(y)
    maxy = max(y)
    deltaX = (b_start - a_start) / 200
    deltaY = abs(maxy - miny) / 200

    Nfigs = min(9, len(coords_history))  # максимум 9 графиков

    print("\n" + "=" * 70)
    print("ВИЗУАЛИЗАЦИЯ МЕТОДА ЗОЛОТОГО СЕЧЕНИЯ:")
    print("=" * 70)

    # Создаем отдельные графики для каждой итерации
    for i in range(Nfigs):
        coord = coords_history[i]

        fig, ax = plt.subplots(figsize=[12, 7])

        # Выбираем цвет для этой итерации
        color = (random.random() * 0.5, random.random() * 0.5 + 0.5, random.random() * 0.5)

        draw_interval(coord["a"], coord["b"], coord["x1"], coord["x2"], ax, color, i + 1)

        # Определяем логику для следующего шага
        if i < len(coords_history) - 1:
            next_coord = coords_history[i + 1]
            if coord["f1"] > coord["f2"]:
                action = f"f1 > f2 → a = x1 = {coord['x1']:.4f}"
            else:
                action = f"f1 ≤ f2 → b = x2 = {coord['x2']:.4f}"

            ax.set_title(f"Метод золотого сечения — Итерация {coord['iteration']}\n"
                        f"Интервал: [{coord['a']:.4f}, {coord['b']:.4f}], Длина: {coord['b'] - coord['a']:.6f}\n"
                        f"Следующий шаг: {action}")
        else:
            ax.set_title(f"Метод золотого сечения — Итерация {coord['iteration']}\n"
                        f"Финальный интервал: [{coord['a']:.4f}, {coord['b']:.4f}]")

        ax.set_xlabel('x')
        ax.set_ylabel('f(x)')
        ax.grid(True, alpha=0.3)

        # Добавляем информацию о золотом сечении
        phi = (1 + np.sqrt(5)) / 2
        ax.text(0.02, 0.98,
               f"Золотое сечение φ = {phi:.6f}\n"
               f"x1 = b - (b-a)/φ\n"
               f"x2 = a + (b-a)/φ",
               transform=ax.transAxes,
               verticalalignment='top',
               bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgray", alpha=0.7))

        fname = f"gs_iter_{coord['iteration']}.png"
        plt.savefig(fname, dpi=100, bbox_inches='tight')
        plt.close()

        print(f'<img width="800px" src="/resources/{fname}">')
        print(f"Итерация {coord['iteration']}:")
        print(f"  Интервал: [{coord['a']:.6f}, {coord['b']:.6f}], длина = {coord['b'] - coord['a']:.6f}")
        print(f"  Точки: x1 = {coord['x1']:.6f} (f={coord['f1']:.6f}), "
              f"x2 = {coord['x2']:.6f} (f={coord['f2']:.6f})")
        if 'xmin' in coord:
            print(f"  xmin = {coord['xmin']:.6f}, fmin = {coord['fmin']:.6f}")
        print("-" * 60)

    # Финальный график с результатом
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=[16, 6])

    # Левый график: весь процесс
    colors = plt.cm.rainbow(np.linspace(0, 1, len(coords_history)))
    for i, coord in enumerate(coords_history):
        if i < len(coords_history) - 1:
            color = colors[i]
            ax1.axvspan(coord["a"], coord["b"], alpha=0.2, color=color)
            ax1.scatter([coord["x1"], coord["x2"]], [coord["f1"], coord["f2"]],
                       color=color, s=50, alpha=0.6)
            if i < 5:  # Подписываем только первые 5 итераций
                ax1.text((coord["a"] + coord["b"]) / 2, miny - 15,
                        f"{i+1}", ha='center', fontsize=8)

    # Основной график функции
    ax1.plot(x, y, 'b-', lw=2, label='f(x)')
    ax1.scatter(xmin, fmin, color='red', s=200, marker='*',
               label=f'Найденный минимум\nx={xmin:.8f}\nf={fmin:.8f}')

    # Точный минимум (можно вычислить аналитически)
    # f(x) = (x-3)² - 3x + x² - 40 = 2x² - 9x - 31
    # Производная: 4x - 9 = 0 → x = 2.25
    exact_min = 2.25
    ax1.axvline(exact_min, color='green', linestyle='--', alpha=0.5,
               label=f'Точный минимум\nx={exact_min:.8f}')

    ax1.set_title(f"Весь процесс поиска ({len(coords_history)} итераций)")
    ax1.set_xlabel('x')
    ax1.set_ylabel('f(x)')
    ax1.legend(loc='upper right')
    ax1.grid(True, alpha=0.3)

    # Правый график: детали последней итерации
    last_coord = coords_history[-1]
    draw_interval(last_coord["a"], last_coord["b"],
                  last_coord.get("x1", last_coord["a"]),
                  last_coord.get("x2", last_coord["b"]),
                  ax2, 'blue', len(coords_history))

    ax2.scatter(xmin, fmin, color='red', s=200, marker='*', zorder=10,
               label=f'xmin={xmin:.8f}\nfmin={fmin:.8f}')
    ax2.axvline(exact_min, color='green', linestyle='--', alpha=0.5,
               label=f'Точный x={exact_min:.8f}')

    ax2.set_title(f"Финальная итерация\nДлина интервала: {last_coord['b'] - last_coord['a']:.10f}")
    ax2.legend(loc='upper right')

    plt.tight_layout()
    plt.savefig("gs_final.png", dpi=100, bbox_inches='tight')
    plt.close()

    print("\n" + "=" * 70)
    print("ФИНАЛЬНЫЙ РЕЗУЛЬТАТ:")
    print("=" * 70)
    print('<img width="1000px" src="/resources/gs_final.png">')
    print(f"\nНайденный минимум: x = {xmin:.10f}, f(x) = {fmin:.10f}")
    print(f"Точный минимум:    x = {exact_min:.10f}, f(x) = {f(exact_min):.10f}")
    print(f"Разница: Δx = {abs(xmin - exact_min):.12f}")
    print(f"Количество вычислений функции: {neval}")
    print(f"Количество итераций: {len(coords_history)}")

    # Таблица итераций
    print("\n" + "=" * 90)
    print("ТАБЛИЦА ИТЕРАЦИЙ:")
    print("=" * 90)
    print(f"{'Итер':<6} {'a':<12} {'b':<12} {'x1':<12} {'x2':<12} {'f(x1)':<12} {'f(x2)':<12} {'Длина':<12}")
    print("-" * 90)

    for i, coord in enumerate(coords_history):
        if i < 15:  # Показываем первые 15 итераций
            print(f"{coord['iteration']:<6} "
                  f"{coord['a']:<12.6f} "
                  f"{coord['b']:<12.6f} "
                  f"{coord['x1']:<12.6f} "
                  f"{coord['x2']:<12.6f} "
                  f"{coord['f1']:<12.6f} "
                  f"{coord['f2']:<12.6f} "
                  f"{coord['b'] - coord['a']:<12.6f}")

    if len(coords_history) > 15:
        print("... (показаны только первые 15 итераций)")
    print("=" * 90)


if __name__ == "__main__":
    print("=" * 70)
    print("МЕТОД ЗОЛОТОГО СЕЧЕНИЯ")
    print("=" * 70)
    print("Функция: f(x) = (x-3)² - 3x + x² - 40 = 2x² - 9x - 31")
    print("Интервал: [0, 10], Точность: 1e-8")
    print("=" * 70)

    result = gsearch([0, 10], 0.00000001)
    gs2slides([0, 10], result)