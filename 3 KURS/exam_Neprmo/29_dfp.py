import numpy as np
from numpy.linalg import norm


def fH(X):
    x = X[0]
    y = X[1]
    v = (x**2 + y - 11)**2 + (x + y**2 - 7)**2
    return v


def dfH(X):
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = 2 * (x**2 + y - 11) * (2 * x) + 2 * (x + y**2 - 7)
    v[1] = 2 * (x**2 + y - 11) + 2 * (x + y**2 - 7) * (2 * y)
    return v

 

def wolfesearch(f, df, xk, pk, amax, c1, c2):
    """
    Поиск длины шага по Вольфе для нелинейной оптимизации.
    Упрощенная версия для случая, когда xk и gk - двумерные массивы.
    """
    
    # Инициализация
    alpha = amax
    alpha_low = 0.0
    alpha_high = amax
    
    # Вычисление значений в начальной точке
    fk = f(xk)
    gk = df(xk)
    
    # Производная по направлению - для двумерных массивов
    # Если gk и pk имеют форму (n, 1), то dot(gk.T, pk) вернет [[scalar]]
    slope_result = np.dot(gk.T, pk)
    
    # Извлекаем скалярное значение
    if hasattr(slope_result, 'shape') and slope_result.shape == (1, 1):
        slope = slope_result[0, 0]
    elif hasattr(slope_result, 'item'):  # Для numpy скаляров
        slope = slope_result.item()
    else:
        slope = float(slope_result)
    
    # Максимальное количество итераций
    max_iter = 50
    
    for i in range(max_iter):
        # Вычисление значения функции и градиента в новой точке
        x_new = xk + alpha * pk
        f_new = f(x_new)
        g_new = df(x_new)
        
        # Проверка условия Армихо (достаточного уменьшения)
        armijo = f_new <= fk + c1 * alpha * slope
        
        # Проверка условия кривизны
        curvature_result = np.dot(g_new.T, pk)
        if hasattr(curvature_result, 'shape') and curvature_result.shape == (1, 1):
            curvature_slope = curvature_result[0, 0]
        elif hasattr(curvature_result, 'item'):
            curvature_slope = curvature_result.item()
        else:
            curvature_slope = float(curvature_result)
            
        curvature = curvature_slope >= c2 * slope
        
        if not armijo:
            # Условие Армихо не выполнено - уменьшаем шаг
            alpha_high = alpha
            alpha = 0.5 * (alpha_low + alpha_high)
        elif not curvature:
            # Условие кривизны не выполнено - увеличиваем нижнюю границу
            alpha_low = alpha
            if alpha_high >= amax:
                alpha = 2.0 * alpha
            else:
                alpha = 0.5 * (alpha_low + alpha_high)
        else:
            # Оба условия выполнены
            return alpha
            
        # Ограничение alpha сверху
        alpha = min(alpha, amax)
    
    # Если не удалось найти подходящий шаг, возвращаем последний alpha
    return alpha
import numpy as np

def Hnew(dk, yk, H):
    """
    Обновление матрицы H по формуле DFP (Davidon-Fletcher-Powell).
    
    Параметры:
    dk - вектор изменения переменных: dk = x_{k+1} - x_k
    yk - вектор изменения градиентов: yk = g_{k+1} - g_k
    H - текущая аппроксимация обратной матрицы Гессе
    
    Возвращает:
    H_new - обновленная матрица
    """
    
    # Преобразуем входные данные в массивы NumPy
    dk = np.asarray(dk, dtype=float).reshape(-1, 1)  # столбец
    yk = np.asarray(yk, dtype=float).reshape(-1, 1)  # столбец
    H = np.asarray(H, dtype=float)
    
    # Вычисляем скалярные произведения
    dk_T_yk = np.dot(dk.T, yk)[0, 0]  # скаляр
    yk_T_H_yk = np.dot(yk.T, np.dot(H, yk))[0, 0]  # скаляр
    
    # Проверяем, что dk_T_yk не слишком мал (для численной устойчивости)
    if abs(dk_T_yk) < 1e-15:
        return H  # возвращаем старую матрицу если знаменатель слишком мал
    
    # Вычисляем слагаемые для обновления
    term1 = np.dot(dk, dk.T) / dk_T_yk
    
    # Второе слагаемое
    H_yk = np.dot(H, yk)
    term2 = np.dot(H_yk, yk.T)
    term2 = np.dot(term2, H)
    term2 = term2 / yk_T_H_yk
    
    # Обновляем матрицу H по формуле DFP
    H_new = H + term1 - term2
    
    return H_new

def dfpsearch(f, df, x0, tol):
   c1 = tol
   c2 = 0.1
   coords = []
   coords.append(x0.copy())
   kmax = 1000
   k = 0
   amax = 3
   g0 = df(x0)
   gnew = g0
   H = np.eye(2)
   dk = 0.5
   while (np.linalg.norm(dk) >= tol) and (k < kmax):
       p0 = np.dot(-H, gnew)
       alpha = wolfesearch(f, df, x0, p0, amax, c1, c2)
       x1 = x0 + alpha * p0
       dk = alpha * p0
       gnew = df(x1)
       yk = gnew - g0
       H = Hnew(dk, yk, H)
       g0 = gnew
       x0 = x1


       coords.append(x0)
       k += 1
   xmin = x0
   fmin = f(xmin)
   neval = k
   answer_ = [xmin, fmin, neval, coords]
   print("xmin = ", xmin, "Fmin = ", fmin)
   return answer_

if __name__ == "__main__":
    dfpsearch(fH, dfH, [1.5,1.5], 1e-4)

