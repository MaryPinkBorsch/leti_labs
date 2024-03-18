#include "ListNode.h"
using namespace std;


void StrM::print1(std::ofstream &res)
{
    int i = 0;
    while (massiv[i] != Marker)
    {
        cout << massiv[i];
        res << massiv[i];
        i++;
    }
    cout << endl;
    res << endl;
}

// CHTENIE S MARKEROM 1.1.1
bool StrM::read_StrM(std::ifstream &input, std::ofstream &res)
{
    int i = 0;
    char s;

    input >> Marker; //  маркер для нашего массива
    if (Marker == '\n')
    {
        return false;
    }
    if (input.eof())
    {
        // cout << " Файл пуст, упс"; ///// !!!! s etim toje nado choto sdelat
        // cout << endl;

        // res << " Файл пуст, упс";
        // res << endl;

        return false;
    }
    // char MMarker = -1; // второй маркер для прерывания чтения с файла
    // input >> MMarker;

    if (input.eof())
    {
        massiv[0] = Marker;
        cout << "stroka pusta v " << endl;
        res << "stroka pusta v " << endl;
    }
    else
    {
        while (1)
        {
            input >> noskipws >> s;

            if (s == '\n')
                break;

            if (input.eof())
                break;          

            massiv[i] = s;
            i++;
            if (i >= N)
            {
                char nextChar;
                do
                {
                    input >> noskipws >> nextChar;
                } while(nextChar!='\n');
                break;
            }
        }
        massiv[i] = Marker;
    }

    if (massiv[0] == Marker)
    {
        cout << " Строка пуста  " << endl;
        res << " Строка пуста  " << endl;
    }
    return true;
}