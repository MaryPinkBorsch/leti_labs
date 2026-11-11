"""
Лабораторная работа №4, пункт 4.
Диаграмма продолжения по начальному условию x0 для разных шагов h.
Строит графики вида "x0 vs значения пиков компоненты".
"""

import os
import numpy as np
import matplotlib
matplotlib.use('Agg')            # без интерактивных окон
import matplotlib.pyplot as plt
from matplotlib import rcParams
from scipy.signal import find_peaks
from tqdm import tqdm

rcParams['figure.figsize'] = (10, 6)
rcParams['font.size'] = 11
rcParams['axes.grid'] = True


# =====================================================================
# Система Sprott Case L
# =====================================================================
def sprott_L(state, a=1.0, b=1.0, c=1.0):
    x, y, z = state
    return np.array([a*y, -b*x + y*z, c - y*y])


# =====================================================================
# Одношаговые интеграторы
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
    k1 = f(x, **p)
    k2 = f(x + 0.5*h*k1, **p)
    return x + h*k2


def step_rk4(f, x, h, p, s=0.5):
    k1 = f(x, **p)
    k2 = f(x + 0.5*h*k1, **p)
    k3 = f(x + 0.5*h*k2, **p)
    k4 = f(x + h*k3, **p)
    return x + (h/6.0)*(k1 + 2*k2 + 2*k3 + k4)


def step_cd_comp(f, x, h, p, s=0.5):
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


# =====================================================================
# Интегратор траектории
# =====================================================================
def integrate(f, x0, h, N, method='RK4', params=None, s=0.5):
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


# =====================================================================
# Извлечение пиков выбранной компоненты
# =====================================================================
def peaks_of_component(traj, transient_frac=0.3, component=0):
    """
    Возвращает значения компоненты в моменты локальных экстремумов
    (по модулю отклонения от среднего) после отбрасывания транзиента.
    """
    n0 = int(len(traj) * transient_frac)
    tr = traj[n0:]
    tr = tr[np.all(np.isfinite(tr), axis=1)]
    if len(tr) < 20:
        return np.array([])
    sig = tr[:, component]
    abs_sig = np.abs(sig - sig.mean())
    max_val = abs_sig.max() if abs_sig.size else 0.0
    if max_val <= 0:
        return np.array([])
    prom = 0.05 * max_val
    peaks, _ = find_peaks(abs_sig, prominence=prom, distance=2)
    if len(peaks) == 0:
        return np.array([])
    return sig[peaks]


# =====================================================================
# Диаграмма продолжения по x0 для разных h
# =====================================================================
def continuation_by_x0(x0_range=None,
                       h_list=(0.001, 0.005, 0.01, 0.05),
                       T_total=1000.0, transient_frac=0.3,
                       method='Euler', component=0,
                       a=1.0, b=1.0, c=1.0,
                       y0=0.1, z0=0.1,
                       fname='continuation_by_x0.png'):
    if x0_range is None:
        x0_range = np.linspace(-1.5, 1.5, 151)

    # палитра через tab10 по индексам (без строковых имён вроде 'tab:gold')
    base_cols = plt.cm.tab10(np.linspace(0, 1, max(len(h_list), 1)))
    colors = [base_cols[i % len(base_cols)] for i in range(len(h_list))]

    fig, ax = plt.subplots(figsize=(9, 7))

    for h, color in zip(h_list, colors):
        X_pts, Y_pts = [], []
        for x0 in tqdm(x0_range, desc=f'cont h={h:g} [{method}]'):
            tr = integrate(
                sprott_L, (x0, y0, z0), h, int(T_total / h),
                method=method, params={'a': a, 'b': b, 'c': c})
            pk = peaks_of_component(tr, transient_frac=transient_frac,
                                    component=component)
            if len(pk) > 0:
                X_pts.extend([x0] * len(pk))
                Y_pts.extend(pk.tolist())

        if X_pts:
            ax.scatter(X_pts, Y_pts, s=4, color=color,
                       label=f'h={h:g}', alpha=0.75,
                       edgecolors='none', rasterized=True)

    ax.set_xlabel('x₀')
    ax.set_ylabel(f'Пики {"xyz"[component]}')
    ax.set_title(f'Continuation, {method}')
    ax.legend(loc='upper center', framealpha=0.95, markerscale=2)
    ax.grid(True, alpha=0.4)
    plt.tight_layout()
    if fname:
        plt.savefig(fname, dpi=150, bbox_inches='tight')
    plt.close(fig)


# =====================================================================
# MAIN
# =====================================================================
if __name__ == '__main__':
    os.makedirs('lab4_continua', exist_ok=True)
    os.chdir('lab4_continua')

    # for m in ['Euler', 'Euler-Cromer', 'Midpoint', 'CD', 'RK4']:
    for m in ['RK4']:
        continuation_by_x0(
            x0_range=np.linspace(-1.5, 1.5, 151),
            h_list=(0.001, 0.005, 0.01, 0.05),
            T_total=1000.0,
            transient_frac=0.3,
            method=m,
            component=0,
            fname=f'continuation_by_x0_{m}.png',
        )

    print('Готово. Файлы в lab4_continua2/')