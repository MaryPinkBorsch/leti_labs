
import numpy as np

def f(x): return x**2 - 10*np.cos(0.3*np.pi*x) - 20
def df(x): return 2*x + 3*np.pi*np.sin(0.3*np.pi*x)
def ddf(x): return 2 + 0.9*(np.pi**2)*np.cos(0.3*np.pi*x)


def nsearch(tol, x0):
    coords = []
    neval = 0
    x_k = x0
    coords.append(x_k)
    max_i = 100
    i = 0 

    df_xk = df(x_k)
    ddf_xk = ddf(x_k)
    neval += 2

    while ((np.abs(df_xk)) > tol) and (i < max_i):

        x_k = x_k - (df_xk / ddf_xk)
        coords.append(x_k)
        df_xk = df(x_k)
        ddf_xk = ddf(x_k)
        neval += 2
        i += 1

    xmin = x_k
    fmin = f(x_k)
    neval+=1
    answer_ = [xmin, fmin, neval, coords]
    return answer_




if __name__ == "__main__":
    nsearch([0,10],0.00000001)