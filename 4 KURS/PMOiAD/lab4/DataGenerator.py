import numpy as np


def norm_dataset(mu, sigma, N):
    """
    Линейно разделимые данные: два облака нормально распределённых точек.
    mu    = [mu0, mu1]        — центры классов (кортежи по признакам)
    sigma = [sigma0, sigma1]  — СКО классов (кортежи по признакам)
    """
    mu0, mu1 = mu[0], mu[1]
    sigma0, sigma1 = sigma[0], sigma[1]
    col = len(mu0)

    class0 = np.random.normal(mu0[0], sigma0[0], [N, 1])
    class1 = np.random.normal(mu1[0], sigma1[0], [N, 1])

    for i in range(1, col):
        v0 = np.random.normal(mu0[i], sigma0[i], [N, 1])
        class0 = np.hstack((class0, v0))
        v1 = np.random.normal(mu1[i], sigma1[i], [N, 1])
        class1 = np.hstack((class1, v1))

    Y1 = np.ones((N, 1), dtype=bool)
    Y0 = np.zeros((N, 1), dtype=bool)

    X = np.vstack((class0, class1))
    Y = np.vstack((Y0, Y1)).ravel()

    # seed=7 — привязка к варианту 7 (воспроизводимость перемешивания)
    rng = np.random.default_rng(7)
    arr = np.arange(2 * N)
    rng.shuffle(arr)
    X, Y = X[arr], Y[arr]

    return X, Y, class0, class1


def nonlinear_dataset_7(N=1000, seed=None):
    """
    Нелинейно разделимые данные: два Г-образных угла.
    Класс 0 — верхний левый угол, класс 1 — нижний правый угол.
    """
    rng = np.random.default_rng(seed)

    L = 3.0   # длина длинной полосы
    S = 1.0   # длина короткой полосы
    t = 0.15  # половина толщины полосы (СКО шума по нормали)

    # ---------- класс 0: верхний левый угол ----------
    x0, y0 = 0.0, 3.0
    n_h = int(round(N * L / (L + S)))
    n_v = N - n_h

    xh0 = rng.uniform(x0, x0 + L, n_h)
    yh0 = y0 + rng.normal(0.0, t, n_h)

    xv0 = x0 + rng.normal(0.0, t, n_v)
    yv0 = rng.uniform(y0 - S, y0, n_v)

    class0 = np.column_stack([
        np.concatenate([xh0, xv0]),
        np.concatenate([yh0, yv0]),
    ])

    # ---------- класс 1: нижний правый угол ----------
    x1, y1 = 3.5, 2.0
    n_h = int(round(N * L / (L + S)))
    n_v = N - n_h

    xv1 = x1 + rng.normal(0.0, t, n_v)
    yv1 = rng.uniform(y1, y1 + S, n_v)

    xh1 = rng.uniform(x1 - L, x1, n_h)
    yh1 = y1 + rng.normal(0.0, t, n_h)

    class1 = np.column_stack([
        np.concatenate([xh1, xv1]),
        np.concatenate([yh1, yv1]),
    ])

    # ---------- объединяем, метки, перемешиваем ----------
    X = np.vstack([class0, class1])
    Y = np.concatenate([
        np.zeros(len(class0), dtype=int),
        np.ones(len(class1),  dtype=int),
    ])

    idx = rng.permutation(len(X))
    X = X[idx]
    Y = Y[idx]

    return X, Y, class0, class1