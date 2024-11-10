import csv  # загружаю модул для работы с csv
import xml.etree.ElementTree as ET  # модуль для работы с xml
import time
from collections import defaultdict  # словарь с дефолтными значениями


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


class AddressBook:

    # конструктор
    def __init__(self):
        self.addresses = {}  # пустой словарь
        # хэш мапа из хэш мап = количество домов с таким-то кол-вом этажей в разных городах
        self.city_floors_per_house = defaultdict(lambda: defaultdict(int))

    def add_address(self, to_add):
        unique_address = True
        # если такой адрес уже есть
        if to_add in self.addresses:
            self.addresses[to_add] += 1
            unique_address = False
        else:
            self.addresses[to_add] = 1

        # если адрес уникален, учитываем его в статистике домов с такими-то этажами в городе
        if unique_address is True:
            self.city_floors_per_house[to_add.city][to_add.floor] += 1

    def read_addresses_from_csv(self, file_path):
        # открываем файл на чтение
        with open(file_path, mode="r", encoding="utf-8", newline="") as csvfile:
            # передаем открытый файл в класс для чтения csv файлов
            csv_reader = csv.DictReader(csvfile, delimiter=";")
            # итерайия по строкам
            for row in csv_reader:
                # создаем конструктором новый объект класса адрес
                address = Address(
                    city=row["city"],
                    street=row["street"],
                    street_number=row["house"],
                    floor=row.get("floor"),
                )
                # добавляем адрес в наш объект книга адресов
                self.add_address(address)

    def read_addresses_from_xml(self, file_path):
        tree = ET.parse(file_path)
        root = tree.getroot()
        # чтение из xml
        for item in root.findall("item"):
            city = item.get("city")
            street = item.get("street")
            street_number = item.get("house")
            floor = item.get("floor")
            # создаем конструктором новый объект адрес
            address = Address(
                city=city, street=street, street_number=street_number, floor=floor
            )
            # добавляем в книгу
            self.add_address(address)

    # функция обработки файла
    def process_file(self, file_name):
        # засекаем начало обработки
        start_time = time.perf_counter()
        if file_name.endswith("xml"):
            self.read_addresses_from_xml(file_name)
        elif file_name.endswith("csv"):
            self.read_addresses_from_csv(file_name)
        else:
            print("Неизвестный формат!!!")
            return

        for address in self.addresses:
            if self.addresses[address] > 1:
                # выводим количесво повторений адресов (если кол-во > 1  ---> адрес дублируется в файле)
                print(str(address) + " - " + str(self.addresses[address]) + " вхождений")

        print("")
        # вывод статистики по этажам
        for city in self.city_floors_per_house:
            print(city + ":")
            for floor in self.city_floors_per_house[city]:
                print(str(floor) + " этажный: " + str(self.city_floors_per_house[city][floor]) + " домов")
            print("")

        end_time = time.perf_counter()
        # рассчитываем время обработки
        execution_time = end_time - start_time
        print(f"Время обработки: {execution_time} сек")


def display_menu():
    print("Меню:")
    print("1. Обработка файла по имени")
    print("2. Выход")


if __name__ == "__main__":
    while True:
        display_menu()
        choice = input("Введите ваш выбор: ")
        if choice == "1":
            file_name = input("Введите имя файла: ")
            address_book = AddressBook()
            address_book.process_file(file_name)
        if choice == "2":
            break
