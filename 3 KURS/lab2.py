import numpy as np

def f(x): return (x - 3) ** 2 - 3 * x + x ** 2 - 40

def gsearch(interval, tol):
    a, b = interval
    coords = []
    F_const = (1 + np.sqrt(5)) / 2

    x1 = b - (b - a) / F_const
    x2 = a + (b - a) / F_const
    f1 = f(x1)
    f2 = f(x2)
    neval = 2
    while (np.abs(b - a) > tol) and np.abs(f2 - f1) > tol:
        coords.append([x1, x2, a, b])
        if f1 > f2:
            a = x1
            x1 = x2
            x2 = a + (b - a) / F_const
            f1 = f2
            f2 = f(x2)
            neval += 1
        else:
            b = x2
            x2 = x1
            x1 = b - (b - a) / F_const
            f2 = f1
            f1 = f(x1)
            neval += 1
    coords.append([x1, x2, a, b])
    xmin = (a + b) / 2
    fmin = f(xmin)
    neval += 1
    print("xmin ", xmin)
    answer_ = [xmin, fmin, neval, coords]
    return answer_


if __name__ == "__main__":
    gsearch([0,10],0.00000001)


# GOLDENSECTIONSEARCH searches for minimum using golden section
# 	[xmin, fmin, neval] = GOLDENSECTIONSEARCH(f,interval,tol)
#   INPUT ARGUMENTS
# 	f is a function
# 	interval = [a, b] - search interval
# 	tol - set for bot range and function value
#   OUTPUT ARGUMENTS
# 	xmin is a function minimizer
# 	fmin = f(xmin)
# 	neval - number of function evaluations
#   coords - array of statistics,  coord[i][:] =  [x1,x2, a, b]