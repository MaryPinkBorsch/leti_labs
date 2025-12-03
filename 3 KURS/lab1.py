import numpy as np

def f(x): return 2 * (x ** 2) - 9 * x - 31
def df(x): return 4 * x - 9


def bsearch(interval, tol):
    a, b = interval
    coords = []
    neval = 0

    L = b - a
    g = df(a)
    neval += 1

    while (np.abs(L) > tol) and (np.abs(g) > tol):

        x_k = (a + b) / 2
        coords.append(x_k)
        g_xk = df(x_k)
        neval+=1

        if g_xk > 0:
            b = x_k
        else:
            a = x_k
            g = g_xk

        L = b - a

    xmin = (a + b) / 2
    fmin = f(xmin)
    neval += 1

    answer_ = [xmin, fmin, neval, coords]

    print("finished, xmin =", xmin, "  fmin = ", fmin)

    return answer_

if __name__ == "__main__":
    bsearch([0,10],0.00000001)
    
# searches for minimum using bisection method
# arguments: bisectionsearch(f,df,interval,tol)
# f - an objective function
# df -  an objective function derivative
# interval = [a, b] - search interval
#tol - tolerance for both range and function value
# output: [xmin, fmin, neval, coords]
# xmin - value of x in fmin
# fmin - minimul value of f
# neval - number of function evaluations
# coords - array of x values found during optimization