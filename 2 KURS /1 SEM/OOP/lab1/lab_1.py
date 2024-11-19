import requests
import webbrowser as wb  # импортирую модуль для работы с браузером

class menuu:
    # конструктор
               
        
    def display_menu():
        print("Меню:")
        print("1. Поиск")
        print("2. Просмотреть результаты поиска")
        print("3. Показать результат по номеру")
        print("4. Выход")


    def print_results(to_search, results):
        # вывод строки и завпрашиваемого слова
        print(f"Результаты поиска по '{to_search}':\n")
        # "двойной" цикл по результатам (массив с результатами нумеруется с
        # 1цы с помощью enumerate и по нему проходится for)
        for i, result in enumerate(results, start=1):
            # каждый элемент массива results это map/dictionary  и берем из него значение по ключу title
            title = result["title"]
            # вывод номера и одного из 10 результатов поиска
            print(f"{i}. {title}")


def main():
    results = None  # пустота
    m = menuu
    while True:
        m.display_menu()
        # считываем выбор
        choice = input("Введите ваш выбор (1-4): ")

        if choice == "1":

            to_search = input("Строка поиска по википедии:")  # наш запрос

            # Определение API URL и параметров (создание запроса к АПИ Википедии)
            # https://www.mediawiki.org/wiki/Manual:Api.php/ru

            url = "https://ru.wikipedia.org/w/api.php"
            # список параметров для АПИ Вики
            params = {
                "action": "query",
                "list": "search",
                "format": "json",
                "utf8": "",
                "srsearch": to_search,
            }

            # Выполнение запроса к АПИ Вики
            response = requests.get(url, params=params)

            # Если запрос успешно выполнился
            if response.status_code == 200:
                data = response.json()  # ответ в формате json

                # Проверка на наличие результата
                if "query" in data and "search" in data["query"]:
                    results = data["query"]["search"]
                    m.print_results(to_search, results)  # выводим результаты в консоль
                else:
                    print("Ничего не найдено.")
            else:
                print("Ошибка выполнения запроса к АПИ википедии.")

        # вывод результатов запроса
        elif choice == "2":
            m.print_results(to_search, results)
        # открыть результат по номеру
        elif choice == "3":
            result_num = input("Номер результата: ")
            # айди страницы в Вики для выбранного результата:
            page_id = results[int(result_num) - 1]["pageid"]
            # итоговая ссылка на страницу Вики с результатом
            result_url = "https://ru.wikipedia.org/?curid=" + str(page_id)
            # открываем страницу в браузере
            wb.open_new_tab(result_url)
        elif choice == "4":
            break
        else:
            print("Неправильный ввод, повторите попытку.")


# место вызова функции мейн при запуске программы
if __name__ == "__main__":
    main()
