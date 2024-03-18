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

    if (1) //! input.eof()
    {
        head = new ListNode;
    }
    ListNode *cur = head;
    // пока в стриме есть что читать и в тексте есть место, считываем строки
    while (!input.eof() && num_stroki < M) // && cur != nullptr ?
    {
        // if (cur != head)//&& cur == nullptr
        // {
        //     cur = new ListNode;
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
        }
    }

    if (!input.eof() && num_stroki == M)
    {
        cout << "ОБРАБОТКА НЕВОЗМОЖНА, слишком много строк!" << endl;
        res << "ОБРАБОТКА НЕВОЗМОЖНА, слишком много строк!" << endl;
        exit(1);
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
    }
    while (cur2 != nullptr)
    {
        if (cur->strochka.search1(res) == 1 && cur == head)
        {
            ListNode *tmp = head->next;
            delete cur;
            head = tmp;
            cur = tmp;
            if (cur->next == 0)
                break;
            cur2 = cur->next;
            continue;
        }
        else if (cur2->strochka.search1(res) == 1 && cur2->next != 0)
        {
            ListNode *tmp = cur2->next;
            delete cur2;
            cur->next = tmp;
            cur2 = tmp;
        }
        else if(cur2->strochka.search1(res) == 1 && cur2->next == 0)
        {
            delete cur2;
            cur->next = nullptr;
        }
        else
        {
            cur = cur->next;
            cur2 = cur2->next;
        }
    }

    
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