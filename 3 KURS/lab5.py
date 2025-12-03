import numpy as np
import sys
from numpy.linalg import norm

#F_HIMMELBLAU is a Himmelblau function
# 	v = F_HIMMELBLAU(X)
#	INPUT ARGUMENTS:
#	X - is 2x1 vector of input variables
#	OUTPUT ARGUMENTS:
#	v is a function value
def f(X):
    x = X[0]
    y = X[1]
    #Версия питона в codeboard не поддерживает метод библиотеки numpy float_power 
    v = (x**2 + y - 11)**2 + (x + y** 2 - 7)**2
    return v
    
# DF_HIMMELBLAU is a Himmelblau function derivative
# 	v = DF_HIMMELBLAU(X)
#	INPUT ARGUMENTS:
#	X - is 2x1 vector of input variables
#	OUTPUT ARGUMENTS:
#	v is a derivative function value
def df(X):
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = 2 * (x**2 + y - 11) * (2 * x) + 2 * (x + y**2 - 7)
    v[1] = 2 * (x**2 + y - 11) + 2 * (x + y**2 - 7) * (2 * y)
    return v


def grsearch(x0, tol):
    # GRSEARCH searches for minimum using gradient descent method
    # 	answer_ = grsearch(x0,tol)
    #   INPUT ARGUMENTS
    #	x0 - starting point
    # 	tol - set for bot range and function value
    #   OUTPUT ARGUMENTS
    #   answer_ = [xmin, fmin, neval, coords]
    # 	xmin is a function minimizer
    # 	fmin = f(xmin)
    # 	neval - number of function evaluations
    #   coords - array of x values found during optimization    

    kmax = 1000  # максимальное число итераций
    al = 0.01    # параметр шага
    k = 0        # счетчик итераций
    neval = 0    # счетчик вычислений производной
    coords = []  # массив для хранения координат
    
    x = x0.copy()
    coords.append(x.copy())
    
    while k < kmax:
        # Вычисляем градиент
        grad = df(x)
        neval += 1
        
        # Выполняем шаг градиентного спуска
        x_new = x - al * grad
        
        # Проверяем условие остановки
        deltaX = x_new - x
        if norm(deltaX) < tol:
            break
            
        # Обновляем точку
        x = x_new
        coords.append(x.copy())
        k += 1
    
    xmin = x
    fmin = f(xmin)
    neval += 1
    
    answer_ = [xmin, fmin, neval, coords]
    return answer_


if __name__ == "__main__":
    grsearch([0,10],0.00000001)