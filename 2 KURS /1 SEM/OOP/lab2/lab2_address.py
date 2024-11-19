class Address:

    # конструктор
    def __init__(self, city, street, street_number, floor=None):
        self.city = city
        self.street = street
        self.street_number = street_number
        self.floor = floor

    # строковое описание объекта
    def __str__(self):
        floor_info = f", Этаж: {self.floor}" if self.floor else ""
        return f"Адрес: {self.street} {self.street_number},{self.city}{floor_info}"

    # метод для сравнения объектов
    def __eq__(self, other):
        if not isinstance(other, Address):
            return False
        # возвращает True если объекты одинаковы
        return (
            self.city == other.city
            and self.street == other.street
            and self.street_number == other.street_number
            and self.floor == other.floor
        )

    # хэширование объекта класса адрес (нужно для того, чтобы объект мог быть ключем в словаре / мапе)
    def __hash__(self):
        return hash((self.city, self.street, self.street_number, self.floor))
