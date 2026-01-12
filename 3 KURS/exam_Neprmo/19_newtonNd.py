import numpy as np
import sys
from numpy.linalg import norm

np.seterr(all='warn')


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


def H(x0, tol, df):
    n = len(x0)
    H = np.zeros((n, n))
    delta = 0.1 * tol

    df_x0 = df(x0)
    df_values = []
    for i in range(n):
        new_x = x0.copy()
        new_x[i] += delta
        df_values.append(df(new_x))

    for i in range(n):
        for j in range(i, n):
            H[i, j] = (df_values[j][i] - df_x0[i]) / delta
            if i != j:
                H[j, i] = H[i, j]

    return H


def nsearch(f, df, x0, tol):
    kmax = 1000
    k, neval = 0, 0
    a = 0.5

    x_k = np.array(x0, dtype=float)
    coords = [x_k.copy()]
    deltaX = np.ones_like(x_k)

    while norm(deltaX) >= tol and k < kmax:
        g0 = df(x_k)
        neval += 1

        H0 = H(x_k, tol, df)
        neval += len(x0) + 1

        deltaX = np.linalg.solve(-H0, g0)
        x_new = x_k + a*deltaX
        x_k = x_new

        coords.append(x_k.copy())
        k += 1

    xmin = x_k
    fmin = f(xmin)
    neval += 1
    print("xmin = ", xmin, "Fmin = ", fmin)
    return [xmin, fmin, neval, coords]

if __name__ == "__main__":
    nsearch(fH, dfH, [2,3], 1e-4)
