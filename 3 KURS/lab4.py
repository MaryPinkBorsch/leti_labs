import numpy as np

def f(x): 
    return x**2 - 10*np.cos(0.3*np.pi*x) - 20

def df(x): 
    return 2*x + 3*np.pi*np.sin(0.3*np.pi*x)

def ssearch(interval,tol):
    # SSEARCH searches for minimum using secant method
    # 	answer_ = ssearch(interval,tol)
    #   INPUT ARGUMENTS
    # 	interval = [a, b] - search interval
    # 	tol - set for bot range and function value
    #   OUTPUT ARGUMENTS
    #   answer_ = [xmin, fmin, neval, coords]
    # 	xmin is a function minimizer
    # 	fmin = f(xmin)
    # 	neval - number of function evaluations
    #   coords - array of x values found during optimization    

    a, b = interval
    coords = []
    neval = 0
    
    # Начальные точки для метода секущих
    x0 = a
    x1 = b
    
    # Вычисляем производные в начальных точках
    df0 = df(x0)
    df1 = df(x1)
    neval += 2
    
    coords.append([x0, a, b])
    coords.append([x1, a, b])
    
    # Основной цикл метода секущих
    while (np.abs(df1) > tol) and (np.abs(b - a) > tol):
        # Проверяем, чтобы не было деления на ноль
        if abs(df1 - df0) < 1e-12:
            break
            
        # Формула метода секущих для нахождения нуля производной
        x_new = x1 - df1 * (x1 - x0) / (df1 - df0)
        
        # Вычисляем производную в новой точке
        df_new = df(x_new)
        neval += 1
        
        # Обновляем границы интервала в зависимости от знака производной
        if df_new > 0:
            b = x_new  # Производная положительна - минимум слева
        else:
            a = x_new  # Производная отрицательна - минимум справа
        
        # Обновляем точки для следующей итерации
        x0, x1 = x1, x_new
        df0, df1 = df1, df_new
        
        coords.append([x_new, a, b])
    
    # Определяем точку минимума
    xmin = x1
    fmin = f(xmin)
    neval += 1
    
    answer_ = [xmin, fmin, neval, coords]
    return answer_



if __name__ == "__main__":
    ssearch([0,10],0.00000001)