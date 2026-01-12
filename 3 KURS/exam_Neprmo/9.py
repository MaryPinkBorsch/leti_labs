import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def f(x):
    return 2*(x)**2 -9*x -31

def bsearch(interval, tol):
    a, b = interval
    coords = []
    history = []  # Для хранения истории итераций
    
    x_m = (a + b) / 2
    fm = f(x_m)
    neval = 1
    
    iteration = 1
    while np.abs(b - a) > tol:
        # сохраняем текущее состояние для истории
        x1 = a + (b - a) / 4
        x2 = b - (b - a) / 4
        f1 = f(x1)
        f2 = f(x2)
        
        history.append({
            "a": a,
            "b": b,
            "x1": x1,
            "x2": x2,
            "xm": x_m,
            "f1": f1,
            "f2": f2,
            "fm": fm,
            "iteration": iteration
        })
        
        coords.append(x_m)
        neval += 2
        
        # логика поиска
        if f1 < fm:
            b = x_m
            x_m = x1
            fm = f1
        elif f1 >= fm and fm <= f2:
            a = x1
            b = x2
        elif fm > f2:
            a = x_m
            x_m = x2
            fm = f2
        
        iteration += 1
    
    # Добавляем финальное состояние
    x1 = a + (b - a) / 4
    x2 = b - (b - a) / 4
    f1 = f(x1)
    f2 = f(x2)
    
    history.append({
        "a": a,
        "b": b,
        "x1": x1,
        "x2": x2,
        "xm": x_m,
        "f1": f1,
        "f2": f2,
        "fm": fm,
        "iteration": iteration
    })
    
    xmin = x_m
    fmin = fm
    
    print(f"Xmin = {xmin:.8f}, Fmin = {fmin:.8f}")
    print(f"Количество вычислений функции: {neval}")
    print(f"Количество итераций: {iteration-1}")
    
    answer_ = [xmin, fmin, neval, coords]
    return answer_, history

def three_point_search_slides(interval, history):
    x = np.linspace(interval[0], interval[1], 1000)
    y = f(x)

    nslides = min(9, len(history))

    for i in range(nslides):
        h = history[i]

        plt.figure(figsize=(10, 6))
        plt.plot(x, y, label="f(x)", linewidth=2)
        
        # Заливка текущего интервала
        plt.axvspan(h["a"], h["b"], alpha=0.2, color='gray', label='Текущий интервал')
        
        # Границы интервала
        plt.axvline(h["a"], color="black", linestyle="--", linewidth=1.5, label=f'a = {h["a"]:.4f}')
        plt.axvline(h["b"], color="black", linestyle="--", linewidth=1.5, label=f'b = {h["b"]:.4f}')
        
        # Внутренние точки
        plt.axvline(h["x1"], color="orange", linestyle=":", linewidth=1.5, label=f'x1 = {h["x1"]:.4f}')
        plt.axvline(h["x2"], color="orange", linestyle=":", linewidth=1.5, label=f'x2 = {h["x2"]:.4f}')
        plt.axvline(h["xm"], color="red", linewidth=1.5, label=f'xm = {h["xm"]:.4f}')
        
        # Точки на графике
        plt.scatter(
            [h["x1"], h["xm"], h["x2"]],
            [h["f1"], h["fm"], h["f2"]],
            color=["orange", "red", "orange"],
            s=100,
            zorder=5
        )
        
        # Подписи значений
        plt.text(h["x1"], h["f1"] + 0.5, f'f1 = {h["f1"]:.4f}', 
                ha='center', fontsize=9, bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow", alpha=0.7))
        plt.text(h["xm"], h["fm"] + 0.5, f'fm = {h["fm"]:.4f}', 
                ha='center', fontsize=9, bbox=dict(boxstyle="round,pad=0.3", facecolor="red", alpha=0.7))
        plt.text(h["x2"], h["f2"] + 0.5, f'f2 = {h["f2"]:.4f}', 
                ha='center', fontsize=9, bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow", alpha=0.7))
        
        # Определяем следующий шаг
        if h["f1"] < h["fm"]:
            action = "f1 < fm → b = xm, xm = x1"
        elif h["f1"] >= h["fm"] and h["fm"] <= h["f2"]:
            action = "f1 ≥ fm ≤ f2 → a = x1, b = x2"
        elif h["fm"] > h["f2"]:
            action = "fm > f2 → a = xm, xm = x2"
        else:
            action = "Следующий шаг"
        
        plt.title(f"Трехточечный поиск — Итерация {h['iteration']}\n{action}")
        plt.xlabel("x")
        plt.ylabel("f(x)")
        plt.legend(loc='upper right')
        plt.grid(True, alpha=0.3)
        
        # Автоматическое масштабирование по Y
        y_vals = [h["f1"], h["fm"], h["f2"]]
        y_min, y_max = min(y_vals), max(y_vals)
        plt.ylim(y_min - 5, y_max + 10)

        fname = f"9_plot_iter_{h['iteration']}.png"
        plt.savefig(fname, dpi=100, bbox_inches='tight')
        plt.close()
        
        print(f'<img width="600px" src="/resources/{fname}">')
        print(f"Итерация {h['iteration']}: интервал [{h['a']:.6f}, {h['b']:.6f}]")
        print(f"  Точки: x1={h['x1']:.6f}, xm={h['xm']:.6f}, x2={h['x2']:.6f}")
        print(f"  Значения: f1={h['f1']:.6f}, fm={h['fm']:.6f}, f2={h['f2']:.6f}")
        print(f"  Действие: {action}")
        print("-" * 60)

    # Финальный график
    h = history[-1]
    plt.figure(figsize=(10, 6))
    plt.plot(x, y, label="f(x)", linewidth=2)
    
    # Показываем все промежуточные точки поиска
    for i, hist in enumerate(history[:-1]):
        if i < 8:  # Показываем только первые 8 для читаемости
            plt.scatter(hist["xm"], hist["fm"], color="red", s=50, alpha=0.5)
            plt.text(hist["xm"], hist["fm"] + 1, f'{i+1}', fontsize=8, ha='center')
    
    plt.axvline(h["xm"], color="blue", linewidth=3, label=f"xmin = {h['xm']:.6f}")
    plt.scatter(h["xm"], h["fm"], color="blue", s=150, zorder=5, marker="*")
    
    # Вычисляем точное значение минимума (для сравнения)
    # Для квадратичной функции f(x) = 2x² - 9x - 31 минимум при x = 9/4 = 2.25
    exact_min = 9/4
    plt.axvline(exact_min, color="green", linestyle="--", alpha=0.5, label="Точный минимум (x=2.25)")
    
    plt.title(f"Финальный результат\nНайденный минимум: x = {h['xm']:.8f}, f(x) = {h['fm']:.8f}")
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.legend(loc='upper right')
    plt.grid(True, alpha=0.3)
    
    plt.savefig("9_plot_final.png", dpi=100, bbox_inches='tight')
    plt.close()
    
    print("\nФинальный результат:")
    print('<img width="600px" src="/resources/plot_final.png">')
    print(f"Xmin = {h['xm']:.8f}, Fmin = {h['fm']:.8f}")
    print(f"Точный минимум: x = {exact_min:.8f}, f(x) = {f(exact_min):.8f}")
    print(f"Разница: Δx = {abs(h['xm'] - exact_min):.10f}")

if __name__ == "__main__":
    print("=" * 70)
    print("Трехточечный поиск минимума функции f(x) = 2x² - 9x - 31")
    print("=" * 70)
    
    # Выполняем поиск и получаем историю
    result, history = bsearch([-2, 10], 1e-4)
    print("ВИЗУАЛИЗАЦИЯ ПРОЦЕССА ПОИСКА:")
    print("=" * 70)
    
    # Создаем слайды визуализации
    three_point_search_slides([-2, 10], history)