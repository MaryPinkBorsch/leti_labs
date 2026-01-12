import numpy as np

def f(X):
    x = X[0]
    y = X[1]
    v = (x**2 + y - 11)**2 + (x + y** 2 - 7)**2
    return v
   
def df(X):
    x = X[0]
    y = X[1]
    v = np.copy(X)
    v[0] = 2 * (x**2 + y - 11) * (2 * x) + 2 * (x + y**2 - 7)
    v[1] = 2 * (x**2 + y - 11) + 2 * (x + y**2 - 7) * (2 * y)
    return v
def grsearch(x0, tol):
    al = 0.01
    k = 1
    kmax = 1000
    x_prev = x0
    coords = [x0]
    neval = 0
    while (1):
        g = -df(x_prev)
        neval += 1
        x = x_prev + al*g
        coords.append(x)
        deltaX = x - x_prev
        if ((np.linalg.norm(deltaX) < tol) or (k >= kmax)):
            xmin = x
            fmin = f(xmin)
            break
        else:
            k += 1
            x_prev = x
            
    print("Xmin = ",xmin,"Fmin = ", fmin)
    
if __name__ == "__main__":
    grsearch([2,3], 1e-4)