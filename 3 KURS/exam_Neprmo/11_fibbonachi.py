import numpy as np

def f(x):
    return (x - 3) ** 2 - 3 * x + x ** 2 - 40

def fibonacci_search(interval, tol, delta=1e-5):
    a, b = interval
    coord = []
    neval = 0

    fib = [1, 1]
    while fib[-1] < (b - a) / tol:
        fib.append(fib[-1] + fib[-2])
    n = len(fib) - 1

    x_left  = a + fib[n - 2] / fib[n] * (b - a)
    x_right = a + fib[n - 1] / fib[n] * (b - a)

    f_left = f(x_left)
    f_right = f(x_right)
    neval += 2

    for k in range(1, n - 1):
        coord.append([x_left, x_right, a, b])

        if f_left < f_right:
            b = x_right
            x_right = x_left
            f_right = f_left
            x_left = a + fib[n - k - 2] / fib[n - k] * (b - a)
            f_left = f(x_left)
        else:
            a = x_left
            x_left = x_right
            f_left = f_right
            x_right = a + fib[n - k - 1] / fib[n - k] * (b - a)
            f_right = f(x_right)

        neval += 1

    coord.append([x_left, x_right, a, b])


    mu = x_left + delta
    if f_left > f(mu):
        xmin = (x_left + b) / 2
    else:
        xmin = (a + mu) / 2
    fmin = f(xmin)
    neval += 1
    print("xmin = ", xmin, "Fmin = ", fmin)
    return [xmin, fmin, neval, coord]

if __name__ == "__main__":
    fibonacci_search([0,10],0.00000001)