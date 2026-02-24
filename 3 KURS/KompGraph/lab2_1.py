import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
from matplotlib.patches import Polygon
import matplotlib.patches as mpatches

class BezierCurveEditor:
    def __init__(self):
        # Инициализация начальных точек задающей ломаной (12 точек для примера)
        self.control_points = np.array([
            [1, 1],   # P0
            [2, 3],   # P1
            [3, 2],   # P2
            [4, 4],   # P3
            [5, 3],   # P4
            [6, 5],   # P5
            [7, 4],   # P6
            [8, 6],   # P7
            [9, 5],   # P8
            [10, 7],  # P9
            [11, 6],  # P10
            [12, 8]   # P11
        ])
        
        # Параметры для кривой Безье
        self.t = np.linspace(0, 1, 100)  # Параметр t для построения кривой
        
        # Индекс выбранной точки для редактирования
        self.selected_point = None
        self.dragging = False
        
        # Создание фигуры и осей
        self.fig, self.ax = plt.subplots(figsize=(14, 8))
        plt.subplots_adjust(bottom=0.15)
        
        # Настройка отображения
        self.setup_plot()
        
        # Создание элементов управления
        self.create_controls()
        
        # Отрисовка кривой
        self.update_curve()
        
        # Подключение обработчиков событий мыши
        self.connect_events()
        
        plt.show()
    
    def setup_plot(self):
        """Настройка внешнего вида графика"""
        self.ax.set_xlim(-1, 15)
        self.ax.set_ylim(-1, 12)
        self.ax.set_aspect('equal')
        self.ax.grid(True, alpha=0.3, linestyle='--')
        self.ax.set_xlabel('X')
        self.ax.set_ylabel('Y')
        self.ax.set_title('Составная кривая Безье третьей степени\n(Перетаскивайте точки для редактирования)')
        
        # Добавляем легенду
        self.ax.legend(handles=[
            plt.Line2D([0], [0], color='red', linewidth=2, label='Кривая Безье'),
            plt.Line2D([0], [0], color='blue', linewidth=1, linestyle='--', label='Задающая ломаная'),
            plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='green', 
                      markersize=8, label='Опорные точки'),
            plt.Line2D([0], [0], marker='s', color='w', markerfacecolor='orange', 
                      markersize=8, label='Границы сегментов')
        ])
    
    def create_controls(self):
        """Создание элементов управления"""
        # Кнопка добавления точки
        ax_add = plt.axes([0.1, 0.02, 0.1, 0.05])
        self.add_button = Button(ax_add, 'Добавить точку')
        self.add_button.on_clicked(self.add_point)
        
        # Кнопка удаления последней точки
        ax_remove = plt.axes([0.25, 0.02, 0.15, 0.05])
        self.remove_button = Button(ax_remove, 'Удалить последнюю')
        self.remove_button.on_clicked(self.remove_point)
        
        # Кнопка сброса
        ax_reset = plt.axes([0.45, 0.02, 0.1, 0.05])
        self.reset_button = Button(ax_reset, 'Сбросить')
        self.reset_button.on_clicked(self.reset)
        
        # Кнопка случайных точек
        ax_random = plt.axes([0.6, 0.02, 0.15, 0.05])
        self.random_button = Button(ax_random, 'Случайные точки')
        self.random_button.on_clicked(self.random_points)
        
        # Информация о количестве точек
        self.info_text = self.ax.text(0.02, 0.98, '', transform=self.ax.transAxes,
                                      verticalalignment='top',
                                      bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))
    
    def connect_events(self):
        """Подключение обработчиков событий мыши"""
        self.fig.canvas.mpl_connect('button_press_event', self.on_press)
        self.fig.canvas.mpl_connect('button_release_event', self.on_release)
        self.fig.canvas.mpl_connect('motion_notify_event', self.on_motion)
    
    def bezier_curve(self, p0, p1, p2, p3, t):
        """
        Вычисление точки на кривой Безье третьей степени
        B(t) = (1-t)³P0 + 3(1-t)²tP1 + 3(1-t)t²P2 + t³P3
        """
        return (1-t)**3 * p0 + 3*(1-t)**2 * t * p1 + 3*(1-t) * t**2 * p2 + t**3 * p3
    
    def compute_bezier_segments(self):
        """
        Разбиение контрольных точек на сегменты по 4 точки
        и вычисление составной кривой Безье
        """
        n_points = len(self.control_points)
        
        # Нужно минимум 4 точки для одного сегмента
        if n_points < 4:
            return np.array([]), []
        
        # Список для хранения всех точек кривой
        bezier_points = []
        
        # Список для хранения границ сегментов
        segment_boundaries = []
        
        # Проходим по точкам с шагом 3 (перекрытие в 1 точку для гладкости)
        for i in range(0, n_points - 3, 3):
            # Получаем 4 контрольные точки для текущего сегмента
            p0, p1, p2, p3 = self.control_points[i:i+4]
            
            # Вычисляем точки кривой для текущего сегмента
            segment_points = np.array([self.bezier_curve(p0, p1, p2, p3, t_i) 
                                      for t_i in self.t])
            bezier_points.append(segment_points)
            
            # Запоминаем границу сегмента
            segment_boundaries.append(i + 3)
        
        # Объединяем все сегменты
        if bezier_points:
            return np.vstack(bezier_points), segment_boundaries
        else:
            return np.array([]), []
    
    def update_curve(self):
        """Обновление отображения кривой и контрольных точек"""
        self.ax.clear()
        self.setup_plot()
        
        # Получаем точки кривой Безье
        bezier_points, segment_boundaries = self.compute_bezier_segments()
        
        # Отрисовка задающей ломаной
        self.ax.plot(self.control_points[:, 0], self.control_points[:, 1], 
                    'b--', linewidth=1, alpha=0.7, label='Задающая ломаная')
        
        # Отрисовка опорных точек
        self.ax.scatter(self.control_points[:, 0], self.control_points[:, 1], 
                       c='green', s=100, zorder=5, label='Опорные точки')
        
        # Подписи точек
        for i, (x, y) in enumerate(self.control_points):
            self.ax.annotate(f'P{i}', (x, y), xytext=(5, 5), 
                            textcoords='offset points', fontsize=8)
        
        # Отрисовка границ сегментов
        for boundary in segment_boundaries:
            if boundary < len(self.control_points):
                x, y = self.control_points[boundary]
                self.ax.scatter([x], [y], c='orange', s=150, marker='s', zorder=6)
        
        # Отрисовка кривой Безье
        if len(bezier_points) > 0:
            self.ax.plot(bezier_points[:, 0], bezier_points[:, 1], 
                        'r-', linewidth=2.5, label='Кривая Безье')
        
        # Подсветка выбранной точки
        if self.selected_point is not None and self.selected_point < len(self.control_points):
            x, y = self.control_points[self.selected_point]
            self.ax.scatter([x], [y], c='yellow', s=200, zorder=7, edgecolors='red', linewidths=2)
        
        # Обновление информационного текста
        n_points = len(self.control_points)
        n_segments = max(0, (n_points - 3) // 3 + 1 if n_points >= 4 else 0)
        info = f"Всего точек: {n_points}\nСегментов Безье: {n_segments}\n"
        info += f"Требуется минимум 4 точки\n"
        info += f"Режим: редактирование (перетаскивание точек)"
        self.info_text.set_text(info)
        
        self.ax.legend()
        self.fig.canvas.draw_idle()
    
    def on_press(self, event):
        """Обработка нажатия кнопки мыши"""
        if event.inaxes != self.ax:
            return
        
        # Поиск ближайшей точки
        distances = np.sqrt((self.control_points[:, 0] - event.xdata)**2 + 
                           (self.control_points[:, 1] - event.ydata)**2)
        min_dist = np.min(distances)
        
        # Если точка близко (радиус 0.5), выбираем её
        if min_dist < 0.5:
            self.selected_point = np.argmin(distances)
            self.dragging = True
            self.update_curve()
    
    def on_release(self, event):
        """Обработка отпускания кнопки мыши"""
        self.dragging = False
        self.selected_point = None
        self.update_curve()
    
    def on_motion(self, event):
        """Обработка движения мыши"""
        if not self.dragging or self.selected_point is None:
            return
        
        if event.inaxes != self.ax:
            return
        
        # Обновление координат выбранной точки
        self.control_points[self.selected_point] = [event.xdata, event.ydata]
        self.update_curve()
    
    def add_point(self, event):
        """Добавление новой точки в конец ломаной"""
        if len(self.control_points) >= 20:
            print("Достигнуто максимальное количество точек (20)")
            return
        
        # Добавляем точку рядом с последней
        if len(self.control_points) > 0:
            last_point = self.control_points[-1]
            new_point = last_point + [0.5, 0.5]
        else:
            new_point = [5, 5]
        
        self.control_points = np.vstack([self.control_points, new_point])
        self.update_curve()
    
    def remove_point(self, event):
        """Удаление последней точки"""
        if len(self.control_points) > 0:
            self.control_points = self.control_points[:-1]
            self.update_curve()
    
    def reset(self, event):
        """Сброс к начальным точкам"""
        self.control_points = np.array([
            [1, 1], [2, 3], [3, 2], [4, 4], [5, 3], [6, 5],
            [7, 4], [8, 6], [9, 5], [10, 7], [11, 6], [12, 8]
        ])
        self.selected_point = None
        self.dragging = False
        self.update_curve()
    
    def random_points(self, event):
        """Генерация случайных точек"""
        np.random.seed(42)  # Для воспроизводимости
        n_points = 12
        x = np.linspace(2, 12, n_points)
        y = 4 + 3 * np.random.randn(n_points)
        y = np.maximum(1, np.minimum(10, y))  # Ограничение по y
        self.control_points = np.column_stack([x, y])
        self.selected_point = None
        self.dragging = False
        self.update_curve()

 

# Основная программа
if __name__ == "__main__":
     
    
     
    
    
    print("\nЗапуск редактора составной кривой Безье...")
    print("Возможности:")
    print("- Перетаскивайте точки мышью для изменения кривой")
    print("- Добавляйте/удаляйте точки с помощью кнопок")
    print("- Точки на границах сегментов отмечены оранжевыми квадратами")
    editor = BezierCurveEditor()