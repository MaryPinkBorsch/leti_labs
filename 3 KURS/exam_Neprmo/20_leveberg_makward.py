import numpy as np
import sys
from numpy.linalg import norm
np.seterr(all='warn')
def fR(X):
    x = X[0]
    y = X[1]
    v = (1 - x)**2 + 100*(y - x**2)**2
    return v




def dfR(X):
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = -2 * (1 - x) + 200 * (y - x**2)*(- 2 * x)
    v[1] = 200 * (y - x**2)
    return v


def H(X, tol, df):
    n = len(X)
    h = 0.1 * tol
    Hk = np.zeros((n, n))


    for i in range(n):
        for j in range(n):
            ej = np.zeros(n)
            ej[j] = 1.0
            df_plus = df(X + h * ej)[i]
            df_minus = df(X - h * ej)[i]
            Hk[i, j] = (df_plus - df_minus) / (2 * h)


    return Hk


def lmsearch(f, df, x0, tol):
    x = np.array(x0).ravel()
    coords = [x.copy()]
    neval = 0
    alpha = 100    
    v = 3  
    k = 0
    kmax = 1000


    while True:
        gk = df(x)
        neval += 1
        Hk = H(x, tol, df)
        n = len(x)
        neval += 2 * n * n
        A = Hk + alpha * np.eye(n)
        step = np.linalg.solve(A, gk)
        x_new = x - step
        if f(x_new) < f(x):
            alpha = alpha / v
            x = x_new
            coords.append(x.copy())
        else:
            alpha = alpha * v
        if norm(step) < tol or k >= kmax:
            xmin = x
            fmin = f(xmin)
            break
        k += 1
    answer_ = [xmin, fmin, neval, coords]
    print("xmin = ", xmin, "Fmin = ", fmin)
    return answer_


if __name__ == "__main__":
    lmsearch(fR, dfR, [-2,-2], 1e-4)
