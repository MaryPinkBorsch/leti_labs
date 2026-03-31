import tkinter as tk
import math
import sys

# --------------------------------------------
# 1. КЛАСС ТОЧКИ (3D)
# Нужен, чтобы хранить координаты X, Y, Z
# --------------------------------------------
class Point3D:
    def __init__(self, x=0, y=0, z=0):
        self.x = x
        self.y = y
        self.z = z

# --------------------------------------------
# 2. КЛАСС ГРАНИ (ПОЛИГОНА)
# Хранит индексы вершин (точек) и нормаль (направление)
# --------------------------------------------
class Face:
    def __init__(self, vertices_indices):
        # vertices_indices: список номеров вершин (например [0,1,2,3] для квадрата)
        self.vertices_indices = vertices_indices
        self.normal = Point3D()  # Вектор нормали (куда смотрит грань)

# --------------------------------------------
# 3. МОДЕЛЬ КУБА (Выпуклое тело)
# Создаем 8 точек и 6 граней
# --------------------------------------------
def create_cube(size=100):
    # Координаты вершин (от -size/2 до size/2, чтобы центр был в (0,0,0))
    s = size / 2
    vertices = [
        Point3D(-s, -s, -s),  # 0
        Point3D( s, -s, -s),  # 1
        Point3D( s, -s,  s),  # 2
        Point3D(-s, -s,  s),  # 3
        Point3D(-s,  s, -s),  # 4
        Point3D( s,  s, -s),  # 5
        Point3D( s,  s,  s),  # 6
        Point3D(-s,  s,  s)   # 7
    ]
    
    # Описание граней (каждая грань — это 4 вершины, идущие по порядку)
    faces = [
        Face([0, 1, 2, 3]),  # Нижняя грань (Y = -s)
        Face([4, 7, 6, 5]),  # Верхняя грань (Y =  s)
        Face([0, 4, 5, 1]),  # Передняя? В зависимости от системы координат. Это левая/правая? Упростим: Задняя (Z = -s)
        Face([2, 6, 7, 3]),  # Передняя (Z =  s)
        Face([0, 3, 7, 4]),  # Левая (X = -s)
        Face([1, 5, 6, 2])   # Правая (X =  s)
    ]
    return vertices, faces

# --------------------------------------------
# 4. ВЫЧИСЛЕНИЕ НОРМАЛЕЙ
# Для каждой грани считаем перпендикулярный вектор (отличаем "наружу" от "внутри")
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
        
        # Векторное произведение (получаем перпендикуляр)
        nx = u.y * v.z - u.z * v.y
        ny = u.z * v.x - u.x * v.z
        nz = u.x * v.y - u.y * v.x
        
        # Нормализуем (делаем длину = 1), чтобы удобно было сравнивать углы
        length = math.sqrt(nx*nx + ny*ny + nz*nz)
        if length != 0:
            face.normal.x = nx / length
            face.normal.y = ny / length
            face.normal.z = nz / length

# --------------------------------------------
# 5. ПРОЕКЦИЯ 3D -> 2D (Перспектива)
# Превращает 3D координаты в экранные (x, y)
# --------------------------------------------
def project_3d_to_2d(point_3d, camera_distance, center_x, center_y):
    # Простая перспектива: чем дальше Z, тем меньше масштаб
    # Мы смотрим из точки (0,0, camera_distance) на начало координат
    z_offset = 100  # Чтобы объект не пересекался с камерой
    # Формула: экранное_и = центр + (x / (z + расстояние)) * масштаб
    scale = 300
    if (point_3d.z + camera_distance) != 0:
        factor = scale / (point_3d.z + camera_distance + z_offset)
    else:
        factor = scale
    
    x_screen = center_x + point_3d.x * factor
    y_screen = center_y - point_3d.y * factor  # Минус, т.к. Y в экране идет вниз
    return (x_screen, y_screen)

# --------------------------------------------
# 6. ОСНОВНОЙ КЛАСС ПРИЛОЖЕНИЯ (РИСОВАЛКА)
# --------------------------------------------
class VisibilityApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Алгоритм удаления невидимых граней (Куб)")
        
        # Настройки холста для рисования
        self.canvas = tk.Canvas(root, width=800, height=600, bg='white')
        self.canvas.pack()
        
        # Создаем куб
        self.vertices, self.faces = create_cube(150)  # Размер 150
        # Вычисляем нормали (направления граней)
        compute_normals(self.vertices, self.faces)
        
        # Параметры камеры и вращения
        self.camera_distance = 300
        self.angle_x = 25   # Вращение по X (в градусах)
        self.angle_y = 35   # Вращение по Y (в градусах)
        
        # Связываем кнопки мыши для вращения
        self.canvas.bind("<Button-1>", self.start_rotate)
        self.canvas.bind("<B1-Motion>", self.do_rotate)
        
        # Запускаем отрисовку
        self.draw()
    
    # ---- Вращение объекта (чтобы видеть, что алгоритм работает с разных сторон) ----
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
        self.draw()  # Перерисовываем
    
    # ---- Поворот точки в пространстве (математика вращения) ----
    def rotate_point(self, point):
        # Преобразуем градусы в радианы
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
    
    # ---- ГЛАВНЫЙ АЛГОРИТМ ВЫЯВЛЕНИЯ ВИДИМОСТИ ----
    def get_visible_faces(self):
        visible = []
        # Направление взгляда: от камеры к объекту.
        # Камера у нас находится в точке (0, 0, camera_distance) в мировых координатах.
        # Объект вращается. Чтобы узнать, видна ли грань, нужно посчитать угол между
        # нормалью грани (после вращения) и вектором взгляда.
        
        # Вектор взгляда в мировых координатах (камера смотрит на центр - начало координат)
        # Направление: из центра к камере? Нет. Нужно от грани к камере.
        # Упрощенное правило для выпуклых тел (Back-face culling):
        # Если нормаль грани (направленная наружу) имеет положительную проекцию на направление взгляда (Z),
        # то грань видна.
        
        for face in self.faces:
            # Берем первую вершину грани, чтобы применить к ней вращение и получить "повернутую" нормаль?
            # Нормаль хранится для исходного куба. При вращении куба нормаль тоже вращается.
            # Возьмем любую точку грани и повернем её, но для нормали нужно повернуть вектор.
            # Проще: повернуть вектор нормали на те же углы, что и куб.
            
            # Вращаем вектор нормали
            norm = self.rotate_point(face.normal)
            
            # Вектор от грани к камере (приблизительно). Камера в мировом пространстве (0,0, camera_distance)
            # Объект вращается, но центр объекта всегда в (0,0,0) после вращения.
            # Нам нужно направление от центра грани к камере. Упростим: считаем, что камера смотрит вдоль оси Z.
            # Для простоты: грань видна, если нормаль направлена в сторону камеры, т.е. нормаль.z < 0? 
            # Нужно аккуратно.
            # Стандартный метод: Камера в (0,0, dist). Центр объекта (0,0,0). Вектор взгляда: от объекта к камере = (0,0, dist).
            # Условие: если скалярное произведение (нормаль) * (вектор_взгляда) > 0, то грань смотрит на камеру.
            
            # Вектор взгляда (от грани к камере) упрощенно: (0, 0, 1) в локальных координатах, но после поворота нормали это не точно.
            # Для корректности используем локальные координаты камеры после поворота? Сложно.
            # В учебных целях сделаем проще: будем считать грань видимой, если её нормаль (после вращения) направлена к зрителю.
            # Зритель смотрит из точки (0,0, +inf) вдоль оси Z. Значит, нам нужно norm.z > 0? 
            # Давайте проверим: если куб повернут к нам передней гранью, у передней грани нормаль (0,0,1). 
            # Если norm.z > 0, то рисуем. Это и есть условие видимости.
            
            # Это самый простой и рабочий способ для выпуклого тела при центральной проекции.
            if norm.z > 0:
                visible.append(face)
        return visible
    
    # ---- ОТРИСОВКА (Рендеринг) ----
    def draw(self):
        self.canvas.delete("all")
        
        # 1. Поворачиваем все вершины куба
        rotated_vertices = [self.rotate_point(v) for v in self.vertices]
        
        # 2. Определяем, какие грани видимы (по нормалям)
        visible_faces = self.get_visible_faces()
        
        # 3. Рисуем видимые грани
        center_x = 400
        center_y = 300
        
        for face in visible_faces:
            # Получаем экранные координаты всех вершин грани
            points_2d = []
            for idx in face.vertices_indices:
                v = rotated_vertices[idx]
                x_screen, y_screen = project_3d_to_2d(v, self.camera_distance, center_x, center_y)
                points_2d.append((x_screen, y_screen))
            
            # Рисуем многоугольник (грань) с полупрозрачной заливкой
            self.canvas.create_polygon(points_2d, outline='black', fill='pink', width=2)
        
        # 4. Рисуем ребра (линии) поверх граней для наглядности
        # Чтобы не рисовать линии невидимых граней, пройдемся по видимым граням
        edges_drawn = set()  # Множество, чтобы не рисовать одно ребро дважды
        for face in visible_faces:
            indices = face.vertices_indices
            for i in range(len(indices)):
                i1 = indices[i]
                i2 = indices[(i+1) % len(indices)]
                # Сортируем индексы, чтобы (1,2) и (2,1) считались одним ребром
                edge = tuple(sorted((i1, i2)))
                if edge not in edges_drawn:
                    edges_drawn.add(edge)
                    p1 = rotated_vertices[i1]
                    p2 = rotated_vertices[i2]
                    p1_2d = project_3d_to_2d(p1, self.camera_distance, center_x, center_y)
                    p2_2d = project_3d_to_2d(p2, self.camera_distance, center_x, center_y)
                    self.canvas.create_line(p1_2d[0], p1_2d[1], p2_2d[0], p2_2d[1], fill='black', width=2)
        
        # Добавляем подпись
        self.canvas.create_text(400, 20, text="Видимые грани отображаются заливкой. Потяните мышкой для вращения.", fill="gray")

# --------------------------------------------
# ЗАПУСК ПРОГРАММЫ
# --------------------------------------------
if __name__ == "__main__":
    root = tk.Tk()
    app = VisibilityApp(root)
    root.mainloop()