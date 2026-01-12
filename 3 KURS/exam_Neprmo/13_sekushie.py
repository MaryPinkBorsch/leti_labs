import numpy as np
def f(x):
    return x**2 - 10 * np.cos(0.3 * np.pi * x) - 20

def df(x):
    return  2*x + 3* np.pi*np.sin(0.3 * np.pi * x)

def ssearch(interval,tol):
   k_max  = 1000
   k = 0
   a, b = interval
   coords = []
   df_A = df(a)
   df_B = df(b)
   x = b - df_B * (b - a) / (df_B- df_A)
   coords.append([x, a, b])
   neval = 2


   while (np.abs(df_B)) > tol and (np.abs(b - a)) > tol and (k < k_max):
       k += 1
       x = b - df_B * (b-a)/(df_B-df_A)
       df_X = df(x)
       neval += 1
       if df_X >= 0:
           b = x
           df_B = df_X
       else:
           a = x
           df_A = df_X
       coords.append([x,a,b])
   xmin = x
   fmin = f(xmin)
   neval += 1

   print("xmin = ", xmin, "Fmin = ", fmin)
   answer_ = [xmin, fmin, neval, coords]
   return answer_


if __name__ == "__main__":
    ssearch([-2,7],0.00000001)
