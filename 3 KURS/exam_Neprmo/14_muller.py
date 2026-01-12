import numpy as np

# Более простая и надежная версия метода Мюллера
def muller_simple(f, x0, x1, x2, tol=1e-6, max_iter=100):
    """
    Упрощенная и более устойчивая версия метода Мюллера
    """
    neval = 0
    coords = []
    
    # Вычисляем значения функции
    f0 = f(x0); neval += 1
    f1 = f(x1); neval += 1
    f2 = f(x2); neval += 1
    
    coords.append([x0, x1, x2, None, f0, f1, f2])
    
    for k in range(max_iter):
        # Сортируем точки по x
        points = sorted([(x0, f0), (x1, f1), (x2, f2)], key=lambda p: p[0])
        x0, f0 = points[0]
        x1, f1 = points[1]
        x2, f2 = points[2]
        
        # Проверяем сходимость
        if abs(x2 - x0) < tol:
            break
        
        # Используем метод параболической интерполяции
        # Строим квадратичную функцию через три точки
        # Формулы для коэффициентов квадратичной функции
        
        # Решаем систему уравнений для a, b, c:
        # a*x0^2 + b*x0 + c = f0
        # a*x1^2 + b*x1 + c = f1
        # a*x2^2 + b*x2 + c = f2
        
        # Используем метод конечных разностей для устойчивости
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
            
            # Находим вершину параболы (если a > 0 - минимум)
            if abs(a) > 1e-12:
                x_new = -b / (2 * a)
                # Проверяем, что точка внутри интервала
                if x_new < min(x0, x1, x2) or x_new > max(x0, x1, x2):
                    # Если вершина вне интервала, используем метод золотого сечения
                    x_new = x1 + (x2 - x1) / 1.618
            else:
                # Парабола вырождается в прямую
                if abs(b) > 1e-12:
                    x_new = -c / b
                else:
                    x_new = (x0 + x2) / 2.0
        
        # Вычисляем значение в новой точке
        f_new = f(x_new); neval += 1
        
        # Обновляем точки
        # Заменяем точку с наибольшим значением функции, если новая точка лучше
        points = [(x0, f0), (x1, f1), (x2, f2), (x_new, f_new)]
        # Находим точку с максимальным значением функции (худшую)
        worst_idx = np.argmax([f0, f1, f2, f_new])
        
        if worst_idx < 3:  # Если худшая точка среди трех старых
            if worst_idx == 0:
                x0, f0 = x_new, f_new
            elif worst_idx == 1:
                x1, f1 = x_new, f_new
            else:
                x2, f2 = x_new, f_new
        
        coords.append([x0, x1, x2, x_new, f0, f1, f2])
        
        # Проверка сходимости
        f_values = [f0, f1, f2]
        if max(f_values) - min(f_values) < tol:
            break
    
    # Находим точку с минимальным значением функции
    points = [(x0, f0), (x1, f1), (x2, f2)]
    xmin, fmin = min(points, key=lambda p: p[1])
    
    answer_ = [xmin, fmin, neval, coords]
    return answer_


# Пример использования
if __name__ == "__main__":
    # Тестовая функция
    def test_f(x):
        return x**4 - 3*x**3 + 2*x**2 - x + 5
    
    print("Метод Мюллера для поиска минимума функции")
    print("Функция: f(x) = x^4 - 3x^3 + 2x^2 - x + 5")
    
    # Выбираем начальные точки
    x0, x1, x2 = 0.0, 1.0, 2.0
    
    # Запускаем метод Мюллера
    result = muller_simple(test_f, x0, x1, x2, tol=1e-8)
    
    print(f"\nРезультат:")
    print(f"Найденный минимум: x = {result[0]:.10f}")
    print(f"Значение функции: f(x) = {result[1]:.10f}")
    print(f"Количество вычислений функции: {result[2]}")
          
       
    
    
    # Дополнительный тест с более сложной функцией
    print("\n" + "="*50)
    print("Тест с функцией Розенброка (одномерный аналог):")
    
    def rosenbrock_1d(x):
        return (1 - x)**2 + 100*(0 - x**2)**2  # Фиксируем y = 0 для одномерного случая
    
    x0, x1, x2 = -1.5, -0.5, 0.5
    result_r = muller_simple(rosenbrock_1d, x0, x1, x2, tol=1e-8, max_iter=100)
    print(f"Минимум: x = {result_r[0]:.10f}, f(x) = {result_r[1]:.10f}")
    print(f"Количество вычислений: {result_r[2]}")