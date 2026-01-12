import numpy as np

def f(x):
    return x**2 - 10 * np.cos(0.3 * np.pi * x) - 20

def df(x):
    return  2*x + 3* np.pi*np.sin(0.3 * np.pi * x)

def ddf(x):
    return 2 + 0.9* np.pi* np.pi*np.cos(0.3 * np.pi * x)

def nsearch(tol, x0):
    coords = [x0]
    neval = 0
    k = 1
    x_prev = x0
    while (1):
        df_xprev = df(x_prev)
        ddf_xprev = ddf(x_prev)
        x = x_prev - (df_xprev/ddf_xprev)
        df_x = df(x)
        neval += 3 # т.к. вычисление производной целевой функции в общем случае может выполняться разными способами (численно или аналитически)
        coords.append(x)
        if ((np.abs(df_x)) < tol):
            xmin = x
            fmin = f(xmin)
            break
        else:
            k += 1
            x_prev = x
    answer_ = [xmin, fmin, neval, coords]
    print("xmin = ", xmin, "Fmin = ", fmin)
    return answer_

if __name__ == "__main__":
    nsearch(0.00000001, 1.3)