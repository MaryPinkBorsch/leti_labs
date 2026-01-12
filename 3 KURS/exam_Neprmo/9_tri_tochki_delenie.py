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

def three_point_search_slides(interval, history):
    x = np.linspace(interval[0], interval[1], 1000)
    y = f(x)

    nslides = min(9, len(history))

    for i in range(nslides):
        h = history[i]

        plt.figure(figsize=(8, 5))
        plt.plot(x, y, label="f(x)")

        plt.axvline(h["a"], color="black", linestyle="--")
        plt.axvline(h["b"], color="black", linestyle="--")

        plt.axvline(h["x1"], color="orange", linestyle=":")
        plt.axvline(h["x2"], color="orange", linestyle=":")
        plt.axvline(h["xm"], color="red")

        plt.scatter(
            [h["x1"], h["xm"], h["x2"]],
            [h["f1"], h["fm"], h["f2"]],
            color=["orange", "red", "orange"],
            zorder=5
        )

        plt.title(f"Iteration {i+1}")
        plt.xlabel("x")
        plt.ylabel("f(x)")
        plt.legend()
        plt.grid(True)

        plt.savefig(f"plot_iter_{i+1}.png")
        plt.close()


    h = history[-1]
    plt.figure(figsize=(8, 5))
    plt.plot(x, y, label="f(x)")
    plt.axvline(h["xm"], color="blue", linewidth=2, label="xmin")
    plt.scatter(h["xm"], h["fm"], color="blue", zorder=5)

    plt.title("Final result")
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.legend()
    plt.grid(True)

    plt.savefig("plot_final.png")
    plt.close()

if __name__ == "__main__":
    res = bsearch([-2,10], 1e-4)