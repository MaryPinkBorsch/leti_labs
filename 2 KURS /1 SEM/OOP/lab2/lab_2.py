import csv  # загружаю модул для работы с csv
import xml.etree.ElementTree as ET  # модуль для работы с xml
import time
from collections import defaultdict  # словарь с дефолтными значениями

from lab2_address_book import AddressBook


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
        elif choice == "2":
            break
        else:
            print("Ошибка!")
