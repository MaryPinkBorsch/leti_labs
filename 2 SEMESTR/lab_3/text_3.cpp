#include "text3.h"

using namespace std;

void Text::print2(std::ofstream &res)
{
    ListNode *cur = head;

    while (cur != nullptr)
    {
        cur->strochka.print1(res);
        cur = cur->next;
    }
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
        head = new ListNode;
        head->strochka.massiv[0] = {-1};
    }
    ListNode *cur = head;
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
        if (cur->strochka.read_StrM(input, res) == false)
        {
            if (input.eof())
                break;
        }
        else
        {
            ++num_stroki;
            cur->next = new ListNode;
            cur = cur->next;
            cur->strochka.massiv[0] = {-1};
        }
    }
    // if(cur==0)
    //     delete cur;
    if (head->strochka.massiv[0] == -1)
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
    ListNode *cur2 = head->next;
    if (cur2 == nullptr && cur->strochka.search1(res) == 1)
    {
        delete cur;
        res << " Голова была удалена!!!" << endl;
        cout << " Голова была удалена!!!" << endl;
        return;
    }
    while (cur2 != nullptr)
    {
        if (cur->strochka.search1(res) == 1 && cur == head)
        {
            ListNode *tmp = head->next;
            delete cur;
            head = tmp;
            cur = tmp;
            if (cur->next == nullptr)
                break;
            cur2 = cur->next;
            // continue;
        }
        else if (cur2->strochka.search1(res) == 1 && cur2->next != 0)
        {
            if (cur->next == nullptr)
                break;
            if (cur2 == 0)
                break;
            ListNode *tmp = cur2->next;
            delete cur2;
            cur->next = tmp;
            cur2 = tmp;
            // continue;
        }
        else if (cur2->strochka.search1(res) == 1 && cur2->next == 0 && cur2 != 0)
        {
            if (cur->next == nullptr)
                break;
            if (cur2 == 0)
                break;
            delete cur2;
            cur->next = nullptr;
            return;
            abort;
        }
        else
        {
            if (cur->next == 0 || cur2->next == 0)
                break;
            cur = cur->next;
            cur2 = cur2->next;
        }
    }
}

void Text::deleteng(std::ofstream &res)
{
    // if (head == 0)
    //     return;
    ListNode *tmp = head->next;
    ListNode *cur = head;
    if (cur == nullptr)
    {
        cout << " Список пуст " << endl;
        res << " Список пуст " << endl;
        return;
        abort();
    }
    if (cur->next == nullptr)
    {
        delete cur;
        cout << " Список был удален. " << endl;
        res << " Список был удален. " << endl;
        return;
        abort();
    }

    while (cur != nullptr)
    {
        delete cur;
        head = tmp;
        cur = head;
        if (cur->next == nullptr)
        {
            delete cur;
            break;
        }
        tmp = cur->next;
    }

    cout << " Список был удален. " << endl;
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