import numpy as np
from scipy.optimize import line_search

def bfgs_simple(f, grad_f, x0, max_iter=1000, tol=1e-6, alpha_init=1.0):
    """
    Упрощенная версия BFGS с фиксированным или адаптивным шагом
    
    Параметры:
    f - целевая функция
    grad_f - градиент функции
    x0 - начальная точка
    max_iter - максимальное количество итераций
    tol - требуемая точность
    alpha_init - начальный размер шага
    """
    n = len(x0)
    x = x0.copy()
    
    # Начальная аппроксимация обратной матрицы Гессе
    H = np.eye(n)
    
    # Вычисляем начальный градиент
    grad = grad_f(x)
    grad_norm = np.linalg.norm(grad)
    
    # История
    history = {
        'x': [x.copy()],
        'f': [f(x)],
        'grad_norm': [grad_norm],
        'iterations': 0
    }
    
    # Параметры для условия Вольфе
    c1 = 1e-4  # параметр для условия Армихо
    c2 = 0.9   # параметр для условия кривизны
    
    for k in range(max_iter):
        # Направление поиска
        p = -H @ grad
        
        # Линейный поиск с использованием условия Вольфе
        alpha = alpha_init
        max_ls_iter = 20
        ls_success = False
        
        for ls_iter in range(max_ls_iter):
            x_new = x + alpha * p
            f_new = f(x_new)
            grad_new = grad_f(x_new)
            
            # Условие Армихо (достаточное убывание)
            armijo = f_new <= f(x) + c1 * alpha * (grad @ p)
            
            # Условие кривизны
            curvature = grad_new @ p >= c2 * (grad @ p)
            
            if armijo and curvature:
                ls_success = True
                break
            
            alpha *= 0.5  # уменьшаем шаг
        
        if not ls_success:
            alpha = 1e-4  # используем очень маленький шаг
        
        # Сохраняем старые значения
        x_old = x.copy()
        grad_old = grad.copy()
        
        # Делаем шаг
        x = x + alpha * p
        grad = grad_f(x)
        grad_norm = np.linalg.norm(grad)
        
        # Разности для обновления BFGS
        s = x - x_old
        y = grad - grad_old
        
        # Проверка условия обновления (избегаем численных проблем)
        s_dot_y = s @ y
        
        if s_dot_y > 1e-10:
            rho = 1.0 / s_dot_y
            
            # Обновление BFGS для обратной матрицы Гессе
            I = np.eye(n)
            Hy = H @ y
            H = H + (rho * (s @ y + y @ Hy) * np.outer(s, s) * rho 
                    - rho * (np.outer(s, Hy) + np.outer(Hy, s)))
        
        # Сохраняем историю
        history['x'].append(x.copy())
        history['f'].append(f(x))
        history['grad_norm'].append(grad_norm)
        history['iterations'] = k + 1
        
        # Критерий остановки
        if grad_norm < tol:
            print(f"Сходимость достигнута на итерации {k+1}")
            break
    
    if k == max_iter - 1:
        print(f"Достигнуто максимальное количество итераций ({max_iter})")
    
    result = {
        'x_min': x,
        'f_min': f(x),
        'success': grad_norm < tol,
        'iterations': history['iterations'],
        'grad_norm': grad_norm,
        'history': history
    }
    
    return result


# Пример использования
if __name__ == "__main__":
    
    
    # Пример 2: Функция Розенброка
    def rosenbrock_f(x):
        """Функция Розенброка: f(x,y) = (1-x)^2 + 100*(y-x^2)^2"""
        return (1 - x[0])**2 + 100 * (x[1] - x[0]**2)**2
    
    def rosenbrock_grad(x):
        df_dx = -2*(1 - x[0]) - 400*x[0]*(x[1] - x[0]**2)
        df_dy = 200*(x[1] - x[0]**2)
        return np.array([df_dx, df_dy])
    
    print("\nПример 2: Функция Розенброка")
    x0 = np.array([-1.2, 1.0])
    
    result_bfgs_simple = bfgs_simple(rosenbrock_f, rosenbrock_grad, x0, max_iter=1000)
    print(f"Упрощенный BFGS: x_min = {result_bfgs_simple['x_min']}, f_min = {result_bfgs_simple['f_min']:.6f}")
    print(f"Итераций: {result_bfgs_simple['iterations']}")