# leti_labs 
этот репозитории будет содержать исходный код лаб! 

// Solution() // ЧИТ ДЛЯ СКОРОСТИ!!!!!!!!
//     {
//         ios_base::sync_with_stdio(false);
//         cin.tie(NULL);
//         cout.tie(NULL);
//     } 

Команды для работы с гитом:

1) git add <file> - добавить измененный или новый файл в стейджинг
2) git add -u . - добавить все измененные файлы в текущей директории в стейджинг
3) git commit -m "Комментарий к коммиту" - сделать новый коммит с комментарием
4) git push - отправить новые коммиты на сервер хитхаб
5) git pull - получить новые коммиты с сервера гитхаб
6) git log --oneline - показать список коммитов построчно
7) git status - показать статус рабочей директории и стейджинга


# CMake команды
1) Создать мэйк файл для дебага (должно выполнятся в директории (папке) build) (делается один раз (при добавлении срр файла в CMakeLists)): 
    cmake -DCMAKE_BUILD_TYPE=Debug ..
2) Собрать проект используя маке файл (должно выполнятся в директории (папке) build)
    make
3) запустить (без дебага)(сначала сделать 2) (должно выполнятся в директории (папке) build)
    ./lab_2 
4) можно исп. дебаг студии код (чтоб дебажить измененный срр-файл надо как п. (2) в билде запустить команду make )

5) как создать папку билд
    mkdir build
    cd build
    выполнить п 1)

