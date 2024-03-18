#include "text3.h"

using namespace std;

void Text::print2(std::ofstream &res)
{
    ListNode *cur = head;
    // for (int i = 0; i < num_stroki; i++)
    // {
    //     stroki[i].print1(res);
    // }
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

    // // идем по всем строкам
    // int num_predlojenia = 0;
    // for (int i = 0; i < num_stroki; i++)
    // {
    //     int j = 0;
    //     // идем по всем символам каждой строки
    //     while (stroki[i].massiv[j] != stroki[i].Marker)
    //     {
    //         // если символ - разделитель предложений
    //         if (IsRazdelitel(stroki[i].massiv[j]))
    //         {
    //             // и при этом у нас уже было начало предолжения
    //             if (1)
    //             {
    //                 // заполним текущее предложение (концы)
    //                 // indexi_predlojenii[num_predlojenia].stroka_idx_end = i;
    //                 // indexi_predlojenii[num_predlojenia].stroka_smeschenie_end = j;
    //                 // indexi_predlojenii[num_predlojenia].num_znaki++;
    //                 // увеличим счетчик предложений
    //                 //++num_predlojenia;
    //                 if (num_predlojenia == M)
    //                 {
    //                     cout << "ОБРАБОТКА НЕВОЗМОЖНА, слишком много предложений!" << endl;
    //                     res << "ОБРАБОТКА НЕВОЗМОЖНА, слишком много предложений!" << endl;
    //                     exit(1);
    //                 }
    //             }
    //         }
    //         else
    //         {
    //             // у нас обычный символ. Если начала предолжения не было, то это оно

    //             // if (IsZnak(stroki[i].massiv[j]))
    //             //!;
    //         }
    //         // следующий символ
    //         ++j;
    //     }
    // }
}

void Text::Delete(std::ofstream &res)
{
    // int Max = MaxZnaki(res);
    int FromStr = 0;  // строка откуда переносится предложение
    int FromChar = 0; // символ в строке откуда переносится предложение
    int ToStr = 0;    // строка куда переносится предложение
    int ToChar = 0;   // символ в строке куда переносится предложение
    int next_predlojenie = 0;

    // пока не прошли весь текст
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
        // Delete(res);

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