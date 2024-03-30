#include "FormularV.h"

using namespace std;

bool ListNodeV::ReadV(std::ifstream &input, std::ofstream &res)
{
    // пока не перенос строки считывать в ЛистНодХ и создавать новый (некст) если не перенос
    char s;
    int counter = 0;
    f_H.cur = f_H.head ;
    while (s != '\n')
    {
        if(counter%N == 0) 
        {
            f_H.cur = new ListNodeH; 
        }
        input >> noskipws >> s;
        f_H.cur->podstroka.massiv[counter%N] = s;
        ++counter;
        if (counter%N == 0)
            f_H.cur = f_H.cur->next;
    }

    return false;
}
