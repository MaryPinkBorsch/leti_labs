import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rcParams
from tqdm import tqdm
import warnings
warnings.filterwarnings('ignore')

rcParams['figure.figsize'] = (10, 6)
rcParams['font.size'] = 11
rcParams['axes.grid'] = True


# 1. СИСТЕМА SPROTT CASE L

def sprott_L(state, a=1.0, b=1.0, c=1.0):
    """
        dx/dt = a*y
        dy/dt = -b*x + y*z
        dz/dt = c - y**2
    """
    x, y, z = state
    return np.array([a*y, -b*x + y*z, c - y*y])


# 2. МЕТОДЫ


def euler(f, x0, h, N, **params):
    xs = np.zeros((N, 3)); x = np.array(x0, float); xs[0] = x
    for i in range(1, N):
        x = x + h*f(x, **params); xs[i] = x
    return xs

def euler_cromer(f, x0, h, N, **params):
    xs = np.zeros((N, 3)); x = np.array(x0, float); xs[0] = x
    a = params.get('a', 1.0); b = params.get('b', 1.0); c = params.get('c', 1.0)
    for i in range(1, N):
        x_new = x[0] + h*(a*x[1])
        y_new = x[1] + h*(-b*x_new + x[1]*x[2])
        z_new = x[2] + h*(c - y_new*y_new)
        x = np.array([x_new, y_new, z_new]); xs[i] = x
    return xs

def midpoint(f, x0, h, N, **params):
    xs = np.zeros((N, 3)); x = np.array(x0, float); xs[0] = x
    for i in range(1, N):
        k1 = f(x, **params); k2 = f(x + 0.5*h*k1, **params)
        x = x + h*k2; xs[i] = x
    return xs

def rk4(f, x0, h, N, **params):
    xs = np.zeros((N, 3)); x = np.array(x0, float); xs[0] = x
    for i in range(1, N):
        k1 = f(x, **params)
        k2 = f(x + 0.5*h*k1, **params)
        k3 = f(x + 0.5*h*k2, **params)
        k4 = f(x + h*k3, **params)
        x = x + (h/6.0)*(k1 + 2*k2 + 2*k3 + k4); xs[i] = x
    return xs

# ---------------- CD: Composition D-method -------------------
def composition_d(f, x0, h, N, s=0.5, **params):
    """
    Композиционный диагональный метод CD (методичка).
    Один шаг = полушаг Эйлера-Кромера длины h1 = h*s
             + полушаг D-метода (диагонально-неявный) длины h2 = h*(1-s).
    Для системы Sprott Case L:
        dx/dt = a*y
        dy/dt = -b*x + y*z
        dz/dt = c - y*y
    """
    a = params.get('a', 1.0); b = params.get('b', 1.0); c = params.get('c', 1.0)
    h1 = h*s; h2 = h*(1.0 - s)
    eps = 1e-12

    xs = np.zeros((N, 3)); x = np.array(x0, float); xs[0] = x
    for i in range(1, N):
        xn, yn, zn = x[0], x[1], x[2]

        # --- 1) Полушаг Эйлера-Кромера (h1) ---
        x_s = xn + h1*(a*yn)
        y_s = yn + h1*(-b*x_s + yn*zn)
        z_s = zn + h1*(c - y_s*y_s)

        # --- 2) Полушаг D-метода (h2), разрешение неявности ---
        # z: неявность по z? (c - y^2) от z не зависит → явно
        z_new = z_s + h2*(c - y_s*y_s)
        # y: y' = -b*x_s + y*z_new, где y — неявно
        denom_y = 1.0 - h2*z_new
        if abs(denom_y) < eps:
            # fallback: повторить явно, как при "неразрешённой неявности"
            y_new = y_s + h2*(-b*x_s + y_s*z_new)
        else:
            y_new = (y_s - h2*b*x_s) / denom_y
        # x: x' = a*y_new, от x не зависит → явно
        x_new = x_s + h2*(a*y_new)

        x = np.array([x_new, y_new, z_new]); xs[i] = x
    return xs

METHODS = {
    'Euler': euler,
    'Euler-Cromer': euler_cromer,
    'Midpoint': midpoint,
    'CD': composition_d,
    'RK4': rk4
}


# 3. LLE (метод Бенеттина)
#     Для CD — согласованная эволюция отклонения через
#     композицию двух линеаризованных полушагов.


def largest_lyapunov(f, x0, h, N, method='RK4', params=None,
                     n_transient=2000, renorm_every=10, s=0.5):
    if params is None: params = {}
    x = np.array(x0, float)
    d = np.array([1.0, 0.0, 0.0]); d /= np.linalg.norm(d)

    step = _step_funcs[method]

    for _ in range(n_transient):
        x = step(f, x, h, params, s=s)

    lyap_sum = 0.0; count = 0
    for i in range(N):
        J = jacobian(x, params)
        d = _evolve_deviation(method, J, d, h, params, x, s=s)
        x = step(f, x, h, params, s=s)
        if (i + 1) % renorm_every == 0:
            n = np.linalg.norm(d)
            if n > 0:
                lyap_sum += np.log(n); count += 1; d /= n
    if count == 0: return 0.0
    return lyap_sum / (count * renorm_every * h)

def jacobian(x, params):
    a = params.get('a', 1.0); b = params.get('b', 1.0)
    yy, zz = x[1], x[2]
    return np.array([[0.0,  a,     0.0],
                     [-b,   zz,    yy ],
                     [0.0, -2*yy,  0.0]])

# --- одношаговые интеграторы (для LLE) ---

def _step_euler(f, x, h, p, s=0.5):
    return x + h*f(x, **p)

def _step_euler_cromer(f, x, h, p, s=0.5):
    a = p.get('a', 1.0); b = p.get('b', 1.0); c = p.get('c', 1.0)
    xn, yn, zn = x
    x_new = xn + h*(a*yn)
    y_new = yn + h*(-b*x_new + yn*zn)
    z_new = zn + h*(c - y_new*y_new)
    return np.array([x_new, y_new, z_new])

def _step_midpoint(f, x, h, p, s=0.5):
    k1 = f(x, **p); k2 = f(x + 0.5*h*k1, **p)
    return x + h*k2

def _step_rk4(f, x, h, p, s=0.5):
    k1 = f(x, **p)
    k2 = f(x + 0.5*h*k1, **p)
    k3 = f(x + 0.5*h*k2, **p)
    k4 = f(x + h*k3, **p)
    return x + (h/6.0)*(k1 + 2*k2 + 2*k3 + k4)

def _step_cd_comp(f, x, h, p, s=0.5):
    """Один шаг композиционного диагонального метода (для LLE)."""
    a = p.get('a', 1.0); b = p.get('b', 1.0); c = p.get('c', 1.0)
    h1 = h*s; h2 = h*(1.0 - s); eps = 1e-12
    xn, yn, zn = x
    x_s = xn + h1*(a*yn)
    y_s = yn + h1*(-b*x_s + yn*zn)
    z_s = zn + h1*(c - y_s*y_s)
    z_new = z_s + h2*(c - y_s*y_s)
    denom_y = 1.0 - h2*z_new
    if abs(denom_y) < eps:
        y_new = y_s + h2*(-b*x_s + y_s*z_new)
    else:
        y_new = (y_s - h2*b*x_s) / denom_y
    x_new = x_s + h2*(a*y_new)
    return np.array([x_new, y_new, z_new])

_step_funcs = {
    'Euler': _step_euler,
    'Euler-Cromer': _step_euler_cromer,
    'Midpoint': _step_midpoint,
    'CD': _step_cd_comp,
    'RK4': _step_rk4
}

def _evolve_deviation(method, J, d, h, params, x, s=0.5):
    """
    Эволюция вектора отклонения линеаризованной схемой.
    Для CD используем ту же композицию:
        полушаг Эйлера-Кромера (линеаризованный)
      + полушаг D-метода (линеаризованный).
    Для остальных — стандартно.
    """
    if method == 'Euler':
        return d + h*(J @ d)
    if method == 'Euler-Cromer':
        return d + h*(J @ d)
    if method == 'Midpoint':
        k1 = J @ d; k2 = J @ (d + 0.5*h*k1)
        return d + h*k2
    if method == 'RK4':
        k1 = J @ d
        k2 = J @ (d + 0.5*h*k1)
        k3 = J @ (d + 0.5*h*k2)
        k4 = J @ (d + h*k3)
        return d + (h/6.0)*(k1 + 2*k2 + 2*k3 + k4)
    if method == 'CD':
        # Линеаризация композиции:
        # 1) полушаг ЭК — это ЭК-шаг длины h1 по линеаризации.
        #    В линеаризованной схеме ЭК = Эйлер (порядок 1 для отклонения).
        #    Чтобы сохранить согласованность, применяем последовательно
        #    линеаризованные полушаги с одинаковой структурой:
        h1 = h*s; h2 = h*(1.0 - s)
        # полушаг 1 (линеаризованный Эйлер-Кромер ≈ явный Эйлер по J)
        d1 = d + h1*(J @ d)
        # полушаг 2 (линеаризованный D-метод): диагонально-неявный по той же J
        # Разрешаем (I - h2 * diag(J)) d2 = d1 + h2 * (J - diag(J)) d2
        # проще: строим нижнетреугольную матрицу с диагональю как у (I - h2*J)
        # Из-за того, что в Sprott L диагональ J = (0, z, 0) и в нижней части
        # есть связь y<-x, самый простой согласованный вариант —
        # использовать полную неявную схему Эйлера для линеаризации:
        M = np.eye(3) - h2*J
        try:
            d2 = np.linalg.solve(M, d1)
        except np.linalg.LinAlgError:
            d2 = d1 + h2*(J @ d1)
        return d2
    return d


# 4. ГРАФИКИ (без изменений по структуре)


def plot_comparison_trajectories(x0=(0.1, 0.1, 0.1), h=0.01, T=50):
    N = int(T / h); t = np.arange(N)*h
    fig, axes = plt.subplots(5, 1, figsize=(11, 14))
    ref = rk4(sprott_L, x0, h, N)
    for ax, (name, method) in zip(axes, METHODS.items()):
        traj = method(sprott_L, x0, h, N)
        ax.plot(t, ref[:, 0], 'k-', lw=1.5, label='RK4 (эталон)', alpha=0.6)
        ax.plot(t, traj[:, 0], 'r--', lw=1.0, label=name)
        ax.set_ylabel('x(t)'); ax.legend(loc='upper right')
        ax.set_title(f'Сравнение {name} с RK4 (x-компонента)')
    axes[-1].set_xlabel('t')
    plt.tight_layout(); plt.savefig('comparison_trajectories.png', dpi=100); plt.show()

def plot_lle_vs_param(param_name='a', param_range=None, h=0.01, N=3000, n_transient=1000):
    if param_range is None:
        param_range = np.linspace(0.5, 2.0, 30)
    fig, ax = plt.subplots(figsize=(10, 6))
    colors = plt.cm.tab10(np.linspace(0, 1, len(METHODS)))
    for (name, _), color in zip(METHODS.items(), colors):
        lles = []
        for val in tqdm(param_range, desc=f'LLE vs {param_name} [{name}]'):
            params = {'a': 1.0, 'b': 1.0, 'c': 1.0}; params[param_name] = val
            lles.append(largest_lyapunov(sprott_L, (0.1, 0.1, 0.1), h, N,
                                         method=name, params=params,
                                         n_transient=n_transient))
        ax.plot(param_range, lles, 'o-', color=color, label=name, ms=3)
    ax.axhline(0, color='k', lw=0.5, ls=':')
    ax.set_xlabel(f'Параметр {param_name}'); ax.set_ylabel('LLE')
    ax.set_title(f'Зависимость LLE от параметра {param_name}')
    ax.legend(); plt.tight_layout()
    plt.savefig(f'lle_vs_{param_name}.png', dpi=100); plt.show()

def plot_lle_vs_h(h_range=None, N=5000, n_transient=1000):
    if h_range is None: h_range = np.logspace(-4, -1, 15)
    fig, ax = plt.subplots(figsize=(10, 6))
    colors = plt.cm.tab10(np.linspace(0, 1, len(METHODS)))
    for (name, _), color in zip(METHODS.items(), colors):
        lles = []
        for h in tqdm(h_range, desc=f'LLE vs h [{name}]'):
            lles.append(largest_lyapunov(sprott_L, (0.1, 0.1, 0.1), h, N,
                                         method=name, n_transient=n_transient))
        ax.semilogx(h_range, lles, 'o-', color=color, label=name, ms=4)
    ax.axhline(0, color='k', lw=0.5, ls=':')
    ax.set_xlabel('Шаг h'); ax.set_ylabel('LLE')
    ax.set_title('Зависимость LLE от шага h'); ax.legend()
    plt.tight_layout(); plt.savefig('lle_vs_h.png', dpi=100); plt.show()

def plot_lle_vs_h_vs_rk4(h_range=None, N=5000, n_transient=1000):
    if h_range is None: h_range = np.logspace(-4, -1, 15)
    rk4_vals = [largest_lyapunov(sprott_L, (0.1,0.1,0.1), h, N,
                                 method='RK4', n_transient=n_transient)
                for h in tqdm(h_range, desc='RK4 baseline')]
    rk4_vals = np.array(rk4_vals)
    others = [m for m in METHODS if m != 'RK4']
    fig, axes = plt.subplots(len(others), 1, figsize=(10, 3.5*len(others)))
    for ax, name in zip(axes, others):
        lles = [largest_lyapunov(sprott_L, (0.1,0.1,0.1), h, N,
                                 method=name, n_transient=n_transient)
                for h in tqdm(h_range, desc=f'{name} vs h')]
        ax.semilogx(h_range, rk4_vals, 'k-', lw=2, label='RK4 (эталон)')
        ax.semilogx(h_range, lles, 'r--o', label=name, ms=4)
        ax.set_xlabel('h'); ax.set_ylabel('LLE')
        ax.set_title(f'{name} vs RK4: LLE(h)'); ax.legend()
    plt.tight_layout(); plt.savefig('lle_vs_h_comparison_rk4.png', dpi=100); plt.show()

def plot_2d_diagram(param1='a', param2='b', n_grid=100, h=0.01,
                    N=2500, n_transient=1000):
    """Пункт 4: 2D карта LLE по двум параметрам (всеми методами).
    n_grid — количество точек сетки по каждой оси (n_grid × n_grid)."""
    p1_vals = np.linspace(0.5, 1.5, n_grid)
    p2_vals = np.linspace(0.5, 1.5, n_grid)
    for name in METHODS:
        lle_map = np.zeros((n_grid, n_grid))
        for i, v1 in enumerate(tqdm(p1_vals, desc=f'2D LLE [{name}] {param1}')):
            for j, v2 in enumerate(p2_vals):
                params = {'a': 1.0, 'b': 1.0, 'c': 1.0}
                params[param1] = v1; params[param2] = v2
                lle_map[i, j] = largest_lyapunov(
                    sprott_L, (0.1,0.1,0.1), h, N, method=name,
                    params=params, n_transient=n_transient)
        fig, ax = plt.subplots(figsize=(8, 6))
        im = ax.imshow(lle_map, origin='lower', cmap='RdBu_r',
                       extent=[p2_vals[0], p2_vals[-1], p1_vals[0], p1_vals[-1]],
                       aspect='auto', interpolation='nearest')  # ← чёткие пиксели
        plt.colorbar(im, ax=ax, label='LLE')
        ax.set_xlabel(param2); ax.set_ylabel(param1)
        ax.set_title(f'2D карта LLE ({name}): {param1} vs {param2} [{n_grid}×{n_grid}]')
        plt.tight_layout(); plt.savefig(f'2d_lle_{name}_{param1}_{param2}.png', dpi=100)
        plt.show()

def plot_2d_lle_vs_h(param='a', n_grid=100, h_range=None,
                     N=3000, n_transient=800):
    """Пункт 5: 2D карта LLE = LLE(param, h) для всех методов.
    n_grid — количество точек по оси параметра; столько же — по оси log10(h),
    если h_range=None."""
    if h_range is None:
        h_range = np.logspace(-4, -1.5, n_grid)
    param_vals = np.linspace(0.5, 1.5, n_grid)
    for name in METHODS:
        lle_map = np.zeros((n_grid, n_grid))
        for i, v in enumerate(tqdm(param_vals, desc=f'2D LLE {param}-h [{name}]')):
            for j, h in enumerate(h_range):
                params = {'a': 1.0, 'b': 1.0, 'c': 1.0}; params[param] = v
                lle_map[i, j] = largest_lyapunov(
                    sprott_L, (0.1,0.1,0.1), h, N, method=name,
                    params=params, n_transient=n_transient)
        fig, ax = plt.subplots(figsize=(9, 6))
        im = ax.imshow(lle_map, origin='lower', cmap='RdBu_r',
                       extent=[np.log10(h_range[0]), np.log10(h_range[-1]),
                               param_vals[0], param_vals[-1]],
                       aspect='auto', interpolation='nearest')  # ← чёткие пиксели
        plt.colorbar(im, ax=ax, label='LLE')
        ax.set_xlabel('log10(h)'); ax.set_ylabel(f'Параметр {param}')
        ax.set_title(f'2D карта LLE ({name}): {param} vs h [{n_grid}×{len(h_range)}]')
        plt.tight_layout(); plt.savefig(f'2d_lle_vs_h_{name}_{param}.png', dpi=100)
        plt.show()


# 5. Проверка

def run_quick_demo():
    print("=== Быстрая проверка (h=0.01, N=3000) ===")
    for name in METHODS:
        lle = largest_lyapunov(sprott_L, (0.1,0.1,0.1), 0.01, 3000,
                               method=name, n_transient=1000)
        print(f"{name:15s}: LLE = {lle:+.5f}")

if __name__ == '__main__':
    import os
    os.makedirs('lab3_out', exist_ok=True)
    os.chdir('lab3_out')

    # Пункт 2 — сравнение траекторий и LLE от параметров
    plot_comparison_trajectories(h=0.01, T=50)
    plot_lle_vs_param('a', np.linspace(0.5, 2.0, 20), h=0.01, N=2000)
    plot_lle_vs_param('b', np.linspace(0.5, 2.0, 20), h=0.01, N=2000)
    plot_lle_vs_param('c', np.linspace(0.5, 2.0, 20), h=0.01, N=2000)

    # Пункт 3 — LLE от h
    plot_lle_vs_h(np.logspace(-4, -1, 12), N=3000)
    plot_lle_vs_h_vs_rk4(np.logspace(-4, -1, 12), N=3000)

    # Пункт 4 — 2D-карты 100×100 (уменьшили N и n_transient для скорости)
    plot_2d_diagram('a', 'b', n_grid=100, h=0.01, N=1200, n_transient=500)

    # Пункт 5 — 2D-карты (100 по параметру × 100 по h)
    plot_2d_lle_vs_h('a', n_grid=100,
                     h_range=np.logspace(-4, -1.5, 100),
                     N=1200, n_transient=500)

    print("Готово. Файлы в lab3_out/")