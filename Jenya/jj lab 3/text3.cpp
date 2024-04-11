#include "text3.h"
#include <string>
#include <cstring>

using namespace std;

void Text::print(std::ofstream &res)
{
    ListNode *cur = head;

    while (cur != nullptr) //(cur != nullptr)
    {
        cur->strochka.print(res);

        res << " ----> " << endl;

        cur = cur->next;
    }
    res << endl
        << " NULL (конец списка)" << endl;
}

bool Text::read_file(std::string filename, std::ofstream &res)
{
    ifstream input;
    input.open(filename, std::ios_base::in);
    num_stroki = 0;

    // !! TODO proverka eof i return false !!!!!!
    if (input.eof())
    {
        cout << "ФАЙЛ ПУСТ!" << endl;
        res << "ФАЙЛ ПУСТ!" << endl;

        return 0;
    }

    if (!input.eof()) //! input.eof()
    {
        head->addMemory(head);
    }
    ListNode *cur = head;
    ListNode *prev = nullptr;
    // пока в стриме есть что читать и в тексте есть место, считываем строки
    while (!input.eof() && num_stroki < M) // && cur != nullptr ?
    {
        // пытаемся прочесть строку в cur
        if (cur->strochka.read_StrM(input, res))
        {
            // если получилось, увеличиваем количество строк и
            // выделяем память под следующий элемент списка, одновременно
            // запоминаем текущий в prev
            ++num_stroki;
            cur->addNext(cur);
            prev = cur;
            cur = cur->next;
        }
    }
    // иначе если  не получилось считать у нас хвост в котором ничего нет, надо его удалить, а
    // предыдущий элемент сделать новым хвостом (-> next = nullptr)
    {
        delete cur;
        if (prev)
            prev->next = nullptr;
        else
            head = nullptr; // если предыдущего  не было значит только что удалили голову, запишем в голову ноль
    }

    if (head == nullptr)
    {
        cout << " Файл пуст! " << endl;
        res << " Файл пуст! " << endl;
    }

    if (!input.eof() && num_stroki == M)
    {
        cout << "ОБРАБОТКА НЕВОЗМОЖНА, слишком много строк!" << endl;
        res << "ОБРАБОТКА НЕВОЗМОЖНА, слишком много строк!" << endl;
        return false;
    }
    return true;
}

void Text::clear(std::ofstream &res)
{
    ListNode *cur = head;
    if (cur == nullptr)
    {
        cout << " Список пуст " << endl;
        res << " Список пуст " << endl;
        return;
    }
    ListNode *tmp = nullptr;
    while (cur != nullptr)
    {
        tmp = cur->next;
        delete cur;
        cur = tmp;
    }

    std::cout << " Список был удален. " << endl;
    res << " Список был удален. " << endl;
}

// функция вставляет элементы после to
void Text::insert_after(ListNode *to, StrM & toInsert, int Kolichestvo) 
{
    if(Kolichestvo==0)
        return; // если количество элементов для вставки ноль - выходим сразу
    ListNode* firstNew = nullptr;
    ListNode* cur = nullptr;
    for (int i = 0 ; i < Kolichestvo; ++i) 
    {
        // получаем память
        if (i == 0) 
        {
            firstNew = new ListNode(); // запоминаем первый новый элемент
            cur = firstNew;
        }
        else
        {
            cur->addMemory(cur->next); // получаем место под следующий
            cur = cur->next;
        }
        // заполняем строку
        cur->strochka = toInsert; // копируем строчку
    }
    ListNode* oldNext = to->next; // сохраняем некст элемента после которого вставлять
    to->next = firstNew; // меняем некст элемента после которого вставлять на первый новый
    cur->next = oldNext; // у последнего нового переставляем некст на сохраненный
}

void Text::process(string filename, ofstream &res)
{
    cout << endl
         << " Началась обработка файла " << filename << endl
         << endl;
    res << endl
        << " Началась обработка файла " << filename << endl
        << endl;
    if (!read_file(filename, res))
    {
        res << "Oshibka reading" << endl;
        cout << "Oshibka reading" << endl;
    }
    else
    {
        cout << endl
             << " Исходный текст: " << endl;
        res << endl
            << " Исходный текст: " << endl;
        print(res);

        // ввести номер K после которого вставлять элементы
        // ввести строку new_str для вставления и количество P
        // пропустить K элементов от головы и вставить P строк new_str

        int K=0;
        int P=0;
        char NewMarker = '@';
        string new_str;
        cout << " Введите номер K после которого вставлять элементы " <<endl;
        cin >> K;
        cout << " Введите строку new_str для вставления " <<endl;
        cin >> new_str;
        cout << " Введите marker " <<endl;
        cin >> NewMarker;

       res << " Введите номер K после которого вставлять элементы " <<endl;
        res << K<<endl;
        res << " Введите строку new_str для вставления " <<endl;
        res << new_str<<endl;
       res << " Введите marker " <<endl;
        res << NewMarker<<endl;


        StrM new_strM;
        new_strM.Marker = NewMarker;
        std::strncpy(new_strM.massiv, new_str.c_str(), new_str.size() > N ? N : new_str.size());
        new_strM.massiv[new_str.size() > N ? N : new_str.size()] = NewMarker;

        cout << " Введите количество P для вставления " <<endl;
        cin >> P;

        if(K > num_stroki)
        {
            cout<<" Номер превышает реальное кол-во строк "<<endl;
           res<<" Номер превышает реальное кол-во строк "<<endl;
            return;
        }

        if(K==0) // вставку в новое место
        {
            ListNode * fakeHead = new ListNode(); // заведем ненастоящую голову
            fakeHead->next = head; // которая будет указывать на настоящую
            insert_after(fakeHead, new_strM, P); // вставим после нее что нужно
            head = fakeHead->next; // восстановим голову (теперь это первый новый элемент)
            delete fakeHead; // удалим ненастоящую голову
        }
        else 
        {
            ListNode * cur = head;
            for (int i = 0; i < K-1; ++i) // пропускаем К элементов
                cur = cur->next;
            insert_after(cur, new_strM, P);
        }

        cout << endl
             << endl;
        cout << "RESULT: " << endl
             << endl;


        res << endl
            << endl;
        res << "RESULT: " << endl
            << endl;

        print(res);
        cout << endl;
        res << endl;
        clear(res);

        cout << " Кончилась обработка файла " << filename << endl
             << endl
             << endl
             << endl;
        res << " Кончилась обработка файла " << filename << endl
            << endl
            << endl
            << endl;
    }
}

