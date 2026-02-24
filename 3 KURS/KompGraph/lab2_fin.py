import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Button, TextBox
from matplotlib.patches import Polygon
import matplotlib.patches as mpatches

class BezierCurveEditor:
    def __init__(self):
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
        
        # Индекс выбранной точки для редактирования
        self.selected_point_idx = 0
        
        # Создаем фигуру и оси
        self.fig, self.ax = plt.subplots(figsize=(14, 8))
        plt.subplots_adjust(bottom=0.25, left=0.1, right=0.9)
        
        # Параметры кривой Безье
        self.t_values = np.linspace(0, 1, 100)
        
        # Настройка отображения
        self.setup_plot()
        
        # Создание элементов управления
        self.create_controls()
        
        # Отрисовка кривой
        self.update_plot()
        
        plt.show()
    
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
        from scipy.special import comb
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
        
        # Выделяем выбранную точку
        if self.selected_point_idx < len(self.control_points):
            x, y = self.control_points[self.selected_point_idx]
            self.ax.scatter([x], [y], color='purple', s=200, zorder=6, 
                          marker='s', label='Выбранная точка')
        
        self.ax.legend(loc='upper right')
        self.fig.canvas.draw_idle()
    
    def create_controls(self):
        """Создание элементов управления"""
        # Панель для выбора индекса точки
        ax_idx = plt.axes([0.15, 0.12, 0.15, 0.05])
        self.idx_box = TextBox(ax_idx, 'Индекс точки:', initial=str(self.selected_point_idx))
        self.idx_box.on_submit(self.select_point)
        
        # Поле для ввода координаты X
        ax_x = plt.axes([0.35, 0.12, 0.1, 0.05])
        self.x_box = TextBox(ax_x, 'X:', initial=str(self.control_points[self.selected_point_idx, 0]))
        self.x_box.on_submit(self.update_point_x)
        
        # Поле для ввода координаты Y
        ax_y = plt.axes([0.5, 0.12, 0.1, 0.05])
        self.y_box = TextBox(ax_y, 'Y:', initial=str(self.control_points[self.selected_point_idx, 1]))
        self.y_box.on_submit(self.update_point_y)
        
        # Кнопки для навигации по точкам
        ax_prev = plt.axes([0.65, 0.12, 0.07, 0.05])
        self.prev_button = Button(ax_prev, '◀ Пред.')
        self.prev_button.on_clicked(self.previous_point)
        
        ax_next = plt.axes([0.73, 0.12, 0.07, 0.05])
        self.next_button = Button(ax_next, 'След. ▶')
        self.next_button.on_clicked(self.next_point)
        
        # Кнопка добавления точки
        ax_add = plt.axes([0.15, 0.05, 0.12, 0.05])
        self.add_button = Button(ax_add, 'Добавить точку')
        self.add_button.on_clicked(self.add_point)
        
        # Кнопка удаления точки
        ax_remove = plt.axes([0.3, 0.05, 0.12, 0.05])
        self.remove_button = Button(ax_remove, 'Удалить точку')
        self.remove_button.on_clicked(self.remove_point)
        
        # Кнопка сброса
        ax_reset = plt.axes([0.55, 0.05, 0.1, 0.05])
        self.reset_button = Button(ax_reset, 'Сброс')
        self.reset_button.on_clicked(self.reset)
        
        # Кнопка случайные точки
        ax_random = plt.axes([0.7, 0.05, 0.15, 0.05])
        self.random_button = Button(ax_random, 'Случайные точки')
        self.random_button.on_clicked(self.random_points)
    
    def select_point(self, text):
        """Выбор точки по индексу"""
        try:
            idx = int(text)
            if 0 <= idx < len(self.control_points):
                self.selected_point_idx = idx
                self.update_coordinate_boxes()
                self.update_plot()
            else:
                print(f"Индекс должен быть от 0 до {len(self.control_points)-1}")
        except ValueError:
            pass
    
    def previous_point(self, event):
        """Переход к предыдущей точке"""
        if self.selected_point_idx > 0:
            self.selected_point_idx -= 1
            self.idx_box.set_val(str(self.selected_point_idx))
            self.update_coordinate_boxes()
            self.update_plot()
    
    def next_point(self, event):
        """Переход к следующей точке"""
        if self.selected_point_idx < len(self.control_points) - 1:
            self.selected_point_idx += 1
            self.idx_box.set_val(str(self.selected_point_idx))
            self.update_coordinate_boxes()
            self.update_plot()
    
    def update_point_x(self, text):
        """Обновление X координаты выбранной точки"""
        try:
            x = float(text)
            self.control_points[self.selected_point_idx, 0] = x
            self.update_plot()
        except ValueError:
            pass
    
    def update_point_y(self, text):
        """Обновление Y координаты выбранной точки"""
        try:
            y = float(text)
            self.control_points[self.selected_point_idx, 1] = y
            self.update_plot()
        except ValueError:
            pass
    
    def update_coordinate_boxes(self):
        """Обновление полей с координатами"""
        if self.selected_point_idx < len(self.control_points):
            self.x_box.set_val(str(self.control_points[self.selected_point_idx, 0]))
            self.y_box.set_val(str(self.control_points[self.selected_point_idx, 1]))
    
    def add_point(self, event):
        """Добавление новой точки"""
        # Добавляем точку рядом с последней
        last_point = self.control_points[-1]
        new_point = last_point + np.array([0.5, 0.5])
        self.control_points = np.vstack([self.control_points, new_point])
        self.update_plot()
        print(f"Добавлена точка P{len(self.control_points)-1}")
    
    def remove_point(self, event):
        """Удаление выбранной точки"""
        if len(self.control_points) > 10:  # Минимум 10 точек
            self.control_points = np.delete(self.control_points, self.selected_point_idx, axis=0)
            if self.selected_point_idx >= len(self.control_points):
                self.selected_point_idx = len(self.control_points) - 1
            self.idx_box.set_val(str(self.selected_point_idx))
            self.update_coordinate_boxes()
            self.update_plot()
            print(f"Точка удалена. Осталось {len(self.control_points)} точек")
        else:
            print("Нельзя удалить точку - минимальное количество 10")
    
    def reset(self, event):
        """Сброс к начальным значениям"""
        self.control_points = np.array([
            [1, 1], [2, 3], [3, 2], [4, 4], [5, 2],
            [6, 3], [7, 1], [8, 3], [9, 2], [10, 4],
            [11, 2], [12, 3]
        ])
        self.selected_point_idx = 0
        self.idx_box.set_val(str(self.selected_point_idx))
        self.update_coordinate_boxes()
        self.update_plot()
        print("Сброс к начальной конфигурации")
    
    def random_points(self, event):
        """Генерация случайных точек"""
        num_points = np.random.randint(10, 15)
        x = np.linspace(1, 13, num_points)
        y = 2 + 2 * np.random.random(num_points)
        self.control_points = np.column_stack([x, y])
        self.selected_point_idx = 0
        self.idx_box.set_val(str(self.selected_point_idx))
        self.update_coordinate_boxes()
        self.update_plot()
        print(f"Сгенерировано {num_points} случайных точек")


 


# Запуск программы
if __name__ == "__main__":
     
    
   
   
    print("\nЗапуск редактора составной кривой Безье...")
    print("\nИнструкция:")
    print(" Красные точки - управляющие точки задающей ломаной")
    print(" Синяя линия - задающая ломаная")
    print(" Зеленая кривая - составной сплайн Безье")
    print("\nУправление:")
    print(" Выберите индекс точки для редактирования")
    print(" Изменяйте координаты X и Y")
    print(" Используйте кнопки для навигации и добавления/удаления точек")
    print(" Минимальное количество точек - 10")
    
    app = BezierCurveEditor()