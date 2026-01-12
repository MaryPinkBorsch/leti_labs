import numpy as np
import matplotlib.pyplot as plt
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
# Тестовая функция и её производная
def f(x):
    return (x - 2)**2 + 1  # минимум в x=2, f(2)=1

def df(x):
    return 2*(x - 2)  # производная



def bisectionsearch2slides(interval, answer, tol):
    xmin, fmin, neval, coords = answer
    
    # Создаем данные для графика
    x = np.linspace(interval[0], interval[1], 1000)
    y = f(x)
    
    nslides = min(9, len(coords))
    
    # Для хранения истории поиска (чтобы совместиться с оригинальным форматом)
    history = []
    a, b = interval[0], interval[1]
    
    # Реконструируем историю поиска
    for i, xk in enumerate(coords):
        history.append({
            "a": a,
            "b": b,
            "xk": xk,
            "df": df(xk)
        })
        
        # Обновляем границы как в вашем алгоритме
        if df(xk) > 0:
            b = xk
        else:
            a = xk
    
    # Если история пуста (сошлось за 0 итераций)
    if len(history) == 0:
        history.append({
            "a": interval[0],
            "b": interval[1],
            "xk": (interval[0] + interval[1]) / 2,
            "df": df((interval[0] + interval[1]) / 2)
        })
    
    # Создаем слайды для каждой итерации
    for i in range(nslides):
        h = history[i]
        
        plt.figure(figsize=(9, 5))
        plt.plot(x, y, label="f(x) = (x-2)² + 1", linewidth=2)
        
        # текущий интервал
        plt.axvline(h["a"], color="blue", linestyle="--", alpha=0.7, label="a")
        plt.axvline(h["b"], color="green", linestyle="--", alpha=0.7, label="b")
        
        # область текущего интервала
        plt.axvspan(h["a"], h["b"], alpha=0.2, color='gray')
        
        # текущая середина
        plt.scatter(h["xk"], f(h["xk"]), color="red", s=100, zorder=5, label=f"x_{i+1}")
        plt.axvline(h["xk"], color="red", alpha=0.4, linestyle=':')
        
        # подпись df
        sign = "f'(xk) > 0" if h["df"] > 0 else "f'(xk) ≤ 0"
        action = "правая граница b" if h["df"] > 0 else "левая граница a"
        plt.text(
            h["xk"],
            f(h["xk"]) + 0.1,
            f"x_{i+1} = {h['xk']:.4f}\n{sign}\nобновляем {action}",
            fontsize=9,
            verticalalignment="bottom",
            horizontalalignment="center",
            bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow", alpha=0.8)
        )
        
        plt.title(f"Бинарный поиск минимума — итерация {i+1}")
        plt.xlabel("x")
        plt.ylabel("f(x)")
        plt.grid(True, alpha=0.3)
        plt.legend(loc='upper right')
        plt.ylim(min(y) - 0.5, max(y) + 0.5)
        
        fname = f"8_plot_iter_{i+1}.png"
        plt.savefig(fname, dpi=100, bbox_inches='tight')
        plt.close()
        
        print(f'<img width="550px" src="{fname}">')
        print(f"Итерация {i+1}: x = {h['xk']:.6f}, f'(x) = {h['df']:.6f}")
        print(f"Интервал: [{h['a']:.6f}, {h['b']:.6f}]")
        print("-" * 50)
    
    # финальный график
    plt.figure(figsize=(9, 5))
    plt.plot(x, y, label="f(x) = (x-2)² + 1", linewidth=2)
    plt.axvline(x=2, color='green', linestyle='--', alpha=0.5, label='Истинный минимум (x=2)')
    plt.axhline(y=1, color='green', linestyle='--', alpha=0.5)
    
    # Показываем все точки поиска
    for i, xk in enumerate(coords):
        plt.plot(xk, f(xk), 'ro', markersize=8)
        if i < len(coords) - 1:
            plt.text(xk, f(xk) + 0.15, f'x_{i+1}', fontsize=9, ha='center')
    
    plt.scatter(
        xmin,
        fmin,
        color="blue",
        s=150,
        marker="*",
        label=f"Найденный минимум\nx = {xmin:.6f}\nf(x) = {fmin:.6f}"
    )
    
    plt.title(f"Финальный результат\nТочность: {tol}, Вычислений: {neval}")
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.grid(True, alpha=0.3)
    plt.legend(loc='upper right')
    plt.ylim(min(y) - 0.5, max(y) + 0.5)
    
    plt.savefig("8_plotFin.png", dpi=100, bbox_inches='tight')
    plt.close()
    
    print("\nФинальный результат:")
    print('<img width="550px" src="/8/plotFin.png">')
    print(f"x_min = {xmin:.8f}, f_min = {fmin:.8f}")
    print(f"Всего вычислений: {neval}")
    print(f"Количество итераций: {len(coords)}")

# Ваш код функции бинарного поиска
def bsearch(interval,tol):
    a = interval[0]
    b = interval[1]
    L = b - a
    k = 1
    g = df(a)
    neval = 1
    coords = []
    xmin = None
    while ((np.abs(L) > tol) and (np.abs(g) > tol)):
        x_k = (a + b)/2
        xmin = x_k
        coords.append(x_k)
        d = df(x_k)
        neval += 1
        if d > 0:
            b = x_k
        else:
            a = x_k
            g = d
        L = b - a
        k = k + 1
    if xmin == None:
        xmin = x_k = (a + b)/2
    fmin = f(xmin)
    neval += 1
    answer_ = [xmin, fmin, neval, coords]
    print("Xmin = ", xmin,"Fmin = ", fmin,"k = ", neval)
    return answer_
 
    
if __name__ == "__main__":
    # Пример 1: Простой тест
    print("ПРИМЕР 1: Поиск минимума функции f(x) = (x-2)² + 1")
    print("="*60)
    result1 = bsearch([0, 8], 1e-3)
    
    bisectionsearch2slides([0,8], result1,1e-3)
     

    # Пример 2: Другой интервал
    print("\n\n" + "="*60)
    print("ПРИМЕР 2: Поиск на интервале [1, 5]")
    print("="*60)
    result2 = bsearch([1, 5], 1e-4) 