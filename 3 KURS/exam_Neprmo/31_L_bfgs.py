import numpy as np
from scipy.optimize import line_search

# Версия BFGS с ограниченной памятью (L-BFGS)
class LBFGS:
    """
    Метод L-BFGS (Limited-memory BFGS) для больших задач
    """
    def __init__(self, m=10):
        """
        Инициализация L-BFGS
        
        Параметры:
        m - количество пар (s, y) для хранения в памяти
        """
        self.m = m
        self.s_history = []
        self.y_history = []
        self.rho_history = []
    
    def two_loop_recursion(self, grad):
        """
        Двухцикловая процедура для вычисления направления поиска
        """
        q = grad.copy()
        n = len(grad)
        
        # Количество сохраненных пар
        k = len(self.s_history)
        
        # Прямой цикл
        alpha = np.zeros(k)
        for i in range(k-1, -1, -1):
            alpha[i] = self.rho_history[i] * (self.s_history[i] @ q)
            q = q - alpha[i] * self.y_history[i]
        
        # Начальная аппроксимация H0 (скалярная)
        if k > 0:
            H0 = (self.s_history[-1] @ self.y_history[-1]) / (self.y_history[-1] @ self.y_history[-1])
        else:
            H0 = 1.0
        
        r = H0 * q
        
        # Обратный цикл
        for i in range(k):
            beta = self.rho_history[i] * (self.y_history[i] @ r)
            r = r + (alpha[i] - beta) * self.s_history[i]
        
        return -r
    
    def optimize(self, f, grad_f, x0, max_iter=1000, tol=1e-6):
        """
        Оптимизация методом L-BFGS
        """
        x = x0.copy()
        grad = grad_f(x)
        
        history = {
            'x': [x.copy()],
            'f': [f(x)],
            'grad_norm': [np.linalg.norm(grad)],
            'iterations': 0
        }
        
        for k in range(max_iter):
            # Вычисляем направление поиска
            p = self.two_loop_recursion(grad)
            
            # Простой линейный поиск
            alpha = 1.0
            for _ in range(10):
                x_new = x + alpha * p
                f_new = f(x_new)
                
                # Простое условие Армихо
                if f_new <= f(x) + 1e-4 * alpha * (grad @ p):
                    break
                alpha *= 0.5
            
            # Сохраняем старые значения
            x_old = x.copy()
            grad_old = grad.copy()
            
            # Делаем шаг
            x = x + alpha * p
            grad = grad_f(x)
            
            # Обновляем историю пар (s, y)
            s = x - x_old
            y = grad - grad_old
            
            s_dot_y = s @ y
            
            if s_dot_y > 1e-10:
                rho = 1.0 / s_dot_y
                
                self.s_history.append(s)
                self.y_history.append(y)
                self.rho_history.append(rho)
                
                # Поддерживаем ограниченную память
                if len(self.s_history) > self.m:
                    self.s_history.pop(0)
                    self.y_history.pop(0)
                    self.rho_history.pop(0)
            
            # Сохраняем историю
            history['x'].append(x.copy())
            history['f'].append(f(x))
            history['grad_norm'].append(np.linalg.norm(grad))
            history['iterations'] = k + 1
            
            # Критерий остановки
            if np.linalg.norm(grad) < tol:
                print(f"L-BFGS: Сходимость достигнута на итерации {k+1}")
                break
        
        result = {
            'x_min': x,
            'f_min': f(x),
            'success': np.linalg.norm(grad) < tol,
            'iterations': history['iterations'],
            'history': history
        }
        
        return result


# Пример использования
if __name__ == "__main__":
    # Пример 1: Квадратичная функция
    def quadratic_f(x):
        """f(x) = (x1-1)^2 + (x2-2)^2 + (x3-3)^2"""
        return (x[0]-1)**2 + (x[1]-2)**2 + (x[2]-3)**2
    
    def quadratic_grad(x):
        return np.array([2*(x[0]-1), 2*(x[1]-2), 2*(x[2]-3)])
    
     
    
    # Пример 2: Функция Розенброка
    def rosenbrock_f(x):
        """Функция Розенброка: f(x,y) = (1-x)^2 + 100*(y-x^2)^2"""
        return (1 - x[0])**2 + 100 * (x[1] - x[0]**2)**2
    
    def rosenbrock_grad(x):
        df_dx = -2*(1 - x[0]) - 400*x[0]*(x[1] - x[0]**2)
        df_dy = 200*(x[1] - x[0]**2)
        return np.array([df_dx, df_dy])
    
    print("\n Функция Розенброка")
    x0 = np.array([-1.2, 1.0])
    
    
    
    # Пример 3: L-BFGS
    print("\nПример 3: L-BFGS")
    lbfgs_solver = LBFGS(m=5)
    result_lbfgs = lbfgs_solver.optimize(rosenbrock_f, rosenbrock_grad, x0, max_iter=1000)
    print(f"L-BFGS: x_min = {result_lbfgs['x_min']}, f_min = {result_lbfgs['f_min']:.6f}")
    print(f"Итераций: {result_lbfgs['iterations']}")