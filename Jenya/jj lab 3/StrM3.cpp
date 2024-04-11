#include "ListNode.h"
using namespace std;

void StrM::print(std::ofstream &res)
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
        return false;
    }

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
            // if (s == Marker)
            //     break;

            if (s == '\n')
                break;

            if (input.eof())
                break;

            massiv[i] = s;
            i++;
            if (i >= N)
            {
                // ????
                // if (input.eof())
                //     break;
                char nextChar;
                do
                {
                    input >> noskipws >> nextChar;
                    if (input.eof())
                        break;
                } while (nextChar != '\n');
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
