import numpy as np
import matplotlib
# Устанавливаем неинтерактивный бэкенд Agg для сохранения в файлы
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import matplotlib.patches as mpatches
import os
from datetime import datetime

class TriangleRotator:
    def __init__(self, save_to_file=True):
        # Исходный треугольник (координаты вершин)
        self.original_triangle = np.array([
            [2, 1],  # вершина A
            [4, 1],  # вершина B
            [3, 3]   # вершина C
        ])
        
        # Точка поворота (по умолчанию центр треугольника)
        self.rotation_point = np.array([3, 2])
        
        # Угол поворота в градусах (положительный - по часовой стрелке)
        self.angle_degrees = 45
        
        # Создаем папку для сохранения изображений, если её нет
        self.output_dir = "rotation_output"
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
        
        # Создаем фигуру и оси
        self.fig, self.ax = plt.subplots(figsize=(10, 8))
        
        # Настройка отображения
        self.setup_plot()
        
        # Первоначальная отрисовка
        self.update_plot()
        
        # Сохраняем изображение
        if save_to_file:
            self.save_current_plot()
    
    def rotate_triangle(self):
        """
        Поворот треугольника вокруг заданной точки по часовой стрелке
        с использованием матрицы поворота
        """
        # Преобразование угла в радианы
        angle_rad = np.radians(self.angle_degrees)
        
        # Матрица поворота ПО ЧАСОВОЙ СТРЕЛКЕ
        rotation_matrix = np.array([
            [np.cos(angle_rad), np.sin(angle_rad)],
            [-np.sin(angle_rad), np.cos(angle_rad)]
        ])
        
        # Перенос точки поворота в начало координат
        translated_triangle = self.original_triangle - self.rotation_point
        
        # Поворот
        rotated_triangle = np.dot(translated_triangle, rotation_matrix.T)
        
        # Обратный перенос
        rotated_triangle = rotated_triangle + self.rotation_point
        
        return rotated_triangle
    
    def setup_plot(self):
        """Настройка внешнего вида графика"""
        self.ax.set_xlim(-2, 8)
        self.ax.set_ylim(-2, 8)
        self.ax.set_aspect('equal')
        self.ax.grid(True, alpha=0.3)
        self.ax.set_xlabel('X')
        self.ax.set_ylabel('Y')
        self.ax.set_title('Поворот треугольника по часовой стрелке вокруг произвольной точки')
        
        # Добавляем легенду
        self.ax.legend(handles=[
            mpatches.Patch(color='blue', alpha=0.3, label='Исходный треугольник'),
            mpatches.Patch(color='red', alpha=0.3, label='Повернутый (по часовой стрелке)'),
            plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='green', 
                      markersize=10, label='Точка поворота')
        ])
    
    def update_plot(self):
        """Обновление отображения"""
        self.ax.clear()
        self.setup_plot()
        
        # Получаем повернутый треугольник
        rotated_triangle = self.rotate_triangle()
        
        # Создаем полигоны для заливки треугольников
        original_polygon = Polygon(self.original_triangle, alpha=0.3, color='blue', label='Исходный')
        rotated_polygon = Polygon(rotated_triangle, alpha=0.3, color='red', label='Повернутый')
        
        # Добавляем треугольники на график
        self.ax.add_patch(original_polygon)
        self.ax.add_patch(rotated_polygon)
        
        # Рисуем вершины треугольников
        self.ax.plot(self.original_triangle[:, 0], self.original_triangle[:, 1], 
                    'bo', markersize=8)
        self.ax.plot(rotated_triangle[:, 0], rotated_triangle[:, 1], 
                    'ro', markersize=8)
        
        # Соединяем соответствующие вершины треугольников линиями
        for i in range(3):
            self.ax.plot([self.original_triangle[i, 0], rotated_triangle[i, 0]],
                        [self.original_triangle[i, 1], rotated_triangle[i, 1]],
                        'k--', alpha=0.5, linewidth=0.5)
        
        # Отмечаем точку поворота
        self.ax.plot(self.rotation_point[0], self.rotation_point[1], 
                    'go', markersize=10, label='Точка поворота')
        
        # Добавляем информацию о точке поворота и угле
        direction = "по часовой стрелке"
        info_text = f'Точка поворота: ({self.rotation_point[0]:.1f}, {self.rotation_point[1]:.1f})\nУгол: {self.angle_degrees:.1f}° {direction}'
        self.ax.text(0.02, 0.98, info_text, transform=self.ax.transAxes, 
                    verticalalignment='top', bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))
        
        self.ax.legend()
    
    def save_current_plot(self, filename=None):
        """Сохранение текущего графика в файл"""
        if filename is None:
            # Генерируем имя файла на основе параметров
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            filename = f"triangle_rot_{self.angle_degrees}deg_point_{self.rotation_point[0]}_{self.rotation_point[1]}_{timestamp}.png"
        
        filepath = os.path.join(self.output_dir, filename)
        self.fig.savefig(filepath, dpi=150, bbox_inches='tight')
        print(f"График сохранен: {filepath}")
        return filepath


class VectorTriangleRotator:
    """
    Альтернативная версия с использованием вектор-строк
    """
    
    def __init__(self):
        self.triangle = np.array([
            [2, 1],
            [4, 1],
            [3, 3]
        ])
        self.center = np.array([4, 4])
        self.angle = 45
        
        # Создаем папку для сохранения
        self.output_dir = "vector_rotation_output_copy"
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
        
    def rotate_points(self, points, center, angle_degrees, clockwise=True):
        """
        Поворот набора точек вокруг центра с использованием вектор-строк
        
        Параметры:
        points - массив точек (каждая строка - точка [x, y])
        center - центр поворота
        angle_degrees - угол поворота в градусах
        clockwise - True для поворота по часовой стрелке, False для против
        
        Возвращает:
        Массив повернутых точек
        """
        # Преобразование угла в радианы
        angle_rad = np.radians(angle_degrees)
        
        if clockwise:
            # Матрица поворота ПО ЧАСОВОЙ СТРЕЛКЕ
            rotation_matrix = np.array([
                [np.cos(angle_rad), np.sin(angle_rad)],
                [-np.sin(angle_rad), np.cos(angle_rad)]
            ])
        else:
            # Матрица поворота ПРОТИВ ЧАСОВОЙ СТРЕЛКИ
            rotation_matrix = np.array([
                [np.cos(angle_rad), -np.sin(angle_rad)],
                [np.sin(angle_rad), np.cos(angle_rad)]
            ])
        
        # Перенос в начало координат, поворот, обратный перенос
        translated = points - center
        rotated = np.dot(translated, rotation_matrix)
        result = rotated + center
        
        return result
    
    def demonstrate(self):
        """Демонстрация работы с вектор-строками"""
        print("Исходные точки треугольника:")
        print(self.triangle)
        print(f"\nЦентр поворота: {self.center}")
        print(f"Угол поворота: {self.angle}° по часовой стрелке")
        
        rotated = self.rotate_points(self.triangle, self.center, self.angle, clockwise=True)
        print("\nПовернутые точки (по часовой стрелке):")
        print(rotated)
        
        # Визуализация
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 7))
        
        # По часовой стрелке
        ax1.set_title('Поворот ПО часовой стрелке')
        self.plot_triangles(ax1, self.triangle, rotated, self.center)
        
        # Против часовой стрелки для сравнения
        rotated_ccw = self.rotate_points(self.triangle, self.center, self.angle, clockwise=False)
        ax2.set_title('Поворот ПРОТИВ часовой стрелки')
        self.plot_triangles(ax2, self.triangle, rotated_ccw, self.center)
        
        plt.tight_layout()
        
        # Сохраняем изображение
        filename = os.path.join(self.output_dir, f"vector_demo_{self.angle}deg.png")
        fig.savefig(filename, dpi=150, bbox_inches='tight')
        print(f"График сохранен: {filename}")
        plt.close(fig)
    
    def plot_triangles(self, ax, original, rotated, center):
        """Вспомогательная функция для отрисовки треугольников"""
        # Полигоны
        original_polygon = Polygon(original, alpha=0.3, color='blue')
        rotated_polygon = Polygon(rotated, alpha=0.3, color='red')
        
        ax.add_patch(original_polygon)
        ax.add_patch(rotated_polygon)
        
        # Точки
        ax.plot(original[:, 0], original[:, 1], 'bo', markersize=8)
        ax.plot(rotated[:, 0], rotated[:, 1], 'ro', markersize=8)
        ax.plot(center[0], center[1], 'go', markersize=10)
        
        ax.set_aspect('equal')
        ax.grid(True, alpha=0.3)
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        
        # Легенда
        ax.legend(handles=[
            mpatches.Patch(color='blue', alpha=0.3, label='Исходный'),
            mpatches.Patch(color='red', alpha=0.3, label='Повернутый'),
            plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='green', 
                      markersize=10, label='Центр')
        ])


class RotationDirectionDemo:
    """Класс для демонстрации разницы между направлениями поворота"""
    
    def __init__(self):
        # Создаем простую фигуру (треугольник)
        self.triangle = np.array([[2, 1], [4, 1], [3, 3]])
        self.center = np.array([3, 2])
        
        # Создаем папку для сохранения
        self.output_dir = "direction_demo_output"
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
        
    def demonstrate_directions(self):
        """Показывает разницу между поворотом по и против часовой стрелки"""
        angles = [30, 60, 90, 120]
        
        fig, axes = plt.subplots(2, 4, figsize=(16, 8))
        fig.suptitle('Сравнение направлений поворота', fontsize=16)
        
        for i, angle in enumerate(angles):
            # По часовой стрелке (верхний ряд)
            ax_cw = axes[0, i]
            rotated_cw = self.rotate_points(self.triangle, self.center, angle, clockwise=True)
            self.plot_with_direction(ax_cw, self.triangle, rotated_cw, self.center, 
                                    angle, "По часовой")
            
            # Против часовой стрелки (нижний ряд)
            ax_ccw = axes[1, i]
            rotated_ccw = self.rotate_points(self.triangle, self.center, angle, clockwise=False)
            self.plot_with_direction(ax_ccw, self.triangle, rotated_ccw, self.center, 
                                    angle, "Против часовой")
        
        plt.tight_layout()
        
        # Сохраняем изображение
        filename = os.path.join(self.output_dir, "direction_comparison.png")
        fig.savefig(filename, dpi=150, bbox_inches='tight')
        print(f"График сохранен: {filename}")
        plt.close(fig)
    
    def rotate_points(self, points, center, angle_degrees, clockwise=True):
        """Поворот точек"""
        angle_rad = np.radians(angle_degrees)
        
        if clockwise:
            rotation_matrix = np.array([
                [np.cos(angle_rad), np.sin(angle_rad)],
                [-np.sin(angle_rad), np.cos(angle_rad)]
            ])
        else:
            rotation_matrix = np.array([
                [np.cos(angle_rad), -np.sin(angle_rad)],
                [np.sin(angle_rad), np.cos(angle_rad)]
            ])
        
        translated = points - center
        rotated = np.dot(translated, rotation_matrix)
        return rotated + center
    
    def plot_with_direction(self, ax, original, rotated, center, angle, direction):
        """Отрисовка с указанием направления"""
        # Полигоны
        ax.add_patch(Polygon(original, alpha=0.3, color='blue'))
        ax.add_patch(Polygon(rotated, alpha=0.3, color='red'))
        
        # Точки
        ax.plot(original[:, 0], original[:, 1], 'bo', markersize=6)
        ax.plot(rotated[:, 0], rotated[:, 1], 'ro', markersize=6)
        ax.plot(center[0], center[1], 'go', markersize=8)
        
        ax.set_title(f'{direction}: {angle}°')
        ax.set_aspect('equal')
        ax.grid(True, alpha=0.3)
        ax.set_xlim(0, 6)
        ax.set_ylim(0, 5)


# Функция для генерации нескольких изображений с разными параметрами
def generate_multiple_rotations():
    """Генерирует несколько изображений с разными углами поворота"""
    print("Генерация изображений с разными углами поворота...")
    
    angles = [0, 30, 45, 60, 90, 120, 180]
    rotation_points = [
        ([3, 2], "center"),
        ([2, 1], "vertex_a"),
        ([4, 1], "vertex_b")
    ]
    
    for point_coords, point_name in rotation_points:
        for angle in angles:
            rotator = TriangleRotator(save_to_file=False)
            rotator.rotation_point = np.array(point_coords)
            rotator.angle_degrees = angle
            rotator.update_plot()
            
            filename = f"rotation_{point_name}_angle_{angle}deg.png"
            rotator.save_current_plot(filename)
            plt.close(rotator.fig)
    
    print("Все изображения сгенерированы!")


# Запуск основной программы
if __name__ == "__main__":
    # print("=" * 60)
    # print("ПРОГРАММА ПОВОРОТА ГЕОМЕТРИЧЕСКИХ ФИГУР")
    # print("=" * 60)
    # print("\nВыберите режим работы:")
    # print("1. Сохранить один график с текущими параметрами")
    # print("2. Демонстрация разницы между направлениями поворота")
    # print("3. Сравнение по и против часовой стрелки")
    # print("4. Сгенерировать несколько изображений с разными углами")
    
    # choice = input("\nВведите номер режима (1-4): ").strip()
    
    # if choice == "1":
    #     print("\nСохранение графика с текущими параметрами...")
    #     print("Параметры: угол 45°, точка поворота (3, 2)")
    #     app = TriangleRotator(save_to_file=True)
    #     plt.close(app.fig)  # Закрываем фигуру после сохранения
        
    # elif choice == "2":
    #     print("\nДемонстрация разницы между направлениями...")
    #     demo = RotationDirectionDemo()
    #     demo.demonstrate_directions()
    
    # elif choice == "3":
    #     print("\nСравнение направлений...")
    #     vector_demo = VectorTriangleRotator()
    #     vector_demo.demonstrate()
        
    # elif choice == "4":
    #     print("\nГенерация нескольких изображений...")
    #     generate_multiple_rotations()
    
    # else:
    #     print("\nНеверный выбор. Сохраняю график по умолчанию...")
    #     app = TriangleRotator(save_to_file=True)
    #     plt.close(app.fig)
    print("\nГенерация нескольких изображений...")
    generate_multiple_rotations()
    
    print("\nГотово! Все изображения сохранены в соответствующие папки.")