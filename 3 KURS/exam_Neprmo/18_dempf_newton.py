import numpy as np

def damped_newton_search(tol, x0, max_iter=1000):
    """
    Демпфированный метод Ньютона для одномерной оптимизации
    
    Параметры:
    tol - требуемая точность (критерий остановки по градиенту)
    x0 - начальная точка
    max_iter - максимальное количество итераций
    
    Возвращает:
    [xmin, fmin, neval, coords] - минимум, значение функции, 
                                  количество вычислений, траектория
    """
    coords = [x0]
    neval = 0
    k = 1
    x_prev = x0
    alpha = 1.0  # начальный шаг (демпфирующий коэффициент)
    
    while k <= max_iter:
        # Вычисляем градиент и гессиан
        df_xprev = df(x_prev)
        ddf_xprev = ddf(x_prev)
        neval += 2
        
        # Проверяем, что гессиан положительный (для минимизации)
        if ddf_xprev <= 0:
            # Если гессиан неположительный, используем небольшой демпфирующий коэффициент
            ddf_xprev = max(ddf_xprev, 1e-6)
            alpha = 0.1  # уменьшаем шаг для стабилизации
        
        # Вычисляем направление Ньютона
        newton_direction = df_xprev / ddf_xprev
        
        # Линейный поиск для нахождения оптимального шага
        alpha = line_search(x_prev, -newton_direction, df_xprev)
        
        # Обновляем точку с демпфированием
        x = x_prev - alpha * newton_direction
        
        # Вычисляем градиент в новой точке
        df_x = df(x)
        neval += 1
        
        coords.append(x)
        
        # Критерий остановки
        if np.abs(df_x) < tol:
            xmin = x
            fmin = f(xmin)
            neval += 1
            break
        else:
            k += 1
            x_prev = x
            
            # Адаптивное изменение демпфирующего коэффициента
            # Увеличиваем шаг, если прогресс хороший
            if np.abs(df_x) < 0.5 * np.abs(df_xprev):
                alpha = min(alpha * 1.2, 1.0)
    
    if k > max_iter:
        xmin = x
        fmin = f(xmin)
        neval += 1
        print("Достигнуто максимальное количество итераций")
    
   
    answer_ = [xmin, fmin, neval, coords]
    return answer_


def line_search(x, direction, grad, max_alpha=1.0):
    """
    Простой линейный поиск для определения демпфирующего коэффициента
    
    Параметры:
    x - текущая точка
    direction - направление поиска
    grad - градиент в текущей точке
    max_alpha - максимально допустимый шаг
    """
    alpha = max_alpha
    c = 0.5  # параметр достаточного убывания
    rho = 0.5  # коэффициент уменьшения шага
    
    # Проверяем несколько значений альфа
    for _ in range(10):
        x_new = x + alpha * direction
        f_x = f(x)
        f_new = f(x_new)
        
        # Проверка условия Армихо (достаточного убывания)
        if f_new <= f_x + c * alpha * grad * direction:
            return alpha
        alpha *= rho
    
    return alpha  # возвращаем последнее значение


# Пример использования функций f, df, ddf (нужно определить)
def f(x):
    """Пример целевой функции"""
    return x**2 + np.sin(x)

def df(x):
    """Производная целевой функции"""
    return 2*x + np.cos(x)

def ddf(x):
    """Вторая производная целевой функции"""
    return 2 - np.sin(x)


# Альтернативная версия с фиксированным демпфированием
def damped_newton_fixed(tol, x0, damping_factor=0.5, max_iter=1000):
    """
    Демпфированный метод Ньютона с фиксированным коэффициентом демпфирования
    
    Параметры:
    tol - требуемая точность
    x0 - начальная точка
    damping_factor - фиксированный коэффициент демпфирования (0 < factor <= 1)
    max_iter - максимальное количество итераций
    """
    coords = [x0]
    neval = 0
    k = 1
    x_prev = x0
    
    while k <= max_iter:
        # Вычисляем градиент и гессиан
        df_xprev = df(x_prev)
        ddf_xprev = ddf(x_prev)
        neval += 2
        
        # Добавляем демпфирование для обеспечения положительности гессиана
        if ddf_xprev <= 0:
            ddf_xprev = 1.0  # или abs(ddf_xprev) + 1e-6
        
        # Применяем демпфирование
        x = x_prev - damping_factor * (df_xprev / ddf_xprev)
        
        # Вычисляем градиент в новой точке
        df_x = df(x)
        neval += 1
        
        coords.append(x)
        
        # Критерий остановки
        if np.abs(df_x) < tol:
            xmin = x
            fmin = f(xmin)
            neval += 1
            break
        else:
            k += 1
            x_prev = x
    
    if k > max_iter:
        xmin = x
        fmin = f(xmin)
        neval += 1
        print("Достигнуто максимальное количество итераций")
    
    return [xmin, fmin, neval, coords]


# Пример использования
if __name__ == "__main__":
    # Тестирование демпфированного метода Ньютона
    result = damped_newton_search(tol=1e-6, x0=2.0)
    print(f"Минимум: x = {result[0]:.6f}, f(x) = {result[1]:.6f}")
    print(f"Количество вычислений: {result[2]}")
    
    # Тестирование метода с фиксированным демпфированием
    result_fixed = damped_newton_fixed(tol=1e-6, x0=2.0, damping_factor=0.7)
    print(f"\nФиксированное демпфирование:")
    print(f"Минимум: x = {result_fixed[0]:.6f}, f(x) = {result_fixed[1]:.6f}")
    print(f"Количество вычислений: {result_fixed[2]}")