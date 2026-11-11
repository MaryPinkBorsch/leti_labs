"""
Лабораторная работа №4
Система Sprott Case L
Методы: Euler, Euler-Cromer, Midpoint, CD, RK4
Пункты:
    1) V(параметр a, b, c)
    2) V(h)
    3) V(s) для CD (два диапазона s)
    4) Диаграммы продолжения (continuation) при разных h
    5) Бассейны притяжения всеми методами
"""

import os
import numpy as np
import matplotlib
matplotlib.use('Agg')            # убирает Tkinter-сообщения 'delayed_destroy'
import matplotlib.pyplot as plt
from matplotlib import rcParams
from matplotlib.colors import ListedColormap, BoundaryNorm
from tqdm import tqdm
import warnings
warnings.filterwarnings('ignore')

rcParams['figure.figsize'] = (10, 6)
rcParams['font.size'] = 11
rcParams['axes.grid'] = True


# =====================================================================
# 1. СИСТЕМА SPROTT CASE L
# =====================================================================
def sprott_L(state, a=1.0, b=1.0, c=1.0):
    """dx/dt = a*y, dy/dt = -b*x + y*z, dz/dt = c - y^2"""
    x, y, z = state
    return np.array([a*y, -b*x + y*z, c - y*y])


# =====================================================================
# 2. ОДНОШАГОВЫЕ ИНТЕГРАТОРЫ
# =====================================================================
def step_euler(f, x, h, p, s=0.5):
    return x + h*f(x, **p)


def step_euler_cromer(f, x, h, p, s=0.5):
    a = p.get('a', 1.0); b = p.get('b', 1.0); c = p.get('c', 1.0)
    xn, yn, zn = x
    x_new = xn + h*(a*yn)
    y_new = yn + h*(-b*x_new + yn*zn)
    z_new = zn + h*(c - y_new*y_new)
    return np.array([x_new, y_new, z_new])


def step_midpoint(f, x, h, p, s=0.5):
    k1 = f(x, **p); k2 = f(x + 0.5*h*k1, **p)
    return x + h*k2


def step_rk4(f, x, h, p, s=0.5):
    k1 = f(x, **p)
    k2 = f(x + 0.5*h*k1, **p)
    k3 = f(x + 0.5*h*k2, **p)
    k4 = f(x + h*k3, **p)
    return x + (h/6.0)*(k1 + 2*k2 + 2*k3 + k4)


def step_cd_comp(f, x, h, p, s=0.5):
    """CD: полушаг Эйлера-Кромера + неявный D-полушаг.
       h1 = h*s, h2 = h*(1-s)."""
    a = p.get('a', 1.0); b = p.get('b', 1.0); c = p.get('c', 1.0)
    h1 = h*s; h2 = h*(1.0 - s); eps = 1e-12
    xn, yn, zn = x
    # 1-й полушаг (Эйлер-Кромер)
    x_s = xn + h1*(a*yn)
    y_s = yn + h1*(-b*x_s + yn*zn)
    z_s = zn + h1*(c - y_s*y_s)
    # 2-й полушаг (неявный D-метод)
    z_new = z_s + h2*(c - y_s*y_s)
    denom_y = 1.0 - h2*z_new
    if abs(denom_y) < eps:
        y_new = y_s + h2*(-b*x_s + y_s*z_new)
    else:
        y_new = (y_s - h2*b*x_s)/denom_y
    x_new = x_s + h2*(a*y_new)
    return np.array([x_new, y_new, z_new])


STEP_FUNCS = {
    'Euler': step_euler,
    'Euler-Cromer': step_euler_cromer,
    'Midpoint': step_midpoint,
    'CD': step_cd_comp,
    'RK4': step_rk4,
}
METHOD_ORDER = ['Euler', 'Euler-Cromer', 'Midpoint', 'CD', 'RK4']


# =====================================================================
# 3. ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
# =====================================================================
def integrate(f, x0, h, N, method='RK4', params=None, s=0.5):
    """Возвращает массив состояний (N, 3). NaN при расходимости."""
    if params is None:
        params = {}
    xs = np.zeros((N, 3))
    x = np.array(x0, float)
    xs[0] = x
    step = STEP_FUNCS[method]
    for i in range(1, N):
        x = step(f, x, h, params, s=s)
        if not np.all(np.isfinite(x)):
            xs[i:] = np.nan
            break
        xs[i] = x
    return xs


def phase_volume(traj, transient_frac=0.3):
    """V = (xmax-xmin)(ymax-ymin)(zmax-zmin) после транзиента."""
    n0 = int(len(traj)*transient_frac)
    tr = traj[n0:]
    tr = tr[np.all(np.isfinite(tr), axis=1)]
    if len(tr) < 10:
        return np.nan
    xr = tr[:, 0].max() - tr[:, 0].min()
    yr = tr[:, 1].max() - tr[:, 1].min()
    zr = tr[:, 2].max() - tr[:, 2].min()
    return xr*yr*zr


def volume_for(method, a=1.0, b=1.0, c=1.0, h=0.01, N=6000,
               x0=(0.1, 0.1, 0.1), s=0.5, transient_frac=0.3):
    tr = integrate(sprott_L, x0, h, N, method=method,
                   params={'a': a, 'b': b, 'c': c}, s=s)
    return phase_volume(tr, transient_frac)


def largest_lyapunov_simple(method='RK4', a=1.0, b=1.0, c=1.0,
                            h=0.01, N=1000, x0=(0.1, 0.1, 0.1),
                            n_transient=300, s=0.5, renorm=5):
    """
    Устойчивый LLE через расхождение двух близких траекторий
    с реортогонализацией.
    """
    p = {'a': a, 'b': b, 'c': c}
    step = STEP_FUNCS[method]

    x = np.array(x0, float)
    for _ in range(n_transient):
        x = step(sprott_L, x, h, p, s=s)
        if not np.all(np.isfinite(x)) or np.max(np.abs(x)) > 1e6:
            return np.nan

    d0 = 1e-7
    y = x + np.array([d0, 0.0, 0.0])

    log_sum = 0.0
    count = 0

    for i in range(N):
        x = step(sprott_L, x, h, p, s=s)
        y = step(sprott_L, y, h, p, s=s)

        if (not np.all(np.isfinite(x))) or (not np.all(np.isfinite(y))):
            return np.nan
        if np.max(np.abs(x)) > 1e6 or np.max(np.abs(y)) > 1e6:
            return np.nan

        if (i + 1) % renorm == 0:
            d = np.linalg.norm(y - x)
            if not np.isfinite(d) or d < 1e-30:
                # траектории слиплись — подтолкнём по случайной оси
                rnd = np.random.default_rng(seed=i)
                y = x + d0 * rnd.standard_normal(3)
                continue
            log_sum += np.log(d)
            count += 1
            y = x + (y - x) * (d0 / d)

    if count == 0:
        return 0.0
    return log_sum / (count * renorm * h)


# =====================================================================
# ПУНКТ 1. ФАЗОВЫЙ ОБЪЁМ vs ПАРАМЕТР (a, b, c)
# =====================================================================
def plot_volume_vs_param(param_name='a', param_range=None,
                         h=0.01, N=6000, s=0.5,
                         x0=(0.1, 0.1, 0.1), transient_frac=0.3,
                         fname=None):
    if param_range is None:
        param_range = np.linspace(0.5, 2.0, 21)

    fig, ax = plt.subplots(figsize=(10, 6))
    colors = plt.cm.tab10(np.linspace(0, 1, len(METHOD_ORDER)))

    results = {}
    for name, color in zip(METHOD_ORDER, colors):
        vols = []
        for val in tqdm(param_range, desc=f'V vs {param_name} [{name}]'):
            params = {'a': 1.0, 'b': 1.0, 'c': 1.0}
            params[param_name] = val
            tr = integrate(sprott_L, x0, h, N, method=name,
                           params=params, s=s)
            vols.append(phase_volume(tr, transient_frac))
        results[name] = np.array(vols)
        ax.plot(param_range, vols, 'o-', color=color, label=name, ms=4)

    ax.set_xlabel(f'Параметр {param_name}')
    ax.set_ylabel('Фазовый объём V')
    ax.set_yscale('log')
    ax.set_title(f'Зависимость фазового объёма от параметра {param_name} '
                 f'(h={h}, N={N})')
    ax.legend()
    plt.tight_layout()
    if fname:
        plt.savefig(fname, dpi=140)
    plt.close(fig)
    return param_range, results


# =====================================================================
# ПУНКТ 2. ФАЗОВЫЙ ОБЪЁМ vs ШАГ h
# =====================================================================
def plot_volume_vs_h(h_range=None, N=6000, s=0.5,
                     x0=(0.1, 0.1, 0.1), transient_frac=0.3,
                     fname='V_vs_h.png'):
    if h_range is None:
        h_range = np.logspace(-4, -1, 15)

    fig, ax = plt.subplots(figsize=(10, 6))
    colors = plt.cm.tab10(np.linspace(0, 1, len(METHOD_ORDER)))
    results = {}

    for name, color in zip(METHOD_ORDER, colors):
        vols = []
        for h in tqdm(h_range, desc=f'V(h) [{name}]'):
            tr = integrate(sprott_L, x0, h, N, method=name,
                           params={'a': 1, 'b': 1, 'c': 1}, s=s)
            vols.append(phase_volume(tr, transient_frac))
        results[name] = np.array(vols)
        ax.loglog(h_range, vols, 'o-', color=color, label=name, ms=4)

    ax.set_xlabel('Шаг h')
    ax.set_ylabel('Фазовый объём V')
    ax.set_title(f'Зависимость фазового объёма от шага h (N={N})')
    ax.legend()
    plt.tight_layout()
    plt.savefig(fname, dpi=140)
    plt.close(fig)
    return h_range, results


def plot_volume_vs_h_vs_rk4(h_range=None, N=6000, s=0.5,
                            x0=(0.1, 0.1, 0.1), transient_frac=0.3,
                            fname='V_vs_h_vs_RK4.png'):
    if h_range is None:
        h_range = np.logspace(-4, -1, 15)

    rk4_vals = []
    for h in tqdm(h_range, desc='RK4 baseline'):
        tr = integrate(sprott_L, x0, h, N, method='RK4',
                       params={'a': 1, 'b': 1, 'c': 1}, s=s)
        rk4_vals.append(phase_volume(tr, transient_frac))
    rk4_vals = np.array(rk4_vals)

    others = [m for m in METHOD_ORDER if m != 'RK4']
    fig, axes = plt.subplots(len(others), 1,
                             figsize=(10, 3.3*len(others)))
    for ax, name in zip(axes, others):
        vols = []
        for h in tqdm(h_range, desc=f'{name} vs h'):
            tr = integrate(sprott_L, x0, h, N, method=name,
                           params={'a': 1, 'b': 1, 'c': 1}, s=s)
            vols.append(phase_volume(tr, transient_frac))
        ax.loglog(h_range, rk4_vals, 'k-', lw=2, label='RK4 (эталон)')
        ax.loglog(h_range, vols, 'ro-', label=name, ms=4)
        ax.set_xlabel('h')
        ax.set_ylabel('V')
        ax.set_title(f'{name} vs RK4: V(h)')
        ax.legend()
    plt.tight_layout()
    plt.savefig(fname, dpi=140)
    plt.close(fig)


# =====================================================================
# ПУНКТ 3. ФАЗОВЫЙ ОБЪЁМ vs КОЭФФИЦИЕНТ СИММЕТРИИ s (метод CD)
# =====================================================================
def plot_volume_vs_s(s_range, h=0.01, N=6000, a=1.0, b=1.0, c=1.0,
                     x0=(0.1, 0.1, 0.1), transient_frac=0.3,
                     fname='V_vs_s_CD.png', title_extra=''):
    vols = []
    for s in tqdm(s_range, desc='V(s) [CD]'):
        tr = integrate(sprott_L, x0, h, N, method='CD',
                       params={'a': a, 'b': b, 'c': c}, s=float(s))
        vols.append(phase_volume(tr, transient_frac))
    vols = np.array(vols)

    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(s_range, vols, 'o-', color='crimson', ms=4)
    ax.axvline(0.5, color='k', ls='--', lw=1, label='s = 0.5 (методичка)')
    ax.set_xlabel('Коэффициент симметрии s')
    ax.set_ylabel('Фазовый объём V')
    ax.set_yscale('log')
    ax.set_title(f'Зависимость V от s для метода CD {title_extra}')
    ax.legend()
    plt.tight_layout()
    plt.savefig(fname, dpi=140)
    plt.close(fig)
    return vols


# =====================================================================
# ПУНКТ 4. ДИАГРАММА ПРОДОЛЖЕНИЯ (continuation)
# =====================================================================
def continuation_diagram(param_name='a', param_range=None,
                         h_list=(0.001, 0.005, 0.01, 0.02),
                         N=4000, N_transient=1000, skip=2,
                         params_base=None, method='RK4',
                         x0=(0.1, 0.1, 0.1), fname='continuation.png',
                         ycomponent=1):
    if param_range is None:
        param_range = np.linspace(0.5, 2.0, 201)
    if params_base is None:
        params_base = {'a': 1.0, 'b': 1.0, 'c': 1.0}

    fig, ax = plt.subplots(figsize=(11, 6))
    colors = plt.cm.viridis(np.linspace(0, 0.9, len(h_list)))

    for h, color in zip(h_list, colors):
        A_vals, Y_vals = [], []
        for val in tqdm(param_range, desc=f'continuation h={h:.3g}'):
            p = dict(params_base)
            p[param_name] = val
            tr = integrate(sprott_L, x0, h, N_transient + N,
                           method=method, params=p)
            seg = tr[N_transient:]
            seg = seg[np.all(np.isfinite(seg), axis=1)]
            seg = seg[::skip]
            A_vals.extend([val]*len(seg))
            Y_vals.extend(seg[:, ycomponent])

        ax.scatter(A_vals, Y_vals, s=0.3, color=color,
                   label=f'h = {h:g}', alpha=0.6, rasterized=True)

    ax.set_xlabel(f'Параметр {param_name}')
    ax.set_ylabel('y (сечение)')
    ax.set_title(f'Диаграмма продолжения по {param_name} '
                 f'(метод {method})')
    ax.legend(markerscale=15, loc='upper right')
    plt.tight_layout()
    if fname:
        plt.savefig(fname, dpi=140)
    plt.close(fig)


# =====================================================================
# ПУНКТ 5. БАССЕЙНЫ ПРИТЯЖЕНИЯ
# =====================================================================
def basin_of_attraction(method='RK4', a=1.0, b=1.0, c=1.0,
                        h=0.01, N=800, n_transient=300, s=0.5,
                        x_range=(-2.0, 2.0, 41),
                        y_range=(-2.0, 2.0, 41),
                        z0=0.1,
                        div_threshold=1e6):
    """
    Возвращает:
        Z =  1.0 — хаос (LLE > 0)
        Z =  0.0 — регулярность (LLE <= 0)
        Z = -1.0 — расходимость/неустойчивость
    """
    xs = np.linspace(*x_range)
    ys = np.linspace(*y_range)
    X, Y = np.meshgrid(xs, ys)
    Z = np.full_like(X, -1.0)

    p = {'a': a, 'b': b, 'c': c}
    step = STEP_FUNCS[method]

    for i, x0 in enumerate(tqdm(xs, desc=f'basin {method}')):
        for j, y0 in enumerate(ys):
            x = np.array([x0, y0, z0], float)

            ok = True
            for _ in range(n_transient):
                x = step(sprott_L, x, h, p, s=s)
                if (not np.all(np.isfinite(x))) or \
                   (np.max(np.abs(x)) > div_threshold):
                    ok = False
                    break
            if not ok:
                Z[j, i] = -1.0
                continue

            lle = largest_lyapunov_simple(
                method=method, a=a, b=b, c=c,
                h=h, N=N, x0=x,
                n_transient=0, s=s, renorm=5,
            )
            if not np.isfinite(lle):
                Z[j, i] = -1.0
            elif lle > 0:
                Z[j, i] = 1.0
            else:
                Z[j, i] = 0.0

    return X, Y, Z


def plot_basin(method='RK4', fname=None, **kw):
    X, Y, Z = basin_of_attraction(method=method, **kw)

    cmap = ListedColormap(['#3a3a3a', '#3b7dd8', '#d63b3b'])
    norm = BoundaryNorm([-1.5, -0.5, 0.5, 1.5], cmap.N)

    fig, ax = plt.subplots(figsize=(8, 7))
    im = ax.pcolormesh(X, Y, Z, cmap=cmap, norm=norm, shading='auto')

    cbar = plt.colorbar(im, ax=ax, ticks=[-1, 0, 1])
    cbar.ax.set_yticklabels(['расходимость', 'регулярность', 'хаос'])

    n_total = Z.size
    n_chaos = int((Z == 1.0).sum())
    n_reg = int((Z == 0.0).sum())
    n_div = int((Z == -1.0).sum())
    print(f'  {method:15s}: chaos={n_chaos/n_total:5.1%}, '
          f'reg={n_reg/n_total:5.1%}, div={n_div/n_total:5.1%}')

    ax.set_xlabel('x₀')
    ax.set_ylabel('y₀')
    ax.set_title(f'Бассейн режимов (a=b=c=1), метод {method}')
    plt.tight_layout()
    if fname:
        plt.savefig(fname, dpi=140)
    plt.close(fig)


# =====================================================================
# MAIN
# =====================================================================
if __name__ == '__main__':
    os.makedirs('lab4_out', exist_ok=True)
    os.chdir('lab4_out')

    # ---------- ПУНКТ 1 ----------
    print('\n=== Пункт 1: фазовый объём vs параметр ===')
    pr = np.linspace(0.5, 2.0, 21)
    plot_volume_vs_param('a', pr, fname='V_vs_a.png')
    plot_volume_vs_param('b', pr, fname='V_vs_b.png')
    plot_volume_vs_param('c', pr, fname='V_vs_c.png')

    # ---------- ПУНКТ 2 ----------
    print('\n=== Пункт 2: фазовый объём vs шаг ===')
    plot_volume_vs_h(fname='V_vs_h.png')
    plot_volume_vs_h_vs_rk4(fname='V_vs_h_vs_RK4.png')

    # ---------- ПУНКТ 3 ----------
    print('\n=== Пункт 3: фазовый объём vs коэффициент s (CD) ===')
    plot_volume_vs_s(np.linspace(0.05, 0.95, 19),
                     fname='V_vs_s_0_1.png',
                     title_extra='(s ∈ (0,1))')
    plot_volume_vs_s(np.linspace(-2.0, 2.0, 41),
                     fname='V_vs_s_m2_2.png',
                     title_extra='(s ∈ [-2, 2])')

    # ---------- ПУНКТ 4 ----------
    print('\n=== Пункт 4: диаграммы продолжения ===')
    continuation_diagram('a', np.linspace(0.5, 2.0, 101),
                         h_list=(0.001, 0.005, 0.01, 0.02),
                         method='RK4', N=4000, N_transient=1000,
                         skip=2, fname='continuation_a_RK4.png')
    continuation_diagram('a', np.linspace(0.5, 2.0, 101),
                         h_list=(0.001, 0.005, 0.01, 0.02),
                         method='CD', N=4000, N_transient=1000,
                         skip=2, fname='continuation_a_CD.png')

    # ---------- ПУНКТ 5 ----------
    print('\n=== Пункт 5: бассейны притяжения ===')
    for m in METHOD_ORDER:
        plot_basin(m,
                   x_range=(-2.0, 2.0, 41),
                   y_range=(-2.0, 2.0, 41),
                   h=0.01, N=800, n_transient=300,
                   fname=f'basin_{m}.png')

    print('\nГотово. Все файлы в lab4_out/')