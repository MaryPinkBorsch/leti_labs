import numpy as np
import matplotlib.pyplot as plt

# Тестовая функция и её производная
def f(x):
    return (x - 2)**2 + 1  # минимум в x=2, f(2)=1

def df(x):
    return 2*(x - 2)  # производная

# Ваш код функции бинарного поиска
def bsearch(interval, tol):
    a = interval[0]
    b = interval[1]
    L = b - a
    k = 1
    g = df(a)
    neval = 1
    coords = []
    xmin = None
    
    print(f"Начало поиска на интервале [{a}, {b}]")
    print(f"Ищем минимум функции f(x) = (x-2)² + 1")
    print(f"Точность tol = {tol}")
    print("-" * 50)
    
    while ((np.abs(L) > tol) and (np.abs(g) > tol)):
        x_k = (a + b) / 2
        xmin = x_k
        coords.append(x_k)
        d = df(x_k)
        neval += 1
        
        print(f"Итерация {k}:")
        print(f"  Интервал: [{a:.6f}, {b:.6f}]")
        print(f"  Середина x_{k} = {x_k:.6f}")
        print(f"  f(x_{k}) = {f(x_k):.6f}")
        print(f"  f'(x_{k}) = {d:.6f}")
        
        if d > 0:
            print(f"  f'(x_{k}) > 0 => смещаем правую границу: b = {x_k:.6f}")
            b = x_k
        else:
            print(f"  f'(x_{k}) ≤ 0 => смещаем левую границу: a = {x_k:.6f}")
            a = x_k
            g = d
            
        L = b - a
        k = k + 1
        print(f"  Новый интервал: [{a:.6f}, {b:.6f}], длина = {L:.6f}")
        print("-" * 30)
    
    if xmin is None:
        xmin = (a + b) / 2
    
    fmin = f(xmin)
    neval += 1
    
    print("=" * 50)
    print("РЕЗУЛЬТАТ:")
    print(f"Найденный минимум: x = {xmin:.8f}")
    print(f"Значение функции в минимуме: f(x) = {fmin:.8f}")
    print(f"Всего вычислений функции: {neval}")
    print(f"Количество итераций: {k-1}")
    
    answer_ = [xmin, fmin, neval, coords]
    return answer_

# Функция для визуализации процесса поиска
def visualize_search(interval, tol, answer):
    xmin, fmin, neval, coords = answer
    
    # Создаем данные для графика
    x = np.linspace(interval[0], interval[1], 400)
    y = f(x)
    
    plt.figure(figsize=(12, 6))
    
    # График функции
    plt.subplot(1, 2, 1)
    plt.plot(x, y, 'b-', linewidth=2, label='f(x) = (x-2)² + 1')
    plt.axvline(x=2, color='g', linestyle='--', alpha=0.5, label='Истинный минимум (x=2)')
    plt.axhline(y=1, color='g', linestyle='--', alpha=0.5)
    
    # Отмечаем точки поиска
    for i, xk in enumerate(coords):
        plt.plot(xk, f(xk), 'ro', markersize=8)
        plt.text(xk, f(xk) + 0.1, f'x_{i+1}', fontsize=10, ha='center')
    
    plt.plot(xmin, fmin, 'y*', markersize=15, label='Найденный минимум')
    plt.xlabel('x')
    plt.ylabel('f(x)')
    plt.title('Поиск минимума методом бинарного поиска')
    plt.grid(True, alpha=0.3)
    plt.legend()
    
    # График производной
    plt.subplot(1, 2, 2)
    y_prime = df(x)
    plt.plot(x, y_prime, 'r-', linewidth=2, label="f'(x) = 2(x-2)")
    plt.axhline(y=0, color='k', linestyle='-', alpha=0.3)
    plt.axvline(x=2, color='g', linestyle='--', alpha=0.5)
    
    # Отмечаем точки поиска на графике производной
    for i, xk in enumerate(coords):
        plt.plot(xk, df(xk), 'ro', markersize=8)
        plt.text(xk, df(xk) + 0.2, f'x_{i+1}', fontsize=10, ha='center')
    
    plt.xlabel('x')
    plt.ylabel("f'(x)")
    plt.title('Производная функции')
    plt.grid(True, alpha=0.3)
    plt.legend()
    
    plt.tight_layout()
    plt.show()
    
    # Таблица итераций
    print("\n" + "="*60)
    print("ТАБЛИЦА ИТЕРАЦИЙ:")
    print("="*60)
    print(f"{'Итерация':<10} {'x_k':<15} {'f(x_k)':<15} {'f(x_k)':<15}")
    print("-"*60)
    
    for i, xk in enumerate(coords, 1):
        print(f"{i:<10} {xk:<15.6f} {f(xk):<15.6f} {df(xk):<15.6f}")
    
    print(f"{'Финальная':<10} {xmin:<15.6f} {fmin:<15.6f} {df(xmin):<15.6f}")
    print("="*60)
    
    
if __name__ == "__main__":
    # Пример 1: Простой тест
    print("ПРИМЕР 1: Поиск минимума функции f(x) = (x-2)² + 1")
    print("="*60)
    result1 = bsearch([0, 4], 1e-3)
    visualize_search([0, 4], 1e-3, result1)

    # Пример 2: Другой интервал
    print("\n\n" + "="*60)
    print("ПРИМЕР 2: Поиск на интервале [1, 5]")
    print("="*60)
    result2 = bsearch([1, 5], 1e-4)
    visualize_search([1, 5], 1e-4, result2)