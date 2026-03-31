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
        self.intensity = 1.0  # интенсивность освещения

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
# 7. КЛАСС ДЛЯ УПРАВЛЕНИЯ ОСВЕЩЕНИЕМ
# --------------------------------------------
class Light:
    def __init__(self, x=200, y=200, z=200):
        self.position = Point3D(x, y, z)  # позиция источника света
        self.intensity = 1.0  # интенсивность источника
    
    def compute_intensity(self, point, normal):
        """
        Вычисляет интенсивность освещения в точке
        Без учета диффузной и зеркальной составляющих (только падающий свет)
        """
        # Вектор от точки к источнику света
        light_dir = Point3D(
            self.position.x - point.x,
            self.position.y - point.y,
            self.position.z - point.z
        )
        
        # Нормализуем вектор направления света
        length = math.sqrt(light_dir.x**2 + light_dir.y**2 + light_dir.z**2)
        if length != 0:
            light_dir.x /= length
            light_dir.y /= length
            light_dir.z /= length
        
        # Вычисляем косинус угла между нормалью и направлением на источник
        cos_angle = (normal.x * light_dir.x + normal.y * light_dir.y + normal.z * light_dir.z)
        
        # Интенсивность пропорциональна косинусу угла (закон Ламберта)
        # Ограничиваем значения от 0 до 1
        intensity = max(0.0, min(1.0, cos_angle * self.intensity))
        
        return intensity

# --------------------------------------------
# 8. ОСНОВНОЙ КЛАСС ПРИЛОЖЕНИЯ
# --------------------------------------------
class VisibilityApp:
    def __init__(self, root, shape_type="pyramid"):
        self.root = root
        self.root.title(f"3D Модель с освещением - {shape_type}")
        
        # Настройки холста
        self.canvas = tk.Canvas(root, width=800, height=600, bg='black')
        self.canvas.pack()
        
        # Создаем фигуру (по умолчанию пирамиду)
        if shape_type == "tetrahedron":
            self.vertices, self.faces = create_tetrahedron(140)
        else:
            self.vertices, self.faces = create_pyramid(140, 160)
        
        # Вычисляем нормали
        compute_normals(self.vertices, self.faces)
        
        # Создаем источник света
        self.light = Light(x=200, y=300, z=200)
        
        # Параметры камеры и вращения
        self.camera_distance = 300
        self.angle_x = 25   # Вращение по X
        self.angle_y = 35   # Вращение по Y
        
        # Для вращения мышью
        self.last_x = 0
        self.last_y = 0
        
        # Флаги для управления
        self.show_shadows = True  # показывать тени
        self.auto_rotate = False  # автоматическое вращение
        
        # Связываем события мыши
        self.canvas.bind("<Button-1>", self.start_rotate)
        self.canvas.bind("<B1-Motion>", self.do_rotate)
        self.canvas.bind("<Button-3>", self.start_move_light)
        self.canvas.bind("<B3-Motion>", self.do_move_light)
        
        # Добавляем элементы управления
        self.add_controls()
        
        # Запускаем анимацию
        self.animate()
    
    def add_controls(self):
        """Добавляет элементы управления"""
        control_frame = tk.Frame(self.root, bg='gray')
        control_frame.pack(fill=tk.X)
        
        # Кнопки выбора фигур
        btn_pyramid = tk.Button(control_frame, text="Пирамида", 
                                command=lambda: self.change_shape("pyramid"))
        btn_pyramid.pack(side=tk.LEFT, padx=5, pady=5)
        
        btn_tetra = tk.Button(control_frame, text="Тетраэдр", 
                             command=lambda: self.change_shape("tetrahedron"))
        btn_tetra.pack(side=tk.LEFT, padx=5, pady=5)
        
        btn_reset = tk.Button(control_frame, text="Сбросить вращение", 
                             command=self.reset_rotation)
        btn_reset.pack(side=tk.LEFT, padx=5, pady=5)
        
        # Переключатель теней
        self.shadow_var = tk.BooleanVar(value=True)
        chk_shadows = tk.Checkbutton(control_frame, text="Показывать тени", 
                                     variable=self.shadow_var, 
                                     command=self.toggle_shadows)
        chk_shadows.pack(side=tk.LEFT, padx=5, pady=5)
        
        # Автоматическое вращение
        self.rotate_var = tk.BooleanVar(value=False)
        chk_rotate = tk.Checkbutton(control_frame, text="Автовращение", 
                                   variable=self.rotate_var, 
                                   command=self.toggle_auto_rotate)
        chk_rotate.pack(side=tk.LEFT, padx=5, pady=5)
        
        # Информация об источнике света
        self.light_info = tk.Label(control_frame, text="Свет: ПКМ для перемещения", 
                                   bg='gray', fg='white')
        self.light_info.pack(side=tk.RIGHT, padx=5, pady=5)
    
    def toggle_shadows(self):
        """Включает/выключает отображение теней"""
        self.show_shadows = self.shadow_var.get()
    
    def toggle_auto_rotate(self):
        """Включает/выключает автоматическое вращение"""
        self.auto_rotate = self.rotate_var.get()
    
    def change_shape(self, shape_type):
        """Переключает между разными фигурами"""
        if shape_type == "tetrahedron":
            self.vertices, self.faces = create_tetrahedron(140)
        else:
            self.vertices, self.faces = create_pyramid(140, 160)
        
        compute_normals(self.vertices, self.faces)
        self.root.title(f"3D Модель с освещением - {shape_type}")
    
    def reset_rotation(self):
        """Сбрасывает углы поворота"""
        self.angle_x = 25
        self.angle_y = 35
    
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
    
    def start_move_light(self, event):
        self.last_x = event.x
        self.last_y = event.y
    
    def do_move_light(self, event):
        """Перемещает источник света с помощью мыши"""
        dx = (event.x - self.last_x) * 2
        dy = (event.y - self.last_y) * 2
        
        self.light.position.x += dx
        self.light.position.y -= dy
        
        # Ограничиваем перемещение
        self.light.position.x = max(-300, min(300, self.light.position.x))
        self.light.position.y = max(-300, min(500, self.light.position.y))
        
        self.last_x = event.x
        self.last_y = event.y
    
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
    
    def get_visible_faces(self, rotated_vertices):
        """Алгоритм выявления видимых граней с учетом освещения"""
        visible = []
        
        for face in self.faces:
            # Вычисляем центр грани
            center = Point3D()
            for idx in face.vertices_indices:
                center.x += rotated_vertices[idx].x
                center.y += rotated_vertices[idx].y
                center.z += rotated_vertices[idx].z
            center.x /= len(face.vertices_indices)
            center.y /= len(face.vertices_indices)
            center.z /= len(face.vertices_indices)
            
            # Поворачиваем нормаль грани
            norm = self.rotate_point(face.normal)
            
            # Условие видимости: нормаль направлена к зрителю (положительная Z)
            if norm.z > 0:
                # Вычисляем интенсивность освещения для грани
                # Для источника света нужно использовать оригинальные координаты (до вращения камеры)
                # Но для простоты используем повернутые координаты
                light_dir = Point3D(
                    self.light.position.x - center.x,
                    self.light.position.y - center.y,
                    self.light.position.z - center.z
                )
                
                # Нормализуем вектор направления света
                length = math.sqrt(light_dir.x**2 + light_dir.y**2 + light_dir.z**2)
                if length != 0:
                    light_dir.x /= length
                    light_dir.y /= length
                    light_dir.z /= length
                
                # Вычисляем косинус угла между нормалью и направлением на источник
                cos_angle = norm.x * light_dir.x + norm.y * light_dir.y + norm.z * light_dir.z
                
                # Интенсивность освещения (только падающий свет)
                intensity = max(0.2, min(1.0, cos_angle))  # Минимальная интенсивность 0.2 для видимости теневых граней
                
                face.intensity = intensity
                visible.append(face)
        
        return visible
    
    def get_color_with_intensity(self, base_color, intensity):
        """Возвращает цвет с учетом интенсивности освещения"""
        # Преобразуем имя цвета в RGB
        colors = {
            'hotpink': (255, 105, 180),
            'green': (0, 255, 0),
            'orange': (255, 165, 0),
            'cyan': (0, 255, 255),
            'magenta': (255, 0, 255),
            'lightpink': (255, 182, 193),
            'lightgreen': (144, 238, 144),
            'lightblue': (173, 216, 230)
        }
        
        rgb = colors.get(base_color, (200, 200, 200))
        
        # Применяем интенсивность
        r = int(rgb[0] * intensity)
        g = int(rgb[1] * intensity)
        b = int(rgb[2] * intensity)
        
        # Ограничиваем значения
        r = max(0, min(255, r))
        g = max(0, min(255, g))
        b = max(0, min(255, b))
        
        return f'#{r:02x}{g:02x}{b:02x}'
    
    def draw_shadow(self, rotated_vertices, face, center_x, center_y):
        """Рисует тень от грани на плоскости Y=0"""
        shadow_points = []
        
        # Для каждой вершины грани вычисляем проекцию тени
        for idx in face.vertices_indices:
            v = rotated_vertices[idx]
            
            # Направление от источника света к точке
            light_dir = Point3D(
                v.x - self.light.position.x,
                v.y - self.light.position.y,
                v.z - self.light.position.z
            )
            
            # Находим пересечение луча с плоскостью Y=0
            # Параметрическое уравнение луча: P = light_pos + t * light_dir
            # Решаем для Y = 0: light_pos.y + t * light_dir.y = 0
            if light_dir.y != 0:
                t = -self.light.position.y / light_dir.y
                if t > 0:  # Тень только если луч направлен к плоскости
                    shadow_x = self.light.position.x + t * light_dir.x
                    shadow_z = self.light.position.z + t * light_dir.z
                    
                    # Проецируем точку тени на экран
                    shadow_point = Point3D(shadow_x, 0, shadow_z)
                    x_screen, y_screen = project_3d_to_2d(shadow_point, self.camera_distance, center_x, center_y)
                    shadow_points.append((x_screen, y_screen))
        
        # Рисуем тень, если есть точки
        if len(shadow_points) >= 3:
            self.canvas.create_polygon(shadow_points, fill='gray', outline='', stipple='gray50')
    
    def animate(self):
        """Анимация с автоматическим вращением"""
        if self.auto_rotate:
            self.angle_y += 1
            if self.angle_y >= 360:
                self.angle_y -= 360
        
        self.draw()
        
        # Запускаем следующую итерацию
        self.root.after(50, self.animate)
    
    def draw(self):
        """Отрисовка сцены с освещением и тенями"""
        self.canvas.delete("all")
        
        # Поворачиваем все вершины
        rotated_vertices = [self.rotate_point(v) for v in self.vertices]
        
        # Определяем видимые грани с учетом освещения
        visible_faces = self.get_visible_faces(rotated_vertices)
        
        center_x = 400
        center_y = 300
        
        # Сначала рисуем тени (если включены)
        if self.show_shadows:
            for face in self.faces:
                self.draw_shadow(rotated_vertices, face, center_x, center_y)
        
        # Рисуем грани с учетом освещения
        for face in visible_faces:
            points_2d = []
            for idx in face.vertices_indices:
                v = rotated_vertices[idx]
                x_screen, y_screen = project_3d_to_2d(v, self.camera_distance, center_x, center_y)
                points_2d.append((x_screen, y_screen))
            
            # Получаем цвет с учетом интенсивности освещения
            color = self.get_color_with_intensity(face.color, face.intensity)
            
            # Рисуем грань с заливкой
            self.canvas.create_polygon(points_2d, outline='white', fill=color, width=1)
        
        # Рисуем ребра
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
                    self.canvas.create_line(p1_2d[0], p1_2d[1], p2_2d[0], p2_2d[1], fill='white', width=1)
        
        # Рисуем источник света
        light_screen = project_3d_to_2d(self.light.position, self.camera_distance, center_x, center_y)
        self.canvas.create_oval(light_screen[0]-8, light_screen[1]-8, 
                                light_screen[0]+8, light_screen[1]+8, 
                                fill='yellow', outline='orange', width=2)
        
        # Добавляем информацию
        info_text = f"Видимых граней: {len(visible_faces)} из {len(self.faces)} | "
        info_text += f"Источник света: ({self.light.position.x:.0f}, {self.light.position.y:.0f}, {self.light.position.z:.0f})"
        self.canvas.create_text(400, 30, text=info_text, fill="yellow", font=("Arial", 10, "bold"))
        
        self.canvas.create_text(400, 580, text="ЛКМ - вращение | ПКМ - перемещение источника света", 
                                fill="yellow", font=("Arial", 9))

# --------------------------------------------
# ЗАПУСК ПРОГРАММЫ
# --------------------------------------------
if __name__ == "__main__":
    root = tk.Tk()
    app = VisibilityApp(root, shape_type="pyramid")
    root.mainloop()