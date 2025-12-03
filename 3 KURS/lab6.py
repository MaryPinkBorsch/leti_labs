import numpy as np
import sys
from numpy.linalg import norm

def goldensectionsearch(f, interval, tol):
    # GOLDENSECTIONSEARCH searches for minimum using golden section method
    # answer_ = goldensectionsearch(f, interval, tol)
    # INPUT ARGUMENTS
    # f - function
    # interval = [a, b] - search interval
    # tol - set for bot range and function value
    # OUTPUT ARGUMENTS
    # answer_ = [xmin, fmin, neval]
    # xmin is a function minimizer
    # fmin = f(xmin)
    # neval - number of function evaluations
    
    a, b = interval
    neval = 0
    phi = (1 + np.sqrt(5)) / 2  # golden ratio
    resphi = 2 - phi
    
    # Initial points
    x1 = a + resphi * (b - a)
    x2 = b - resphi * (b - a)
    f1 = f(x1)
    f2 = f(x2)
    neval += 2
    
    while (b - a) > tol:
        if f1 < f2:
            b = x2
            x2 = x1
            f2 = f1
            x1 = a + resphi * (b - a)
            f1 = f(x1)
            neval += 1
        else:
            a = x1
            x1 = x2
            f1 = f2
            x2 = b - resphi * (b - a)
            f2 = f(x2)
            neval += 1
    
    xmin = (a + b) / 2
    fmin = f(xmin)
    neval += 1
    
    answer_ = [xmin, fmin, neval]
    return answer_

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

def fR(X):
    x = X[0]
    y = X[1]
    v = (1 - x) ** 2 + 100 * (y - x ** 2) ** 2
    return v

def dfR(X):
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = -2 * (1 - x) + 200 * (y - x ** 2) * (- 2 * x)
    v[1] = 200 * (y - x ** 2)
    return v

def sdsearch(f, df, x0, tol):
    # SDSEARCH searches for minimum using steepest descent method
    # answer_ = sdsearch(f, df, x0, tol)
    # INPUT ARGUMENTS
    # f  - objective function
    # df - gradient
    # x0 - start point
    # tol - set for bot range and function value
    # OUTPUT ARGUMENTS
    # answer_ = [xmin, fmin, neval, coords]
    # xmin is a function minimizer
    # fmin = f(xmin)
    # neval - number of function evaluations
    # coords - array of statistics
    
    kmax = 1000  # maximum number of iterations
    k = 0        # iteration counter
    neval = 0    # gradient evaluations counter
    coords = []  # array for storing coordinates
    
    x = x0.copy()
    coords.append(x.copy())
    
    while k < kmax:
        # Calculate gradient
        grad = df(x)
        neval += 1
        
        # Define 1D function along the gradient direction
        f1dim = lambda alpha: f(x - alpha * grad)
        
        # Find optimal step using golden section search
        interval = [0, 1]
        alpha_result = goldensectionsearch(f1dim, interval, tol)
        alpha_opt = alpha_result[0]
        
        # Calculate new point
        x_new = x - alpha_opt * grad
        
        # Check stopping criterion
        deltaX = x_new - x
        if norm(deltaX) < tol:
            break
            
        # Update point
        x = x_new
        coords.append(x.copy())
        k += 1
    
    xmin = x
    fmin = f(xmin)
    neval += 1
    
    answer_ = [xmin, fmin, neval, coords]
    return answer_


# if __name__ == "__main__":
#     sdsearch([0,10],0.00000001)