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


def NNsearch(f, df, x0, tol):
   kmax = 1000
   x_k = x0.copy()
   coords = [x_k.copy()]
   neval = 0
   k = 0
   deltaX = np.ones_like(x_k) * np.inf


   while (norm(deltaX) >= tol) and (k < kmax):
       g0 = df(x_k)
       neval += 1


       H0 = H(x_k, tol, df)
       neval += 2 * len(x0)


       v = np.linalg.solve(H0, g0)
       lam = np.sqrt(abs((g0.T @ v).item()))
       if lam <= 0.25:
           alpha = 1.0
       else:
           alpha = 1.0 / (1.0 + lam)
       dx = -alpha * v
       x_new = x_k + dx
       deltaX = x_new - x_k
       x_k = x_new
       coords.append(x_k.copy())
       k += 1


   xmin = x_k
   fmin = f(xmin)
   answer_ = [xmin, fmin, neval, coords]
   print("xmin = ", xmin, "Fmin = ", fmin)
   return answer_


if __name__ == "__main__":
    NNsearch(fR, dfR, [-2,-2], 1e-4)