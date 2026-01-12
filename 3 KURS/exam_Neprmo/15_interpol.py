import numpy as np
import matplotlib.pyplot as plt
import os

# Функция и её производная
def f(x):
    return x**2 - 10*np.cos(0.3*np.pi*x) - 20
def g(x):
    return 2*x + 3*np.pi*np.sin(0.3*np.pi*x)

# Метод обратной параболической интерполяции
def inverse_parabola_method(x0, x1, x2, eps=1e-6, kmax=50):
    coords = []
    neval = 0
    for k in range(kmax):
        y0, y1, y2 = g(x0), g(x1), g(x2)
        def P(y):
            L0 = ((y - y1)*(y - y2)) / ((y0 - y1)*(y0 - y2))
            L1 = ((y - y0)*(y - y2)) / ((y1 - y0)*(y1 - y2))
            L2 = ((y - y0)*(y - y1)) / ((y2 - y0)*(y2 - y1))
            return x0*L0 + x1*L1 + x2*L2
        x3 = P(0)

        coords.append((x0, x1, x2, x3))
        neval += 1
        if abs(x3 - x2) <= eps:
            x_min = x3
            return [x_min, f(x_min), neval, coords]
        x0, x1, x2 = x1, x2, x3
    x_min = x2   
    return [x_min, f(x_min), neval, coords]

# Сохранение графиков в PNG
def save_iteration_plot_inverse(x0, x1, x2, x3, k, folder_path):
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)
    xg = np.linspace(-2, 5, 600)
    y0, y1, y2 = g(x0), g(x1), g(x2)
    def P(y):
        L0 = ((y - y1)*(y - y2)) / ((y0 - y1)*(y0 - y2))
        L1 = ((y - y0)*(y - y2)) / ((y1 - y0)*(y1 - y2))
        L2 = ((y - y0)*(y - y1)) / ((y2 - y0)*(y2 - y1))
        return x0*L0 + x1*L1 + x2*L2

    yp = np.linspace(min(y0, y2)-0.5, max(y0, y2)+0.5, 300)
    parabola = P(yp)
    fig, axes = plt.subplots(1, 2, figsize=(14,5))
    axes[0].plot(xg, g(xg), label="g(x)=f'(x)", linewidth=2)
    axes[0].scatter([x0, x1, x2], [y0, y1, y2], color="red", label="Опорные точки")
    axes[0].plot(parabola, yp, linestyle="--", label="Обратная парабола")
    axes[0].scatter(x3, 0, color="black", s=70, label=r"$x_{k+1}$")
    axes[0].axhline(0, color="gray", linewidth=0.8)
    axes[0].set_title(f"Обратная парабола, итерация k={k}")
    axes[0].set_xlabel("x")
    axes[0].set_ylabel("g(x)")
    axes[0].legend()
    axes[0].grid(True)
    axes[1].plot(xg, f(xg), label="f(x)", linewidth=2)
    axes[1].scatter([x0, x1, x2, x3], [f(x0), f(x1), f(x2), f(x3)],
                    color=["red","red","red","black"])
    axes[1].set_title("f(x)")
    axes[1].legend()
    axes[1].grid(True)
    plt.tight_layout()
    filename = os.path.join(folder_path, f"iteration_{k}.png")
    plt.savefig(filename)
    plt.close()

if __name__ == "__main__":
    save_folder = r"15_interpol_study_png"
    answer_ = inverse_parabola_method(-2.0, 1.0, -0.5)
    xmin, fmin, neval, coords = answer_
    print(f"x_min = {xmin:.10f}")
    print(f"f(x_min) = {fmin:.10f}")
    print(f"Число итераций = {neval}\n")
    print("Промежуточные точки:")
    for k, (x0, x1, x2, x3) in enumerate(coords):
        print(f"k={k}: x0={x0:.6f}, x1={x1:.6f}, x2={x2:.6f}, x3={x3:.6f}")
        save_iteration_plot_inverse(x0, x1, x2, x3, k, save_folder)
    print(f"Графики сохранены в папке: {save_folder}")
