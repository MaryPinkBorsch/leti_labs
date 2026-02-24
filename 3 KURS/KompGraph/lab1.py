import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
from matplotlib.widgets import Button, TextBox
import matplotlib.patches as mpatches

class TriangleRotator:
    def __init__(self):
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
        
        # Создаем фигуру и оси
        self.fig, self.ax = plt.subplots(figsize=(10, 8))
        plt.subplots_adjust(bottom=0.25)
        
        # Настройка отображения
        self.setup_plot()
        
        # Создание элементов управления
        self.create_controls()
        
        # Первоначальная отрисовка
        self.update_plot()
        
        plt.show()
    
    def rotate_triangle(self):
        """
        Поворот треугольника вокруг заданной точки по часовой стрелке
        с использованием матрицы поворота
        """
        # Преобразование угла в радианы
        angle_rad = np.radians(self.angle_degrees)
        
        # Матрица поворота ПО ЧАСОВОЙ СТРЕЛКЕ
        # Для поворота по часовой стрелке используем отрицательный угол
        # или меняем знаки в матрице
        rotation_matrix = np.array([
            [np.cos(angle_rad), np.sin(angle_rad)],   # +sin вместо -sin
            [-np.sin(angle_rad), np.cos(angle_rad)]   # -sin в нижнем ряду
        ])
        
        # Перенос точки поворота в начало координат
        translated_triangle = self.original_triangle - self.rotation_point
        
        # Поворот (для вектор-строк используем транспонированную матрицу)
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
    
    def create_controls(self):
        """Создание элементов управления"""
        # Поле для ввода координаты X точки поворота
        ax_x = plt.axes([0.15, 0.05, 0.1, 0.05])
        self.x_box = TextBox(ax_x, 'X точки:', initial=str(self.rotation_point[0]))
        self.x_box.on_submit(self.update_rotation_point_x)
        
        # Поле для ввода координаты Y точки поворота
        ax_y = plt.axes([0.35, 0.05, 0.1, 0.05])
        self.y_box = TextBox(ax_y, 'Y точки:', initial=str(self.rotation_point[1]))
        self.y_box.on_submit(self.update_rotation_point_y)
        
        # Поле для ввода угла поворота (по часовой стрелке)
        ax_angle = plt.axes([0.55, 0.05, 0.1, 0.05])
        self.angle_box = TextBox(ax_angle, 'Угол:', initial=str(self.angle_degrees))
        self.angle_box.on_submit(self.update_angle)
        
        # Кнопка сброса
        ax_reset = plt.axes([0.75, 0.05, 0.1, 0.05])
        self.reset_button = Button(ax_reset, 'Сброс')
        self.reset_button.on_clicked(self.reset)
    
    def update_rotation_point_x(self, text):
        """Обновление X координаты точки поворота"""
        try:
            self.rotation_point[0] = float(text)
            self.update_plot()
        except ValueError:
            pass
    
    def update_rotation_point_y(self, text):
        """Обновление Y координаты точки поворота"""
        try:
            self.rotation_point[1] = float(text)
            self.update_plot()
        except ValueError:
            pass
    
    def update_angle(self, text):
        """Обновление угла поворота"""
        try:
            self.angle_degrees = float(text)
            self.update_plot()
        except ValueError:
            pass
    
    def reset(self, event):
        """Сброс к начальным значениям"""
        self.original_triangle = np.array([[2, 1], [4, 1], [3, 3]])
        self.rotation_point = np.array([3, 2])
        self.angle_degrees = 45
        
        # Обновляем поля ввода
        self.x_box.set_val(str(self.rotation_point[0]))
        self.y_box.set_val(str(self.rotation_point[1]))
        self.angle_box.set_val(str(self.angle_degrees))
        
        self.update_plot()
    
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
        self.fig.canvas.draw_idle()


# Альтернативная версия с явным указанием направления
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
        self.center = np.array([3, 2])
        self.angle = 45
        
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
        plt.show()
    
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


# Демонстрация разницы между направлениями поворота
class RotationDirectionDemo:
    """Класс для демонстрации разницы между направлениями поворота"""
    
    def __init__(self):
        # Создаем простую фигуру (треугольник)
        self.triangle = np.array([[2, 1], [4, 1], [3, 3]])
        self.center = np.array([3, 2])
        
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
        plt.show()
    
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


# Запуск основной программы
if __name__ == "__main__":
    # print("=" * 60)
    # print("ПРОГРАММА ПОВОРОТА ГЕОМЕТРИЧЕСКИХ ФИГУР")
    # print("=" * 60)
    # print("\nВыберите режим работы:")
    # print("1. Интерактивный режим с управлением (по часовой стрелке)")
    # print("2. Демонстрация разницы между направлениями поворота")
    # print("3. Сравнение по и против часовой стрелки")
    
    # choice = input("\nВведите номер режима (1-3): ").strip()
    
    # if choice == "1":
    print("\nЗапуск интерактивного режима...")
    print("Положительный угол = поворот по часовой стрелке")
    print("Изменяйте координаты точки и угол в текстовых полях.")
    app = TriangleRotator()
    
    # elif choice == "2":
    #     print("\nДемонстрация разницы между направлениями...")
    #     demo = RotationDirectionDemo()
    #     demo.demonstrate_directions()
    
    # elif choice == "3":
    #     print("\nСравнение направлений...")
    #     vector_demo = VectorTriangleRotator()
    #     vector_demo.demonstrate()
    
    # else:
    #     print("\nЗапуск интерактивного режима по умолчанию...")
    #     app = TriangleRotator()