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


def armijosearch(f, df, x0, tol):
    x = np.array(x0).ravel()
    coords = [x.copy()]
    neval = 0
    s = 1.0          
    beta = 0.5      
    c1 = 1e-4        
    k = 0
    kmax = 1000
    while True:
        gk = df(x)
        neval += 1
        pk = -gk  
        alpha = s
        fx = f(x)
        neval += 1
        while True:
            x_new = x + alpha * pk
            fx_new = f(x_new)
            neval += 1
            if fx_new <= fx + c1 * alpha * np.dot(gk, pk):
                break
            alpha = alpha*beta
        coords.append(x_new.copy())
        if norm(x_new - x) < tol or k >= kmax:
            xmin = x_new
            fmin = f(xmin)
            neval += 1
            break
        x = x_new
        k += 1
    answer_ = [xmin, fmin, neval, coords]
    print("xmin = ", xmin, "Fmin = ", fmin)
    return answer_

if __name__ == "__main__":
    armijosearch(fH, dfH, [1.5,1.5], 1e-4)
