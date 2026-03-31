import tkinter as tk
import math

# --------------------------------------------
# 1. КЛАСС ТОЧКИ (3D)
# --------------------------------------------
class Point3D:
    def __init__(self, x=0, y=0, z=0):
        self.x = x
        self.y = y
        self.z = z

# --------------------------------------------
# 2. КЛАСС ГРАНИ (ПОЛИГОНА)
# --------------------------------------------
class Face:
    def __init__(self, vertices_indices, color='lightblue'):
        self.vertices_indices = vertices_indices  # список номеров вершин
        self.normal = Point3D()  # вектор нормали
        self.color = color  # цвет грани для наглядности

# --------------------------------------------
# 3. МОДЕЛЬ ПИРАМИДЫ (Выпуклое тело)
# --------------------------------------------
def create_pyramid(size=150, height=180):
    """
    Создает четырехугольную пирамиду
    Основание: квадрат со стороной size
    Вершина: находится по центру на высоте height
    """
    s = size / 2  # половина стороны основания
    
    # Вершины пирамиды
    vertices = [
        Point3D(-s, 0, -s),  # 0 - передняя левая точка основания
        Point3D( s, 0, -s),  # 1 - передняя правая точка основания
        Point3D( s, 0,  s),  # 2 - задняя правая точка основания
        Point3D(-s, 0,  s),  # 3 - задняя левая точка основания
        Point3D( 0, height, 0)  # 4 - вершина пирамиды
    ]
    
    # Грани пирамиды (4 треугольных боковых грани + 1 квадратное основание)
    faces = [
        # Боковые грани (треугольники)
        Face([0, 1, 4], color='hotpink'),   # передняя грань
        Face([1, 2, 4], color='green'),     # правая грань
        Face([2, 3, 4], color='orange'),    # задняя грань
        Face([3, 0, 4], color='cyan'),      # левая грань
        # Основание (квадрат) - порядок вершин по часовой стрелке (вид снизу)
        Face([0, 3, 2, 1], color='magenta')  # исправленный порядок вершин для правильной нормали
    ]
    
    return vertices, faces

# --------------------------------------------
# 4. МОДЕЛЬ ТЕТРАЭДРА (Треугольная пирамида)
# --------------------------------------------
def create_tetrahedron(size=150):
    """
    Создает тетраэдр (пирамиду с треугольным основанием)
    """
    # Вершины правильного тетраэдра
    # Высота тетраэдра: sqrt(2/3) * сторона
    h = size * 0.816  # приблизительно sqrt(2/3) * size
    
    vertices = [
        Point3D(0, 0, size),           # 0 - задняя вершина
        Point3D(size, 0, -size/2),     # 1 - правая
        Point3D(-size, 0, -size/2),    # 2 - левая
        Point3D(0, h, 0)               # 3 - верхняя
    ]
    
    faces = [
        Face([0, 1, 3], color='lightpink'),
        Face([1, 2, 3], color='lightgreen'),
        Face([2, 0, 3], color='lightblue'),
        Face([0, 2, 1], color='magenta')  # основание - порядок вершин по часовой стрелке (вид снизу)
    ]
    
    return vertices, faces

# --------------------------------------------
# 5. ВЫЧИСЛЕНИЕ НОРМАЛЕЙ
# --------------------------------------------
def compute_normals(vertices, faces):
    for face in faces:
        # Берем три точки грани (первых три вершины)
        v1 = vertices[face.vertices_indices[0]]
        v2 = vertices[face.vertices_indices[1]]
        v3 = vertices[face.vertices_indices[2]]
        
        # Векторы двух сторон грани
        u = Point3D(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z)
        v = Point3D(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z)
        
        # Векторное произведение
        nx = u.y * v.z - u.z * v.y
        ny = u.z * v.x - u.x * v.z
        nz = u.x * v.y - u.y * v.x
        
        # Нормализация
        length = math.sqrt(nx*nx + ny*ny + nz*nz)
        if length != 0:
            face.normal.x = nx / length
            face.normal.y = ny / length
            face.normal.z = nz / length

# --------------------------------------------
# 6. ПРОЕКЦИЯ 3D -> 2D
# --------------------------------------------
def project_3d_to_2d(point_3d, camera_distance, center_x, center_y):
    # Простая перспектива
    scale = 400
    z_offset = 200
    
    if (point_3d.z + camera_distance) != 0:
        factor = scale / (point_3d.z + camera_distance + z_offset)
    else:
        factor = scale
    
    x_screen = center_x + point_3d.x * factor
    y_screen = center_y - point_3d.y * factor
    
    return (x_screen, y_screen)

# --------------------------------------------
# 7. ОСНОВНОЙ КЛАСС ПРИЛОЖЕНИЯ
# --------------------------------------------
class VisibilityApp:
    def __init__(self, root, shape_type="pyramid"):
        self.root = root
        self.root.title(f"Алгоритм выявления видимых граней - {shape_type}")
        
        # Настройки холста
        self.canvas = tk.Canvas(root, width=800, height=600, bg='white')
        self.canvas.pack()
        
        # Создаем фигуру (по умолчанию пирамиду)
        if shape_type == "tetrahedron":
            self.vertices, self.faces = create_tetrahedron(140)
        else:
            self.vertices, self.faces = create_pyramid(140, 160)
        
        # Вычисляем нормали
        compute_normals(self.vertices, self.faces)
        
        # Параметры камеры и вращения
        self.camera_distance = 300
        self.angle_x = 25   # Вращение по X
        self.angle_y = 35   # Вращение по Y
        
        # Для вращения мышью
        self.last_x = 0
        self.last_y = 0
        
        # Связываем события мыши
        self.canvas.bind("<Button-1>", self.start_rotate)
        self.canvas.bind("<B1-Motion>", self.do_rotate)
        
        # Добавляем кнопки переключения фигур
        self.add_controls()
        
        # Запускаем отрисовку
        self.draw()
    
    def add_controls(self):
        """Добавляет кнопки для переключения между фигурами"""
        control_frame = tk.Frame(self.root)
        control_frame.pack()
        
        btn_pyramid = tk.Button(control_frame, text="Пирамида", 
                                command=lambda: self.change_shape("pyramid"))
        btn_pyramid.pack(side=tk.LEFT, padx=5, pady=5)
        
        btn_tetra = tk.Button(control_frame, text="Тетраэдр", 
                             command=lambda: self.change_shape("tetrahedron"))
        btn_tetra.pack(side=tk.LEFT, padx=5, pady=5)
        
        btn_reset = tk.Button(control_frame, text="Сбросить вращение", 
                             command=self.reset_rotation)
        btn_reset.pack(side=tk.LEFT, padx=5, pady=5)
    
    def change_shape(self, shape_type):
        """Переключает между разными фигурами"""
        if shape_type == "tetrahedron":
            self.vertices, self.faces = create_tetrahedron(140)
        else:
            self.vertices, self.faces = create_pyramid(140, 160)
        
        compute_normals(self.vertices, self.faces)
        self.root.title(f"Алгоритм выявления видимых граней - {shape_type}")
        self.draw()
    
    def reset_rotation(self):
        """Сбрасывает углы поворота"""
        self.angle_x = 25
        self.angle_y = 35
        self.draw()
    
    def start_rotate(self, event):
        self.last_x = event.x
        self.last_y = event.y
    
    def do_rotate(self, event):
        dx = event.x - self.last_x
        dy = event.y - self.last_y
        self.angle_y += dx * 0.5
        self.angle_x += dy * 0.5
        self.last_x = event.x
        self.last_y = event.y
        self.draw()
    
    def rotate_point(self, point):
        """Поворачивает точку в пространстве"""
        rad_x = math.radians(self.angle_x)
        rad_y = math.radians(self.angle_y)
        
        x, y, z = point.x, point.y, point.z
        
        # Вращение вокруг оси Y
        cos_y = math.cos(rad_y)
        sin_y = math.sin(rad_y)
        x1 = x * cos_y + z * sin_y
        z1 = -x * sin_y + z * cos_y
        y1 = y
        
        # Вращение вокруг оси X
        cos_x = math.cos(rad_x)
        sin_x = math.sin(rad_x)
        y2 = y1 * cos_x - z1 * sin_x
        z2 = y1 * sin_x + z1 * cos_x
        x2 = x1
        
        return Point3D(x2, y2, z2)
    
    def get_visible_faces(self):
        """Алгоритм выявления видимых граней"""
        visible = []
        
        for face in self.faces:
            # Поворачиваем нормаль грани
            norm = self.rotate_point(face.normal)
            
            # Условие видимости: нормаль направлена к зрителю (положительная Z)
            # Для пирамиды это работает отлично, так как все грани выпуклые
            if norm.z > 0:
                visible.append(face)
        
        return visible
    
    def draw(self):
        """Отрисовка сцены"""
        self.canvas.delete("all")
        
        # Поворачиваем все вершины
        rotated_vertices = [self.rotate_point(v) for v in self.vertices]
        
        # Определяем видимые грани
        visible_faces = self.get_visible_faces()
        
        center_x = 400
        center_y = 300
        
        # Сначала рисуем грани (с заливкой)
        for face in visible_faces:
            points_2d = []
            for idx in face.vertices_indices:
                v = rotated_vertices[idx]
                x_screen, y_screen = project_3d_to_2d(v, self.camera_distance, center_x, center_y)
                points_2d.append((x_screen, y_screen))
            
            # Рисуем грань с заливкой
            self.canvas.create_polygon(points_2d, outline='black', fill=face.color, width=2)
        
        # Затем рисуем ребра (чтобы они были поверх заливки)
        edges_drawn = set()
        for face in visible_faces:
            indices = face.vertices_indices
            for i in range(len(indices)):
                i1 = indices[i]
                i2 = indices[(i+1) % len(indices)]
                edge = tuple(sorted((i1, i2)))
                
                if edge not in edges_drawn:
                    edges_drawn.add(edge)
                    p1 = rotated_vertices[i1]
                    p2 = rotated_vertices[i2]
                    p1_2d = project_3d_to_2d(p1, self.camera_distance, center_x, center_y)
                    p2_2d = project_3d_to_2d(p2, self.camera_distance, center_x, center_y)
                    self.canvas.create_line(p1_2d[0], p1_2d[1], p2_2d[0], p2_2d[1], fill='black', width=2)
        
        # Добавляем информацию
        info_text = f"Видимых граней: {len(visible_faces)} из {len(self.faces)}"
        self.canvas.create_text(400, 30, text=info_text, fill="hotpink", font=("Arial", 12, "bold"))
        self.canvas.create_text(400, 550, text="Нажмите и тяните мышью для вращения", fill="hotpink", font=("Arial", 10))

# --------------------------------------------
# ЗАПУСК ПРОГРАММЫ
# --------------------------------------------
if __name__ == "__main__":
    root = tk.Tk()
    
    # Можно выбрать любую фигуру при запуске:
    # "pyramid" - четырехугольная пирамида
    # "tetrahedron" - треугольная пирамида (тетраэдр)
    app = VisibilityApp(root, shape_type="pyramid")
    
    root.mainloop()