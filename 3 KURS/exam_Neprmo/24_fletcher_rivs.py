import numpy as np
import sys
from numpy.linalg import norm

def fH(X):
    x = X[0]
    y = X[1]
    v = (x ** 2 + y - 11) ** 2 + (x + y ** 2 - 7) ** 2
    return v

def dfH(X):
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = 2 * (x ** 2 + y - 11) * (2 * x) + 2 * (x + y ** 2 - 7)
    v[1] = 2 * (x ** 2 + y - 11) + 2 * (x + y ** 2 - 7) * (2 * y)

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
from numpy.linalg import norm

def prsearch(f, df, x0, tol):
    kmax = 1000
    c1 = tol
    c2 = 0.1
    amax = 3

    xk = x0.copy()
    gk = df(xk)
    neval = 1
    pk = -gk

    coords = [xk.copy()]
    k = 0

    while (norm(gk) >= tol) and (k < kmax):
        alpha = wolfesearch(f, df, xk, pk, amax, c1, c2)

        xk1 = xk + alpha * pk
        gk1 = df(xk1)
        neval += 1
        
        # Исправленное вычисление beta - универсальный способ
        # Вычисляем квадрат нормы градиентов
        if isinstance(gk, np.ndarray):
            # Для numpy массивов
            norm_gk1_sq = np.vdot(gk1, gk1)
            norm_gk_sq = np.vdot(gk, gk)
        else:
            # Для других типов данных
            gk1_flat = np.asarray(gk1).ravel()
            gk_flat = np.asarray(gk).ravel()
            norm_gk1_sq = np.dot(gk1_flat, gk1_flat)
            norm_gk_sq = np.dot(gk_flat, gk_flat)
        
        beta = norm_gk1_sq / norm_gk_sq

        pk = -gk1 + beta * pk

        xk = xk1
        gk = gk1
        coords.append(xk.copy())
        k += 1

    xmin = xk
    fmin = f(xmin)
    answer_ = [xmin, fmin, neval, coords]
    print("xmin = ", xmin, "Fmin = ", fmin)
    return answer_


if __name__ == "__main__":
    prsearch(fH, dfH, [1.5,1.5], 1e-4)