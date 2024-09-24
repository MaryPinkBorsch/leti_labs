#include "text3.h"

using namespace std;

void Text::print2(std::ofstream &res)
{
    ListNode *cur = head;

    while (cur != nullptr) //(cur != nullptr)
    {
        cur->strochka.print1(res);

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
        head->addMemo(head);
        // head = new ListNode; // addMemo

        // head->infoInput(head);
        //  head->strochka.massiv[0] = {-1}; // infoInput
    }
    ListNode *cur = head;
    ListNode *prev = nullptr;
    // пока в стриме есть что читать и в тексте есть место, считываем строки
    while (!input.eof() && num_stroki < M) // && cur != nullptr ?
    {
        // if (cur == head && cur->next != 0)
        // {
        //     cur->next = new ListNode;
        //     cur = cur->next;
        // }
        // else if (cur != head && cur->next == nullptr) //
        // {
        //     cur->next = new ListNode;
        //     cur = cur->next;
        // }

        //     cur->addNext(cur);
        //    // cur->next = new ListNode; // addMemo
        //     cur = cur->next;
        //     cur->infoInput(cur);
        //     //cur->strochka.massiv[0] = {-1}; // infoInput

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

    // if(cur==0)
    //     delete cur;
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

bool Text::IsZnak(char c)
{
    return c == ',' || c == ':' || c == '-' || c == '\'' || c == ';' || c == '.' || c == '?' || c == '!';
}

void Text::process_znaki(std::ofstream &res)
{
    ListNode *cur = head;
    if (head == nullptr)
    {
        res << "Список пуст!! " << endl;
        // если пустой список сразу возвращаемся
        return;
    }
    ListNode *prev = nullptr;
    while (cur != nullptr)
    {
        if (cur->strochka.search1(res) == true)
        {
            ListNode *tmp = cur->next;
            if (prev)
                prev->next = tmp;
            delete cur;
            if (cur == head) // если только что удалили голову - переставим ее вперед
            {
                head = tmp;
               // res<<"голова удалена"<< endl;
            }
            cur = tmp;
        }
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }
}

void Text::deleteng(std::ofstream &res)
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

// 17. Предложения могут находится в разных строках текста.
// Удалить в тексте те предложения, которые: 3) содержат максимальное
// число знаков препинания

void Text::BIG_process(string filename, ofstream &res)
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
        print2(res);
        process_znaki(res);

        cout << endl
             << endl;
        cout << "RESULT: " << endl
             << endl;

        res << endl
            << endl;
        res << "RESULT: " << endl
            << endl;

        print2(res);
        cout << endl;
        res << endl;
        deleteng(res);

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