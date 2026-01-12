import numpy as np
import matplotlib.pyplot as plt

def f(x):
    return 2*(x)**2 -9*x -31

def bsearch(interval, tol):
   a, b = interval
   coords = []


   x_m = (a+b)/2
   fm = f(x_m)
   neval = 1
   while np.abs(b - a) > tol:
       # выбираем две внутренние точки
       x1 = a + (b - a) / 4
       x2 = b - (b - a) / 4
       f1 = f(x1)
       f2 = f(x2)
       # fm = f(x_m)
       coords.append(x_m)
       neval+=2
       # если f(x1) < f(x2), минимум ближе к a → x2
       if f1 < fm:
           b = x_m
           x_m = x1
           fm = f1
       elif f1 >= fm and fm<= f2:
           a = x1
           b = x2
       elif fm > f2:
           a = x_m
           x_m = x2
           fm = f2




   xmin = x_m
   fmin = fm
   print("Xmin = ",xmin,"Fmin = ", f(x_m))


   answer_ = [xmin, fmin, neval, coords]
   return answer_


if __name__ == "__main__":
    res = bsearch([-2,10], 1e-4)