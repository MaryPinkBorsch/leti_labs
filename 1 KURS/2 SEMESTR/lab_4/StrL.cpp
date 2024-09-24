#include "StrL.h"
using namespace std;
bool StrL::search1(std::ofstream &res)
{
    // if(massiv[0] == -1)
    // {
    //     return false;
    // }

    int i = 0;
    while (i< len)
    {
        if(massiv[i] ==0)
            return false;
        if (massiv[i] == ',' || massiv[i] == ':' || massiv[i] == '-' || massiv[i] == '\'' || massiv[i] == ';' || massiv[i] == '.' || massiv[i] == '?' || massiv[i] == '!')
        {
            //cout << "Найдено предложение для удаления!" << endl;
            return true;
        }
        i++;
    }
    return false;
}