#include "StrL.h"

using namespace std;
void StrL::Read(std::ifstream &input, std::ofstream &log)
{
    StrL *cur = this;
    char s = -1;
    int counter = 0;
    while (s != Marker)
    {
        input >> noskipws >> s;

        cur->massiv[counter % N] = s;
        ++counter;
        if (counter % N == 0)
        {
            cur->len = N;
            cur->next = new StrL;
            cur = cur->next;
        }
    }
    cur->len = counter % N;
}
void StrL::Write(std::ofstream &output, std::ofstream &log)
{
    StrL *cur = this;
    // // посчитаем общую длину
    // int big_len = 0;
    // while (cur != nullptr)
    // {
    //     big_len += len;
    //     cur = cur->next;
    // }

    // char buf[128];

    while (cur != nullptr)
    {
        for (int i = 0; i < len; i++)
        {
            output << massiv[i];
        }
        cur = cur->next;
    }
    output << Marker;
}
void StrL::Print(std::ofstream &log)
{
    StrL *cur = this; // первый элемент общей строчки

    while (cur != nullptr)
    {
        for (int i = 0; i < len; i++)
        {
            cout << massiv[i];
            log << massiv[i];
        }
        cur = cur->next;
    }
    cout << endl;
    log << endl;
}
// void StrL::FromStroka(std::string value) {}
void StrL::Clear(std::ofstream &log)
{
    // ОНО ДОЛЖНО ОЧИСТИТЬ ПАМЯТЬ В STRL
}

bool StrL::Equal(StrL &another, std::ofstream &log)
{
    StrL *cur = this;
    StrL *cur_other = &another;
    while (cur_other != nullptr && cur != nullptr)
    {
        if (cur->len != cur_other->len)
            return false;
        for (int i = 0; i < cur->len; i++)
        {
            if (cur->massiv[i] != cur_other->massiv[i])
                return false;
        }
        cur = cur->next;
        cur_other = cur_other->next;
    }
    if (cur != cur_other)
        return false;

    return true;
}