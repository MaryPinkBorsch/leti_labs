import numpy as np
import numpy as np
from numpy.linalg import norm

def gsearch(f, interval, tol, max_iter=100):
    """
    Одномерный поиск с помощью метода золотого сечения.
    
    Параметры:
    f - функция одного переменного
    interval - список [a, b], задающий начальный интервал
    tol - точность поиска
    max_iter - максимальное количество итераций
    
    Возвращает:
    alpha - точка минимума на интервале
    """
    a, b = interval[0], interval[1]
    
    # Коэффициент золотого сечения
    phi = (np.sqrt(5) - 1) / 2  # ≈ 0.618
    
    # Начальные точки
    x1 = b - phi * (b - a)
    x2 = a + phi * (b - a)
    
    f1 = f(x1)
    f2 = f(x2)
    
    for _ in range(max_iter):
        if (b - a) < tol:
            break
            
        if f1 < f2:
            # Минимум в [a, x2]
            b = x2
            x2 = x1
            f2 = f1
            x1 = b - phi * (b - a)
            f1 = f(x1)
        else:
            # Минимум в [x1, b]
            a = x1
            x1 = x2
            f1 = f2
            x2 = a + phi * (b - a)
            f2 = f(x2)
    
    return (a + b) / 2

# Пример использования для функции Химмельблау
def himmelblau(x):
    """Функция Химмельблау f(x,y) = (x^2 + y - 11)^2 + (x + y^2 - 7)^2"""
    return (x[0]**2 + x[1] - 11)**2 + (x[0] + x[1]**2 - 7)**2

def himmelblau_grad(x):
    """Градиент функции Химмельблау"""
    df_dx = 4 * x[0] * (x[0]**2 + x[1] - 11) + 2 * (x[0] + x[1]**2 - 7)
    df_dy = 2 * (x[0]**2 + x[1] - 11) + 4 * x[1] * (x[0] + x[1]**2 - 7)
    return np.array([df_dx, df_dy])



def sdsearch(f, df, x0, tol):
   kmax = 1000
   x = x0.copy()
   coords = [x.copy()]


   neval = 0
   interval = [0, 1]


   deltaX = np.ones_like(x) * 1e6
   k = 0


   while (np.linalg.norm(deltaX) >= tol) and (k < kmax):
       g = df(x)
       neval += 1
       f1dim = lambda a: f(x - a * g)


       alpha = gsearch(f1dim, interval, tol)
       x_new = x - alpha * g


       coords.append(x_new.copy())
       deltaX = x_new - x


       x = x_new.copy()
       k += 1


   xmin = x
   fmin = f(xmin)
   neval += 1
   answer_ = [xmin, fmin, neval, coords]
#    print("Xmin = ",xmin,"Fmin = ", fmin)
   return answer_


# Тестовый пример
if __name__ == "__main__":
    # Начальная точка
    x0 = np.array([0.0, 0.0])
    
    # Точность
    tol = 1e-6
    
    # Запуск градиентного спуска
    result = sdsearch(himmelblau, himmelblau_grad, x0, tol)
    
    xmin, fmin, neval, coords = result
    print(f"Найденный минимум: {xmin}")
    print(f"Значение функции: {fmin}")
    print(f"Количество вычислений градиента: {neval}")
    print(f"Количество итераций: {len(coords)-1}")
    
    # Проверка - известные минимумы функции Химмельблау
    # (3.0, 2.0), (-2.805118, 3.131312), (-3.779310, -3.283186), (3.584428, -1.848126)