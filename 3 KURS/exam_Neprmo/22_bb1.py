import numpy as np
import sys
from numpy.linalg import norm


def goldensectionsearch(f, interval, tol):
    a, b = interval
    neval = 0

    fi = (1 + np.sqrt(5)) / 2
    L = b - a

    x_left = b - L / fi
    x_right = a + L / fi

    f_right = f(x_right)
    f_left = f(x_left)
    neval += 2

    while (np.abs(L) > tol) and (np.abs(f_left - f_right) > tol):

        if f_left > f_right:
            a = x_left
            x_left, f_left = x_right, f_right
            L = b - a
            x_right = a + L / fi
            f_right = f(x_right)
        else:
            b = x_right
            x_right, f_right = x_left, f_left
            L = b - a
            x_left = b - L / fi
            f_left = f(x_left)
        neval += 1
    xmin = (a + b) / 2
    fmin = f(xmin)
    neval += 1

    answer_ = [xmin, fmin, neval]
    return answer_


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

def bbsearch(f, df, x0, tol):
    kmax = 1000
    D = 0.1

    k, neval = 0, 0
    x_k = np.array(x0, dtype=float)
    coords = [x_k.copy()]

    grad = df(x_k)
    neval += 1

    phi = lambda alpha: f(x_k - alpha * grad)
    alpha0, fmin, neval_g = goldensectionsearch(phi, [0, 1], tol)
    neval += neval_g

    x_prev = x_k.copy()
    x_k = x_k - alpha0 * grad
    coords.append(x_k.copy())

    deltaX = x_k - x_prev
    while norm(deltaX) >= tol and k < kmax:
        g = df(x_k)
        neval += 1

        deltaX = x_k - x_prev
        delta_g = g - grad

        alph_bb = np.dot(deltaX.T[0], deltaX.T[0]) / np.dot(deltaX.T[0], delta_g.T[0])
        #alph_bb = np.dot(deltaX.T[0], delta_g.T[0]) / np.dot(delta_g.T[0], delta_g.T[0])

        alph_stab = D / norm(g)
        final_alph = min([alph_bb, alph_stab])

        x_prev = x_k.copy()
        x_k = x_k - final_alph * g
        grad = g.copy()

        coords.append(x_k.copy())
        deltaX = x_k - x_prev

        k += 1

    xmin = x_k
    fmin = f(xmin)
    neval += 1

    print("xmin = ", xmin, "Fmin = ", fmin)
    answer_ = [xmin, fmin, neval, coords]
    return answer_


if __name__ == "__main__":
    bbsearch(fH, dfH, [1,1], 1e-4)