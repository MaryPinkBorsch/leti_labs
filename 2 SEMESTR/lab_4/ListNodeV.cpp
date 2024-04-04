#include "FormularV.h"

using namespace std;

bool ListNodeV::ReadV(std::ifstream &input, std::ofstream &res)
{
    if (input.eof())
        return false;
    // пока не перенос строки считывать в ЛистНодХ и создавать новый (некст) если не перенос
    char s = 0;
    int counter = 0;
    f_H.head = new ListNodeH;
    f_H.cur = f_H.head;

    while (s != '\n')
    {
        if (input.eof())
            break;
        input >> noskipws >> s;
        if (s == '\n'||s == 0) //!!   if (s == '\n')
            break;
        f_H.cur->podstroka.massiv[counter % N] = s;
        ++counter;
        if (counter % N == 0)
        {
            f_H.cur->podstroka.len = N;
            f_H.cur->next = new ListNodeH;
            f_H.cur = f_H.cur->next;
        }
    }
    f_H.cur->podstroka.len = counter % N;
    return true;
}

void ListNodeV::print1(std::ofstream &res)
{
    f_H.cur = f_H.head;
    if (f_H.head == nullptr)
        return;
    while (f_H.cur != nullptr)
    {
        for (int i = 0; i < f_H.cur->podstroka.len; i++)
        {
            cout << f_H.cur->podstroka.massiv[i];
            res << f_H.cur->podstroka.massiv[i];
        }
        cout << " -> ";
        res << " -> ";
        f_H.cur = f_H.cur->next;
    }
    cout << " NULL_H" << endl;
    res << " NULL_H" << endl;
}
