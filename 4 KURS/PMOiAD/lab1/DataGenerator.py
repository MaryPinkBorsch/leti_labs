import numpy as np

#объявляем функцию и определяем входные переменные

def norm_dataset(mu,sigma,N): # обозначение имя функции и входных аргументов
# тело функции, обязательно с отступом! 
    mu0 = mu[0]
    mu1 = mu[1] 
    sigma0 = sigma[0] 
    sigma1 = sigma[1]
    col = len(mu0) # количество столбцов-признаков
    class0 = np.random.normal(mu0[0],sigma0[0],[N,1]) # инициализируем первый столбец (в Python нумерация от 0)
    class1 = np.random.normal(mu1[0],sigma1[0],[N,1])

    for i in range(1,col): # подумайте, почему нумерация с 1, а не с 0 
        v0 = np.random.normal(mu0[i],sigma0[i],[N,1])
        class0 = np.hstack((class0,v0))

        v1 = np.random.normal(mu1[i],sigma1[i],[N,1]) 
        class1 = np.hstack((class1,v1))

    

    Y1 =np.ones((N, 1), dtype=bool) 
    Y0 = np.zeros((N, 1), dtype=bool)

    X = np.vstack((class0,class1))
    Y = np.vstack((Y0,Y1)).ravel() #ravel позволяет сделать массив плоским – одномерным, размера (N,), это необходимо для дальнейшего использования в классификаторах


    # перемешиваем данные
    rng = np.random.default_rng()
    arr = np.arange(2*N) #индексы для перемешивания 
    rng.shuffle(arr)

    X = X[arr] 
    Y = Y[arr]
    
    return X, Y, class0, class1 # возвращаемые аргументы



import numpy as np

def nonlinear_dataset_7(N=1000, seed=None):
    """
    Генерирует двумерный датасет из двух классов,
    каждый класс — Г-образный угол (две перпендикулярные полосы).
    
    Класс 0 (серая фигура): верхний левый угол
        - горизонтальная полоса сверху,
        - вертикальная полоса слева,
        - вершина угла в точке (x0, y0).
    
    Класс 1 (жёлтая фигура): нижний правый угол
        - вертикальная полоса справа,
        - горизонтальная полоса снизу,
        - вершина угла в точке (x1, y1).
    
    Параметры
    ---------
    N : int
        Число точек в каждом классе.
    seed : int или None
        Зерно генератора для воспроизводимости.
    
    Возвращает
    ----------
    X : ndarray формы (2N, 2)
        Признаки (перемешанные).
    Y : ndarray формы (2N,)
        Метки классов (0 и 1), перемешанные.
    class0 : ndarray формы (N, 2)
        Точки класса 0 (не перемешанные).
    class1 : ndarray формы (N, 2)
        Точки класса 1 (не перемешанные).
    """
    rng = np.random.default_rng(seed)

    # ---------- общие геометрические параметры ----------
    L  = 3.0    # длина длинной полосы  
    S  = 1.0    # длина короткой полосы  
    t  = 0.15   # половина "толщины" полосы (СКО шума по нормали)

    # ---------- класс 0: верхний левый угол ----------
    # вершина угла
    x0, y0 = 0.0, 3.0

    # сколько точек на горизонтальную и вертикальную полосу
    n_h = int(round(N*L/(L+S)))
    n_v = N-n_h
    
    # горизонтальная полоса: идёт вправо от вершины, y ≈ y0
    xh0 = rng.uniform(x0, x0 + L, n_h)
    yh0 = y0 + rng.normal(0.0, t, n_h)

    # вертикальная полоса: идёт вниз от вершины, x ≈ x0
    xv0 = x0 + rng.normal(0.0, t, n_v)
    yv0 = rng.uniform(y0 - S, y0, n_v)

    class0 = np.column_stack([
        np.concatenate([xh0, xv0]),
        np.concatenate([yh0, yv0]),
    ])

    # ---------- класс 1: нижний правый угол ----------
    # вершина угла
    x1, y1 = 3.5, 2.0

    # сколько точек на горизонтальную и вертикальную полосу
    n_h = int(round(N*L/(L+S)))
    n_v = N-n_h


    # вертикальная полоса: идёт вверх от вершины, x ≈ x1
    xv1 = x1 + rng.normal(0.0, t, n_v)
    yv1 = rng.uniform(y1, y1 + S, n_v)

    # горизонтальная полоса: идёт влево от вершины, y ≈ y1
    xh1 = rng.uniform(x1 - L, x1, n_h)
    yh1 = y1 + rng.normal(0.0, t, n_h)

    class1 = np.column_stack([
        np.concatenate([xh1, xv1]),
        np.concatenate([yh1, yv1]),
    ])

    # ---------- объединяем, формируем метки ----------
    X = np.vstack([class0, class1])
    Y = np.concatenate([
        np.zeros(len(class0), dtype=int),
        np.ones(len(class1),  dtype=int),
    ])

    # ---------- перемешиваем ----------
    idx = rng.permutation(len(X))
    X = X[idx]
    Y = Y[idx]

    return X, Y, class0, class1