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
# ПУНКТ 4 (переработан). ДИАГРАММА ПРОДОЛЖЕНИЯ по одной переменной
# =====================================================================
from scipy.signal import find_peaks

def analyze_trajectory(traj, transient_frac=0.3, component=1):
    """
    По траектории (N,3) возвращает:
        A      — амплитуда (y_max - y_min)/2 по выбранной компоненте,
        T_mean — средний межпиковый интервал (в шагах),
        y_ss   — среднее значение после транзиента.
    Пики ищем по |y - <y>|, чтобы захватить и положительные, и отрицательные.
    """
    n0 = int(len(traj) * transient_frac)
    tr = traj[n0:]
    tr = tr[np.all(np.isfinite(tr), axis=1)]
    if len(tr) < 20:
        return np.nan, np.nan, np.nan

    y = tr[:, component]
    y_mean = y.mean()
    A = 0.5 * (y.max() - y.min())

    # пики ищем по отклонению от среднего, чтобы захватить и впадины
    signal = np.abs(y - y_mean)
    # минимальная высота и расстояние: чтобы не поймать шум
    prom = 0.05 * A if A > 0 else 1e-3
    peaks, _ = find_peaks(signal, prominence=prom, distance=3)
    if len(peaks) >= 2:
        T_mean = float(np.mean(np.diff(peaks)))
    else:
        T_mean = np.nan
    return A, T_mean, y_mean


def continuation_diagram(param_name='a', param_range=None,
                         h=0.01, T_total=1000.0, transient_frac=0.3,
                         x0_list=None, params_base=None,
                         method='RK4', component=1,
                         fname='continuation.png'):
    """
    Диаграмма продолжения по одной переменной состояния.
    Для каждого a (и каждой НУ из x0_list) считаем:
        - среднее значение y после транзиента,
        - амплитуду A = (ymax-ymin)/2,
        - средний межпиковый интервал T_mean.
    Сверху — y(a) точками, снизу — A(a) и T_mean(a) линиями со сглаживанием.
    Время интегрирования: T_total секунд модельного времени => N = int(T_total/h).
    """
    if param_range is None:
        param_range = np.linspace(0.5, 2.0, 41)
    if params_base is None:
        params_base = {'a': 1.0, 'b': 1.0, 'c': 1.0}
    if x0_list is None:
        # разные НУ — это и есть "разные начальные условия" для продолжения
        x0_list = [(0.1, 0.1, 0.1),
                   (0.5, -0.3, 0.2),
                   (-0.4, 0.6, -0.1),
                   (1.0, 1.0, 0.5),
                   (-1.0, -0.5, -0.5)]

    N = int(T_total / h)
    A_avg, T_avg, y_avg = [], [], []

    for val in tqdm(param_range, desc=f'continuation {method}'):
        p = dict(params_base)
        p[param_name] = val

        As, Ts, ys = [], [], []
        for x0 in x0_list:
            tr = integrate(sprott_L, x0, h, N,
                           method=method, params=p)
            A, T_mean, y_mean = analyze_trajectory(
                tr, transient_frac=transient_frac, component=component)
            if np.isfinite(A) and A > 1e-8:
                As.append(A)
                ys.append(y_mean)
            if np.isfinite(T_mean) and T_mean > 0:
                Ts.append(T_mean)

        A_avg.append(np.mean(As) if As else np.nan)
        T_avg.append(np.mean(Ts) if Ts else np.nan)
        y_avg.append(np.mean(ys) if ys else np.nan)

    A_avg = np.array(A_avg); T_avg = np.array(T_avg); y_avg = np.array(y_avg)

    # ---------------- Рисуем ----------------
    fig, axes = plt.subplots(3, 1, figsize=(11, 10), sharex=True)

    ax = axes[0]
    ax.plot(param_range, y_avg, 'o-', color='tab:blue', ms=4,
            label=f'⟨y⟩ (усреднено по {len(x0_list)} НУ)')
    ax.set_ylabel('⟨y⟩')
    ax.set_title(f'Continuation по {param_name}, метод {method} '
                 f'(h={h}, T={T_total})')
    ax.legend(); ax.grid(True)

    ax = axes[1]
    ax.semilogy(param_range, A_avg, 'o-', color='tab:red', ms=4,
                label='Амплитуда A = (ymax−ymin)/2')
    ax.set_ylabel('Амплитуда A')
    ax.legend(); ax.grid(True)

    ax = axes[2]
    # переводим шаги в модельное время
    T_time = T_avg * h
    ax.plot(param_range, T_time, 'o-', color='tab:green', ms=4,
            label='Средний межпиковый интервал')
    ax.set_xlabel(f'Параметр {param_name}')
    ax.set_ylabel('T_межпик (модельное время)')
    ax.legend(); ax.grid(True)

    plt.tight_layout()
    if fname:
        plt.savefig(fname, dpi=140)
    plt.close(fig)
    return param_range, y_avg, A_avg, T_time


# =====================================================================
# ПУНКТ 5 (переработан). БАССЕЙНЫ ПРИТЯЖЕНИЯ КАК КЛАСТЕРЫ
# =====================================================================
from sklearn.cluster import KMeans

def basin_clusters(method='RK4', a=1.0, b=1.0, c=1.0,
                   h=0.01, T_total=400.0, transient_frac=0.3,
                   s=0.5,
                   x_range=(-2.0, 2.0, 61),
                   y_range=(-2.0, 2.0, 61),
                   z0=0.1,
                   n_clusters=4,
                   div_threshold=1e6):
    """
    Бассейны притяжения как кластеры.
    Признак аттрактора для НУ (x0, y0):
        feats = (⟨x⟩, ⟨y⟩, ⟨z⟩, A_x, A_y, A_z, T_mean)
    KMeans по признакам; точки с расходимостью = отдельный класс -1.

    Возвращает X, Y, Z (Z: метки кластеров 0..n_clusters-1, -1 — расходимость),
    а также метки центроидов (для легенды).
    """
    xs = np.linspace(*x_range)
    ys = np.linspace(*y_range)
    X, Y = np.meshgrid(xs, ys)
    N = int(T_total / h)

    feats = np.full((len(ys), len(xs), 7), np.nan)
    p = {'a': a, 'b': b, 'c': c}

    for i, x0 in enumerate(tqdm(xs, desc=f'clusters {method}')):
        for j, y0 in enumerate(ys):
            tr = integrate(sprott_L, (x0, y0, z0), h, N,
                           method=method, params=p, s=s)
            tr_ss = tr[int(N * transient_frac):]
            tr_ss = tr_ss[np.all(np.isfinite(tr_ss), axis=1)]
            if len(tr_ss) < 50:
                continue
            if np.max(np.abs(tr_ss)) > div_threshold:
                continue
            mean_v = tr_ss.mean(axis=0)
            amp_v = 0.5 * (tr_ss.max(axis=0) - tr_ss.min(axis=0))
            # средний межпиковый интервал по |y|
            signal = np.abs(tr_ss[:, 1] - mean_v[1])
            prom = 0.05 * amp_v[1] if amp_v[1] > 0 else 1e-3
            peaks, _ = find_peaks(signal, prominence=prom, distance=3)
            T_mean = float(np.mean(np.diff(peaks))) if len(peaks) >= 2 else 0.0
            feats[j, i] = np.concatenate([mean_v, amp_v, [T_mean]])

    mask = np.all(np.isfinite(feats), axis=2)
    Z = np.full(feats.shape[:2], -1)
    centers = None

    if mask.sum() >= n_clusters:
        # нормируем признаки, чтобы KMeans не доминировал по масштабу
        F = feats[mask]
        F_n = (F - F.mean(axis=0)) / (F.std(axis=0) + 1e-12)
        km = KMeans(n_clusters=n_clusters, n_init='auto',
                    random_state=0).fit(F_n)
        Z[mask] = km.labels_
        centers = km.cluster_centers_

    return X, Y, Z, centers


def plot_basin_clusters(method='RK4', fname=None,
                        a=1.0, b=1.0, c=1.0,
                        h=0.01, T_total=400.0, transient_frac=0.3,
                        x_range=(-2.0, 2.0, 61),
                        y_range=(-2.0, 2.0, 61),
                        z0=0.1, n_clusters=4):
    X, Y, Z, centers = basin_clusters(
        method=method, a=a, b=b, c=c,
        h=h, T_total=T_total, transient_frac=transient_frac,
        x_range=x_range, y_range=y_range, z0=z0,
        n_clusters=n_clusters)

    # палитра: n_clusters цветов + 1 тёмно-серый для расходимости
    base_cols = plt.cm.tab10(np.linspace(0, 1, n_clusters))
    cmap = ListedColormap(np.vstack([base_cols, [0.22, 0.22, 0.22, 1.0]]))
    # метки: 0..n_clusters-1 -> цвета; -1 -> последний индекс
    bounds = np.arange(-1.5, n_clusters + 0.5, 1.0)
    norm = BoundaryNorm(bounds, cmap.N)
    Z_plot = Z.copy()

    fig, ax = plt.subplots(figsize=(8, 7))
    im = ax.pcolormesh(X, Y, Z_plot, cmap=cmap, norm=norm,
                       shading='auto')

    cbar = plt.colorbar(im, ax=ax,
                        ticks=np.arange(-1, n_clusters))
    cbar.ax.set_yticklabels(
        [f'кластер {k}' for k in range(n_clusters)] + ['расходимость'])

    n_total = Z.size
    for k in range(n_clusters):
        frac = (Z == k).sum() / n_total
        print(f'  {method:15s}: кластер {k}: {frac:5.1%}')
    frac_div = (Z == -1).sum() / n_total
    print(f'  {method:15s}: расходимость: {frac_div:5.1%}')

    ax.set_xlabel('x₀')
    ax.set_ylabel('y₀')
    ax.set_title(f'Бассейны притяжения (a={a}, b={b}, c={c}), '
                 f'метод {method}')
    plt.tight_layout()
    if fname:
        plt.savefig(fname, dpi=140)
    plt.close(fig)


from scipy.signal import find_peaks
import numpy as np
import matplotlib.pyplot as plt
from tqdm import tqdm


def analyze_trajectory(traj, transient_frac=0.3, component=1):
    """
    По траектории (N,3) возвращает:
        A      — амплитуда (y_max - y_min)/2 по выбранной компоненте,
        T_mean — средний межпиковый интервал (в шагах),
        y_ss   — среднее значение после транзиента.
    """
    n0 = int(len(traj) * transient_frac)
    tr = traj[n0:]
    tr = tr[np.all(np.isfinite(tr), axis=1)]
    if len(tr) < 20:
        return np.nan, np.nan, np.nan

    y = tr[:, component]
    y_mean = y.mean()
    A = 0.5 * (y.max() - y.min())

    signal = np.abs(y - y_mean)
    prom = 0.05 * A if A > 0 else 1e-3
    peaks, _ = find_peaks(signal, prominence=prom, distance=3)
    T_mean = float(np.mean(np.diff(peaks))) if len(peaks) >= 2 else np.nan

    return A, T_mean, y_mean


def continuation_diagram(param_name='a', param_range=None,
                         h=0.01, T_total=1000.0, transient_frac=0.3,
                         x0_list=None, params_base=None,
                         method='RK4', component=1,
                         fname='continuation.png',
                         cmap='plasma'):
    """
    2D-диаграмма продолжения:
      • по оси X — параметр (по умолчанию a);
      • по оси Y — мгновенные значения y на аттракторе;
      • цвет точки — A_avg(a) (амплитуда, усреднённая по НУ).

    Дополнительно снизу — панель A_avg(a) и T_mean(a),
    чтобы видеть динамику по параметру.
    """
    if param_range is None:
        param_range = np.linspace(0.5, 2.0, 41)
    if params_base is None:
        params_base = {'a': 1.0, 'b': 1.0, 'c': 1.0}
    if x0_list is None:
        x0_list = [(0.1, 0.1, 0.1),
                   (0.5, -0.3, 0.2),
                   (-0.4, 0.6, -0.1),
                   (1.0, 1.0, 0.5),
                   (-1.0, -0.5, -0.5)]

    N = int(T_total / h)

    # ---- накапливаем данные по всем a и всем НУ ----
    A_avg = np.full(len(param_range), np.nan)
    T_avg = np.full(len(param_range), np.nan)
    y_avg = np.full(len(param_range), np.nan)

    # для каждого значения a собираем (a, y) точки + локальную амплитуду
    X_pts, Y_pts = [], []
    # локальная амплитуда для каждой точки: A_avg(a), продублированная
    # для каждого y из этой "коробки" — так цвет показывает амплитуду
    C_pts = []

    for k, val in enumerate(tqdm(param_range,
                                 desc=f'continuation {method}')):
        p = dict(params_base); p[param_name] = val

        As, Ts, ys = [], [], []
        y_this_a = []   # все y по всем НУ для данного a

        for x0 in x0_list:
            tr = integrate(sprott_L, x0, h, N,
                           method=method, params=p)
            A, T_mean, y_mean = analyze_trajectory(
                tr, transient_frac=transient_frac, component=component)
            if np.isfinite(A) and A > 1e-8:
                As.append(A); ys.append(y_mean)
            if np.isfinite(T_mean) and T_mean > 0:
                Ts.append(T_mean)

            # сохраняем хвост траектории после транзиента
            n0 = int(N * transient_frac)
            seg = tr[n0:]
            seg = seg[np.all(np.isfinite(seg), axis=1)]
            if len(seg) > 0:
                y_this_a.extend(seg[:, component])

        # усреднённые по НУ характеристики для данного a
        A_avg[k] = np.mean(As) if As else np.nan
        T_avg[k] = np.mean(Ts) if Ts else np.nan
        y_avg[k] = np.mean(ys) if ys else np.nan

        # накапливаем точки (a, y) с цветом = A_avg[k]
        if len(y_this_a) > 0:
            X_pts.extend([val] * len(y_this_a))
            Y_pts.extend(y_this_a)
            C_pts.extend([A_avg[k]] * len(y_this_a))

    X_pts = np.asarray(X_pts)
    Y_pts = np.asarray(Y_pts)
    C_pts = np.asarray(C_pts)

    # ---- Рисуем ----
    fig, axes = plt.subplots(2, 1, figsize=(11, 9),
                             gridspec_kw={'height_ratios': [3, 1]},
                             sharex=True)

    # ---------- Верхняя панель: 2D-диаграмма (a, y) с цветом A_avg ----------
    ax = axes[0]

    # нормируем цвет по диапазону амплитуды
    finite_A = A_avg[np.isfinite(A_avg)]
    if len(finite_A) > 0:
        cmin, cmax = finite_A.min(), finite_A.max()
    else:
        cmin, cmax = 0, 1
    if cmax <= cmin:
        cmax = cmin + 1

    sc = ax.scatter(X_pts, Y_pts,
                    c=C_pts, cmap=cmap,
                    vmin=cmin, vmax=cmax,
                    s=0.4, alpha=0.6, rasterized=True,
                    edgecolors='none')

    cbar = plt.colorbar(sc, ax=ax, pad=0.01)
    cbar.set_label('A_avg (амплитуда, усреднённая по НУ)', fontsize=10)

    ax.set_ylabel(f'y (значения на аттракторе)', fontsize=11)
    ax.set_title(
        f'2D-диаграмма продолжения по {param_name}, метод {method} '
        f'(h={h}, T={T_total}) — цвет = A_avg',
        fontsize=11)
    ax.grid(True, alpha=0.3)

    # ---------- Нижняя панель: A_avg(a) и T_mean(a) ----------
    ax2 = axes[1]
    ax2.plot(param_range, A_avg, 'o-', color='tab:red', ms=4,
             label='A_avg(a)')
    ax2.set_ylabel('A_avg', color='tab:red', fontsize=10)
    ax2.tick_params(axis='y', labelcolor='tab:red')
    ax2.set_xlabel(f'Параметр {param_name}', fontsize=11)
    ax2.grid(True, alpha=0.3)

    ax3 = ax2.twinx()
    # T_avg переводим в модельное время
    T_time = T_avg * h
    ax3.plot(param_range, T_time, 's-', color='tab:green', ms=4,
             label='T_mean(a)')
    ax3.set_ylabel('T_mean (модельное время)',
                   color='tab:green', fontsize=10)
    ax3.tick_params(axis='y', labelcolor='tab:green')

    # объединённая легенда
    lines1, labels1 = ax2.get_legend_handles_labels()
    lines2, labels2 = ax3.get_legend_handles_labels()
    ax2.legend(lines1 + lines2, labels1 + labels2,
               loc='upper right', fontsize=9)

    plt.tight_layout()
    if fname:
        plt.savefig(fname, dpi=150, bbox_inches='tight')
    plt.close(fig)

    return param_range, y_avg, A_avg, T_avg * h

# =====================================================================
# MAIN — обновлённый блок для пунктов 4 и 5
# =====================================================================
if __name__ == '__main__':
    os.makedirs('lab4_3_out', exist_ok=True)
    os.chdir('lab4_3_out')

    print('\n=== Пункт 4: 2D-диаграммы продолжения ===')
for m in ['RK4', 'CD', 'Midpoint', 'Euler', 'Euler-Cromer']:
    continuation_diagram(
        'a', np.linspace(0.5, 2.0, 41),
        h=0.01, T_total=1000.0, transient_frac=0.3,
        method=m, component=1,
        fname=f'continuation_a_{m}.png')

    print('\nГотово. Все файлы в lab4_3_out/')