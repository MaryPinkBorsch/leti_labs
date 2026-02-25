import numpy as np
import matplotlib
# Устанавливаем неинтерактивный бэкенд Agg для сохранения в файлы
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import matplotlib.patches as mpatches
import os
from datetime import datetime

# Проверяем наличие scipy для комбинаторики
try:
    from scipy.special import comb
    SCIPY_AVAILABLE = True
except ImportError:
    SCIPY_AVAILABLE = False
    print("Scipy не установлен. Используется простая реализация comb.")
    
    def comb(n, k):
        """Простая реализация биномиального коэффициента"""
        if k < 0 or k > n:
            return 0
        k = min(k, n - k)
        result = 1
        for i in range(1, k + 1):
            result = result * (n - k + i) // i
        return result


class BezierCurveEditor:
    def __init__(self, save_to_file=True):
        # Исходные точки задающей ломаной (минимум 10 точек)
        self.control_points = np.array([
            [1, 1],   # P0
            [2, 3],   # P1
            [3, 2],   # P2
            [4, 4],   # P3
            [5, 2],   # P4
            [6, 3],   # P5
            [7, 1],   # P6
            [8, 3],   # P7
            [9, 2],   # P8
            [10, 4],  # P9
            [11, 2],  # P10
            [12, 3]   # P11 (12 точек для демонстрации)
        ])
        
        # Создаем папку для сохранения изображений
        self.output_dir = "bezier_output"
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
        
        # Параметры кривой Безье
        self.t_values = np.linspace(0, 1, 100)
        
        # Создаем фигуру и оси
        self.fig, self.ax = plt.subplots(figsize=(14, 8))
        
        # Настройка отображения
        self.setup_plot()
        
        # Отрисовка кривой
        self.update_plot()
        
        # Сохраняем изображение
        if save_to_file:
            self.save_current_plot("initial_curve.png")
    
    def setup_plot(self):
        """Настройка внешнего вида графика"""
        self.ax.set_xlim(0, 14)
        self.ax.set_ylim(0, 6)
        self.ax.set_aspect('equal')
        self.ax.grid(True, alpha=0.3, linestyle='--')
        self.ax.set_xlabel('X', fontsize=12)
        self.ax.set_ylabel('Y', fontsize=12)
        self.ax.set_title('Составная кривая Безье третьей степени', fontsize=14, fontweight='bold')
        
        # Добавляем информацию
        info_text = 'Красные точки - управляющие точки\nСиняя линия - задающая ломаная\nЗеленая кривая - составной сплайн Безье'
        self.ax.text(0.02, 0.98, info_text, transform=self.ax.transAxes, 
                    verticalalignment='top', fontsize=10,
                    bbox=dict(boxstyle='round', facecolor='lightyellow', alpha=0.8))
    
    def bernstein_poly(self, i, n, t):
        """Полином Бернштейна"""
        return comb(n, i) * (t**i) * ((1 - t)**(n - i))
    
    def bezier_curve_segment(self, points, t):
        """
        Вычисление точки на кривой Безье по заданным управляющим точкам
        points - массив из 4 точек (для кривой третьей степени)
        """
        n = len(points) - 1
        point = np.zeros(2)
        for i in range(len(points)):
            point += points[i] * self.bernstein_poly(i, n, t)
        return point
    
    def compute_composite_bezier(self):
        """
        Вычисление составной кривой Безье третьей степени
        """
        bezier_points = []
        
        # Проходим по всем сегментам (каждые 4 точки образуют один сегмент Безье)
        for i in range(0, len(self.control_points) - 3, 3):
            segment_points = self.control_points[i:i+4]
            
            # Проверяем, что у нас достаточно точек для сегмента
            if len(segment_points) == 4:
                segment_curve = []
                for t in self.t_values:
                    point = self.bezier_curve_segment(segment_points, t)
                    segment_curve.append(point)
                bezier_points.extend(segment_curve)
        
        return np.array(bezier_points)
    
    def update_plot(self):
        """Обновление отображения"""
        self.ax.clear()
        self.setup_plot()
        
        # Вычисляем составную кривую Безье
        bezier_points = self.compute_composite_bezier()
        
        # Отрисовка задающей ломаной
        self.ax.plot(self.control_points[:, 0], self.control_points[:, 1], 
                    'b-', linewidth=2, alpha=0.7, label='Задающая ломаная')
        
        # Отрисовка кривой Безье
        if len(bezier_points) > 0:
            self.ax.plot(bezier_points[:, 0], bezier_points[:, 1], 
                        'g-', linewidth=3, label='Кривая Безье')
        
        # Отрисовка управляющих точек
        self.ax.scatter(self.control_points[:, 0], self.control_points[:, 1], 
                       color='red', s=100, zorder=5, label='Управляющие точки')
        
        # Подписываем точки
        for i, (x, y) in enumerate(self.control_points):
            self.ax.annotate(f'P{i}', (x, y), xytext=(5, 5), 
                           textcoords='offset points', fontsize=9,
                           bbox=dict(boxstyle='round,pad=0.3', facecolor='white', alpha=0.7))
        
        self.ax.legend(loc='upper right')
    
    def save_current_plot(self, filename=None):
        """Сохранение текущего графика в файл"""
        if filename is None:
            # Генерируем имя файла на основе времени
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            num_points = len(self.control_points)
            filename = f"bezier_curve_{num_points}points_{timestamp}.png"
        
        filepath = os.path.join(self.output_dir, filename)
        self.fig.savefig(filepath, dpi=150, bbox_inches='tight')
        print(f"График сохранен: {filepath}")
        return filepath


class BezierCurveGenerator:
    """Класс для генерации различных кривых Безье"""
    
    def __init__(self):
        self.output_dir = "bezier_generated"
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
    
    def generate_example_curves(self):
        """Генерация примеров различных кривых Безье"""
        
        # Пример 1: Простая симметричная кривая
        points1 = np.array([
            [1, 1], [2, 4], [3, 1], [4, 4], [5, 1], [6, 4], [7, 1]
        ])
        self.save_bezier_curve(points1, "symmetrical_curve.png", "Симметричная кривая")
        
        # Пример 2: Кривая с петлей
        points2 = np.array([
            [1, 2], [3, 5], [5, 2], [7, 5], [9, 2], [11, 5]
        ])
        self.save_bezier_curve(points2, "looping_curve.png", "Кривая с петлями")
        
        # Пример 3: Плавная кривая
        points3 = np.array([
            [1, 1], [2, 3], [3, 3], [4, 2], [5, 3], [6, 4], [7, 3], [8, 2]
        ])
        self.save_bezier_curve(points3, "smooth_curve.png", "Плавная кривая")
        
        # Пример 4: Кривая с резкими изменениями
        points4 = np.array([
            [1, 1], [2, 5], [3, 1], [4, 5], [5, 1], [6, 5], [7, 1], [8, 5]
        ])
        self.save_bezier_curve(points4, "sharp_curve.png", "Кривая с резкими изменениями")
        
        print("Все примеры кривых сгенерированы!")
    
    def save_bezier_curve(self, control_points, filename, title):
        """Сохранение кривой Безье в файл"""
        fig, ax = plt.subplots(figsize=(12, 6))
        
        # Настройка отображения
        ax.set_xlim(0, 12)
        ax.set_ylim(0, 6)
        ax.set_aspect('equal')
        ax.grid(True, alpha=0.3, linestyle='--')
        ax.set_xlabel('X', fontsize=12)
        ax.set_ylabel('Y', fontsize=12)
        ax.set_title(f'{title}', fontsize=14, fontweight='bold')
        
        # Вычисление кривой Безье
        bezier_points = self.compute_bezier_curve(control_points)
        
        # Отрисовка задающей ломаной
        ax.plot(control_points[:, 0], control_points[:, 1], 
                'b-', linewidth=2, alpha=0.7, label='Задающая ломаная')
        
        # Отрисовка кривой Безье
        if len(bezier_points) > 0:
            ax.plot(bezier_points[:, 0], bezier_points[:, 1], 
                    'g-', linewidth=3, label='Кривая Безье')
        
        # Отрисовка управляющих точек
        ax.scatter(control_points[:, 0], control_points[:, 1], 
                   color='red', s=100, zorder=5, label='Управляющие точки')
        
        # Подписываем точки
        for i, (x, y) in enumerate(control_points):
            ax.annotate(f'P{i}', (x, y), xytext=(5, 5), 
                       textcoords='offset points', fontsize=9,
                       bbox=dict(boxstyle='round,pad=0.3', facecolor='white', alpha=0.7))
        
        ax.legend(loc='upper right')
        
        # Сохранение
        filepath = os.path.join(self.output_dir, filename)
        fig.savefig(filepath, dpi=150, bbox_inches='tight')
        plt.close(fig)
        print(f"Сохранено: {filepath}")
    
    def compute_bezier_curve(self, control_points, num_points=100):
        """Вычисление составной кривой Безье"""
        t_values = np.linspace(0, 1, num_points)
        bezier_points = []
        
        for i in range(0, len(control_points) - 3, 3):
            segment_points = control_points[i:i+4]
            if len(segment_points) == 4:
                segment_curve = []
                for t in t_values:
                    point = self.bezier_point(segment_points, t)
                    segment_curve.append(point)
                bezier_points.extend(segment_curve)
        
        return np.array(bezier_points)
    
    def bezier_point(self, points, t):
        """Вычисление точки на кривой Безье"""
        n = len(points) - 1
        point = np.zeros(2)
        for i in range(len(points)):
            point += points[i] * self.bernstein(i, n, t)
        return point
    
    def bernstein(self, i, n, t):
        """Полином Бернштейна"""
        return comb(n, i) * (t**i) * ((1 - t)**(n - i))


def generate_with_different_point_counts():
    """Генерация кривых с разным количеством точек"""
    print("Генерация кривых с разным количеством точек...")
    
    output_dir = "bezier_point_counts"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # Базовые точки
    base_points = np.array([
        [1, 2], [2, 4], [3, 2], [4, 4], [5, 2], [6, 4], [7, 2], [8, 4], [9, 2], [10, 4]
    ])
    
    # Кривые с разным количеством точек
    point_counts = [4, 7, 10, 13, 16]
    
    for count in point_counts:
        # Берем первые count точек из базового набора
        points = base_points[:min(count, len(base_points))]
        
        # Если нужно больше точек, дублируем с небольшим смещением
        while len(points) < count:
            last_points = points[-3:]
            new_points = last_points + np.array([1, 0])
            points = np.vstack([points, new_points])
        
        fig, ax = plt.subplots(figsize=(12, 6))
        
        # Настройка
        ax.set_xlim(0, max(15, count))
        ax.set_ylim(0, 6)
        ax.set_aspect('equal')
        ax.grid(True, alpha=0.3)
        ax.set_title(f'Кривая Безье с {count} точками', fontsize=14)
        
        # Вычисление кривой
        t_values = np.linspace(0, 1, 100)
        bezier_points = []
        
        for i in range(0, len(points) - 3, 3):
            segment = points[i:i+4]
            if len(segment) == 4:
                segment_curve = []
                for t in t_values:
                    n = 3
                    point = np.zeros(2)
                    for j in range(4):
                        point += segment[j] * comb(n, j) * (t**j) * ((1 - t)**(n - j))
                    segment_curve.append(point)
                bezier_points.extend(segment_curve)
        
        bezier_points = np.array(bezier_points)
        
        # Отрисовка
        ax.plot(points[:, 0], points[:, 1], 'b-', linewidth=2, alpha=0.7, label='Ломаная')
        if len(bezier_points) > 0:
            ax.plot(bezier_points[:, 0], bezier_points[:, 1], 'g-', linewidth=3, label='Безье')
        ax.scatter(points[:, 0], points[:, 1], color='red', s=80, zorder=5)
        
        ax.legend()
        
        # Сохранение
        filename = f"bezier_{count}_points.png"
        filepath = os.path.join(output_dir, filename)
        fig.savefig(filepath, dpi=150, bbox_inches='tight')
        plt.close(fig)
        print(f"Сохранено: {filepath}")
    
    print("Готово!")


# Запуск программы
if __name__ == "__main__":
    # print("=" * 60)
    # print("ГЕНЕРАТОР КРИВЫХ БЕЗЬЕ")
    # print("=" * 60)
    # print("\nВыберите режим работы:")
    # print("1. Сохранить один график с текущими параметрами")
    # print("2. Сгенерировать примеры различных кривых")
    # print("3. Сгенерировать кривые с разным количеством точек")
    # print("4. Сохранить несколько вариантов с разными наборами точек")
    
    # choice = input("\nВведите номер режима (1-4): ").strip()
    
    # if choice == "1":
    #     print("\nСохранение графика с текущими параметрами...")
    #     app = BezierCurveEditor(save_to_file=True)
    #     plt.close(app.fig)
    #     print(f"График сохранен в папку '{app.output_dir}'")
        
    # elif choice == "2":
    #     print("\nГенерация примеров различных кривых...")
    #     generator = BezierCurveGenerator()
    #     generator.generate_example_curves()
        
    # elif choice == "3":
    #     print("\nГенерация кривых с разным количеством точек...")
    #     generate_with_different_point_counts()
        
    # elif choice == "4":
    #     print("\nСохранение нескольких вариантов...")
        
    #     # Создаем несколько экземпляров с разными наборами точек
    #     variations = [
    #         ("стандартный", None),  # используем точки по умолчанию
    #         ("волна", np.array([[1,2], [2,4], [3,2], [4,4], [5,2], [6,4], [7,2], [8,4]])),
    #         ("горка", np.array([[1,1], [2,3], [3,4], [4,3], [5,1], [6,3], [7,4], [8,3], [9,1]])),
    #         ("петля", np.array([[1,2], [3,5], [5,2], [7,5], [9,2], [11,5]]))
    #     ]
        
    #     for name, points in variations:
    #         app = BezierCurveEditor(save_to_file=False)
    #         if points is not None:
    #             app.control_points = points
    #         app.update_plot()
    #         app.save_current_plot(f"bezier_variation_{name}.png")
    #         plt.close(app.fig)
    #         print(f"Вариант '{name}' сохранен")
    
    # else:
    #     print("\nНеверный выбор. Сохраняю график по умолчанию...")
    #     app = BezierCurveEditor(save_to_file=True)
    #     plt.close(app.fig)
    print("\nСохранение нескольких вариантов...")
        
    # Создаем несколько экземпляров с разными наборами точек
    variations = [
        ("стандартный", None),  # используем точки по умолчанию
        ("волна", np.array([[1,2], [2,4], [3,2], [4,4], [5,2], [6,4], [7,2], [8,4]])),
        ("горка", np.array([[1,1], [2,3], [3,4], [4,3], [5,1], [6,3], [7,4], [8,3], [9,1]])),
        ("петля", np.array([[1,2], [3,5], [5,2], [7,5], [9,2], [11,5],[12,2]]))
    ]
    
    for name, points in variations:
        app = BezierCurveEditor(save_to_file=False)
        if points is not None:
            app.control_points = points
        app.update_plot()
        app.save_current_plot(f"bezier_variation_{name}.png")
        plt.close(app.fig)
        print(f"Вариант '{name}' сохранен")
    
    print("\nГотово! Все изображения сохранены в соответствующие папки.")