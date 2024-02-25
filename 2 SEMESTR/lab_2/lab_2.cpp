#include "lab_2.h"

using namespace std;

// CHTENIE S MARKEROM 1.1.1
bool read_fM(std::string filename, StrM &stroka, std::ofstream &res)
{
    ifstream input;
    input.open(filename, std::ios_base::in);
    
    int i = 0;
    char s;
    
    input >> stroka.Marker; // первый маркер для нашего массива
    if (input.eof())
    {
        cout << " Файл " << filename << " пуст, упс";
        cout << endl;

        res << " Файл " << filename << " пуст, упс";
        res << endl;

        return false;
    }
    char MMarker = -1; // второй маркер для прерывания чтения с файла
    input >> MMarker;

    if (input.eof())
    {
        stroka.massiv[0] = stroka.Marker;
        cout << "stroka pusta v " << filename << endl;
        res << "stroka pusta v " << filename << endl;
    }
    else
    {
        while (1)
        {
            input >> noskipws >> s;
            if (s == MMarker)
                break;
            if (s == '\n')
                break;

            if (input.eof())
                break;

            stroka.massiv[i] = s;
            i++;
            if (i >= N)
            {
                break;
            }
        }
        stroka.massiv[i] = stroka.Marker;
    }

    if (stroka.massiv[0] == stroka.Marker && MMarker == -1)
    {
        cout << "Не хватает 2-го маркера  в файле " << filename;
        cout << endl;

        res << "Не хватает 2-го маркера  в файле " << filename;
        res << endl;
    }
    if (stroka.massiv[0] == stroka.Marker && MMarker != -1)
    {
        cout << " Строка пуста, 2 маркера есть v  " << filename << endl;
        res << " Строка пуста, 2 маркера есть v  " << filename << endl;
    }
    return true;
}

// 17. Предложения могут находится в разных строках текста. 
// Удалить в тексте те предложения, которые: 3) содержат максимальное
// число знаков препинания

int main(int argc, char * argv[]) 
{
    // Считать файл в объект структуры Text
    // Пройти по строкам и выделить предложения
    // Отсортировать предложения по количеству знаков препинания
    // inplace удалить из объекта текст предложения имевшие макс. число знаков препинания
    
    return 0;
}