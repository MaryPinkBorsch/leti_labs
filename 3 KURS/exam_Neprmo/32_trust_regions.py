import numpy as np
from typing import Callable, Tuple, List, Dict, Any

def trust_region_algorithm(
    f: Callable[[np.ndarray], float],
    grad: Callable[[np.ndarray], np.ndarray],
    hess: Callable[[np.ndarray], np.ndarray],
    x0: np.ndarray,
    epsilon: float = 1e-6,
    delta0: float = 1.0,
    eta: float = 0.1,
    max_iter: int = 100
) -> Dict[str, Any]:
    """
    Алгоритм доверительных областей
    
    Параметры:
    ----------
    f : целевая функция R^n -> R
    grad : градиент функции R^n -> R^n
    hess : гессиан функции R^n -> R^(n×n)
    x0 : начальная точка
    epsilon : точность для КОП (критерия останова)
    delta0 : начальный радиус доверительной области
    eta : порог для принятия шага (0 < eta < 1)
    max_iter : максимальное число итераций
    
    Возвращает:
    ----------
    result : словарь с результатами
        'x_star' : найденное решение
        'f_star' : значение функции в решении
        'history' : история итераций
        'converged' : флаг сходимости
        'message' : сообщение о результате
    """
    
    # Инициализация
    x_k = x0.copy()
    delta_k = delta0
    n = len(x0)
    
    # История итераций для возврата
    history = {
        'x': [x_k.copy()],
        'f': [f(x_k)],
        'delta': [delta_k],
        'rho': [],
        'p_norm': [],
        'accepted': [],
        'iteration': 0
    }
    
    # Основной цикл алгоритма
    for iteration in range(max_iter):
        # Шаг 1: Строим модель в точке x_k
        f_k = f(x_k)
        g_k = grad(x_k)
        B_k = hess(x_k)
        
        # Определяем квадратичную модель m_k(p) = f_k + g_k^T p + 0.5 * p^T B_k p
        def model_k(p: np.ndarray) -> float:
            return f_k + np.dot(g_k, p) + 0.5 * np.dot(p, np.dot(B_k, p))
        
        # Находим шаг p_k как решение подзадачи
        # p_k = argmin m_k(p) при условии ||p|| <= delta_k
        p_k = solve_trust_region_subproblem(g_k, B_k, delta_k)
        
        # Шаг 2: Вычисляем отношение улучшений
        # Фактическое улучшение
        x_candidate = x_k + p_k
        f_candidate = f(x_candidate)
        actual_reduction = f_k - f_candidate
        
        # Предсказанное улучшение по модели
        predicted_reduction = model_k(np.zeros(n)) - model_k(p_k)
        
        # Вычисляем rho_k
        if abs(predicted_reduction) > 1e-12:
            rho_k = actual_reduction / predicted_reduction
        else:
            rho_k = 0.0
        
        # Принимаем или отвергаем шаг
        if rho_k > eta:
            x_k = x_candidate  # принимаем шаг
            accepted = True
        else:
            accepted = False  # отвергаем шаг, остаемся в x_k
        
        # Обновляем радиус доверительной области
        delta_k = update_trust_region_radius(delta_k, rho_k, np.linalg.norm(p_k))
        
        # Сохраняем историю
        history['x'].append(x_k.copy())
        history['f'].append(f(x_k))
        history['delta'].append(delta_k)
        history['rho'].append(rho_k)
        history['p_norm'].append(np.linalg.norm(p_k))
        history['accepted'].append(accepted)
        history['iteration'] = iteration + 1
        
        # Шаг 3: Проверка критерия останова (КОП)
        # КОП: ||p_k|| < epsilon и ||g_k|| < epsilon
        g_k_new = grad(x_k)
        stop_condition = (np.linalg.norm(p_k) < epsilon and 
                         np.linalg.norm(g_k_new) < epsilon)
        
        if stop_condition:
            break
    
    # Формируем результат
    result = {
        'x_star': x_k,
        'f_star': f(x_k),
        'history': history,
        'converged': iteration < max_iter - 1,
        'iterations': iteration + 1,
        'message': f"Алгоритм завершился за {iteration + 1} итераций"
    }
    
    return result


def solve_trust_region_subproblem(
    g: np.ndarray,
    B: np.ndarray,
    delta: float
) -> np.ndarray:
    """
    Решение подзадачи доверительных областей
    min_p m(p) = g^T p + 0.5 p^T B p
    s.t. ||p|| <= delta
    
    Используется метод Коши для простоты (самый простой вариант)
    """
    n = len(g)
    
    # Метод Коши (Cauchy point) - самый простой вариант
    # Находим минимум вдоль направления антиградиента
    
    # Если B положительно определенная, можно использовать решение Ньютона
    try:
        # Пробуем решить без ограничений
        p_unconstrained = np.linalg.solve(B, -g)
        
        # Проверяем, попадает ли решение в доверительную область
        p_norm = np.linalg.norm(p_unconstrained)
        if p_norm <= delta:
            return p_unconstrained
        else:
            # Масштабируем решение
            return p_unconstrained * (delta / p_norm)
    except np.linalg.LinAlgError:
        # Если матрица вырождена, используем направление антиградиента
        pass
    
    # Метод Коши (по антиградиенту)
    g_norm = np.linalg.norm(g)
    if g_norm < 1e-12:
        return np.zeros(n)
    
    # Направление антиградиента
    d = -g / g_norm
    
    # Минимизируем квадратичную функцию вдоль d
    # phi(t) = g^T (t*d) + 0.5 (t*d)^T B (t*d)
    #        = t * (g^T d) + 0.5 t^2 * (d^T B d)
    
    gd = np.dot(g, d)
    dBd = np.dot(d, np.dot(B, d))
    
    if dBd > 0:
        # Парабола имеет минимум
        t_opt = -gd / dBd
        # Ограничиваем t доверительной областью
        t = min(t_opt, delta)
    else:
        # Линейная функция, минимум на границе
        t = delta if gd < 0 else -delta
    
    p = t * d
    return p


def update_trust_region_radius(
    delta: float,
    rho: float,
    p_norm: float,
    eta1: float = 0.25,
    eta2: float = 0.75,
    gamma1: float = 0.5,
    gamma2: float = 2.0
) -> float:
    """
    Обновление радиуса доверительной области
    
    Параметры:
    ----------
    delta : текущий радиус
    rho : отношение фактического улучшения к предсказанному
    p_norm : норма шага
    eta1, eta2 : пороги для изменения радиуса (0 < eta1 < eta2 < 1)
    gamma1, gamma2 : коэффициенты уменьшения/увеличения радиуса
    
    Возвращает:
    ----------
    new_delta : новый радиус
    """
    if rho < eta1:
        # Модель плохо предсказывает, уменьшаем радиус
        new_delta = gamma1 * delta
    elif rho > eta2 and abs(p_norm - delta) < 1e-6:
        # Модель хорошо предсказывает и шаг на границе, увеличиваем радиус
        new_delta = min(gamma2 * delta, 10.0)  # ограничиваем сверху
    else:
        # Оставляем радиус без изменений
        new_delta = delta
    
    return new_delta


# Вспомогательная функция для создания истории в формате псевдокода
def print_algorithm_history(result: Dict[str, Any], num_iterations: int = 5) -> None:
    """
    Выводит историю работы алгоритма в формате, похожем на псевдокод
    """
    history = result['history']
    
    print("\n" + "="*80)
    print("ИСТОРИЯ РАБОТЫ АЛГОРИТМА ДОВЕРИТЕЛЬНЫХ ОБЛАСТЕЙ")
    print("="*80)
    
    print(f"\nНачальная точка: x0 = {history['x'][0]}")
    print(f"Начальный радиус: Δ0 = {history['delta'][0]:.6f}")
    print(f"Порог η = 0.1")
    print(f"Точность ε = 1e-6")
    
    print("\n" + "-"*80)
    print("Итерации:")
    print("-"*80)
    
    for i in range(min(num_iterations, history['iteration'])):
        print(f"\nИтерация {i+1}:")
        print(f"  x_{i} = {history['x'][i]}")
        print(f"  f(x_{i}) = {history['f'][i]:.6f}")
        
        if i < len(history['rho']):
            print(f"  Шаг p_{i} (норма) = {history['p_norm'][i]:.6f}")
            print(f"  ρ_{i} = {history['rho'][i]:.6f}")
            print(f"  Принят шаг? {'ДА' if history['accepted'][i] else 'НЕТ'}")
            print(f"  Новый радиус Δ_{i+1} = {history['delta'][i+1]:.6f}")
    
    print("\n" + "-"*80)
    print(f"РЕЗУЛЬТАТ:")
    print("-"*80)
    print(f"Найденное решение: x* = {result['x_star']}")
    print(f"Значение функции: f(x*) = {result['f_star']:.8f}")
    print(f"Количество итераций: {result['iterations']}")
    print(f"Сообщение: {result['message']}")
    print("="*80)


# Пример использования
if __name__ == "__main__":
    # Тестовая функция: квадратичная форма
    def quadratic_f(x: np.ndarray) -> float:
        """f(x) = 0.5 * x^T A x + b^T x"""
        A = np.array([[4, 1], [1, 3]], dtype=float)
        b = np.array([1, 2], dtype=float)
        return 0.5 * np.dot(x, np.dot(A, x)) + np.dot(b, x)
    
    def quadratic_grad(x: np.ndarray) -> np.ndarray:
        A = np.array([[4, 1], [1, 3]], dtype=float)
        b = np.array([1, 2], dtype=float)
        return np.dot(A, x) + b
    
    def quadratic_hess(x: np.ndarray) -> np.ndarray:
        A = np.array([[4, 1], [1, 3]], dtype=float)
        return A
    
    # Начальная точка
    x0 = np.array([5.0, 5.0])
    
    # Запускаем алгоритм
    result = trust_region_algorithm(
        f=quadratic_f,
        grad=quadratic_grad,
        hess=quadratic_hess,
        x0=x0,
        epsilon=1e-6,
        delta0=1.0,
        eta=0.1,
        max_iter=50
    )
    
    # Выводим результаты
    print_algorithm_history(result, num_iterations=10)
    
    # Дополнительный тест с нелинейной функцией
    print("\n\n" + "="*80)
    print("ТЕСТ С НЕЛИНЕЙНОЙ ФУНКЦИЕЙ РОЗЕНБРОКА")
    print("="*80)
    
    def rosenbrock_f(x: np.ndarray) -> float:
        """Функция Розенброка: f(x,y) = (1-x)^2 + 100*(y-x^2)^2"""
        return (1 - x[0])**2 + 100 * (x[1] - x[0]**2)**2
    
    def rosenbrock_grad(x: np.ndarray) -> np.ndarray:
        grad_x = -2*(1 - x[0]) - 400*x[0]*(x[1] - x[0]**2)
        grad_y = 200*(x[1] - x[0]**2)
        return np.array([grad_x, grad_y])
    
    def rosenbrock_hess(x: np.ndarray) -> np.ndarray:
        h_xx = 2 - 400*x[1] + 1200*x[0]**2
        h_xy = -400*x[0]
        h_yy = 200
        return np.array([[h_xx, h_xy], [h_xy, h_yy]])
    
    x0_rosen = np.array([-1.2, 1.0])
    
    result_rosen = trust_region_algorithm(
        f=rosenbrock_f,
        grad=rosenbrock_grad,
        hess=rosenbrock_hess,
        x0=x0_rosen,
        epsilon=1e-6,
        delta0=0.5,
        eta=0.1,
        max_iter=100
    )
    
    print(f"\nФункция Розенброка:")
    print(f"Начальная точка: {x0_rosen}")
    print(f"Найденное решение: {result_rosen['x_star']}")
    print(f"Значение функции: {result_rosen['f_star']:.8f}")
    print(f"Итераций: {result_rosen['iterations']}")
    print(f"Сошёлся: {result_rosen['converged']}")