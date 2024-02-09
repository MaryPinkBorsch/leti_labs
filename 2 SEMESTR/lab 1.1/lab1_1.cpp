#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
static const int N = 100;
struct StrM
{
    char massiv[N + 1];
    char Marker;
};

struct StrL
{
    char massiv[N];
    // char Marker;
    int len;
};

void readf()
{
}
// CHTENIE S MARKEROM 1.1.1
bool read_fM(std::string filename, StrM &stroka)
{
    ifstream input;
    input.open(filename, std::ios_base::in);
    // TODO: тут проверить что файл открылся gotovo
    if (input.eof())
    {
        cout << " Файл input пуст, упс" << endl;
        return false;
    }

    int i = 0;
    char s;
    input >> stroka.Marker; // первый маркер для нашего массива
    if (input.eof())
    {
        cout << "Не хватает 2-го маркера, стrока пуста в файле " << filename;
    }
    char MMarker; // второй маркер для прерывания чтения с файла
    input >> MMarker;

    if (input.eof())
    {
        stroka.massiv[0] = stroka.Marker;
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

    return true;
}

// если ЛЕВОЕ слово меньше ПРАВОГО по алфавиту, то вернут -1
// если ЛЕВОЕ слово больше ПРАВОГО по алфавиту, то вернут 1
// если РАВНЫ вернут 0
int Compare_slova(StrM &l, StrM &r) // сравниваем слова
{
    int i = 0;
    while (l.massiv[i] != l.Marker && r.massiv[i] != r.Marker)
    {
        if (l.massiv[i] < r.massiv[i])
            return -1;
        if (l.massiv[i] > r.massiv[i])
            return 1;
        i++;
    }
    if (l.massiv[i] == l.Marker && r.massiv[i] == r.Marker)
    {
        return 0; // абсолютно одинаковые слова
    }

    if (l.massiv[i] != l.Marker) // так левое слово длиннее правого т.е. больше го
        return 1;
    else
        return -1;
}

void Bubble_sort(StrM *Slova, int num_slova)
{
    // sorting
    bool swapped = true;
    while (swapped)
    {
        swapped = false;
        for (int i = 0; i < num_slova - 1; i++)
        {
            if (Compare_slova(Slova[i], Slova[i + 1]) == 1)
            {
                StrM tmp = Slova[i];
                Slova[i] = Slova[i + 1];
                Slova[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}

void process_fM(StrM &stroka)
{
    // разбить строку на слова, сохраним в массив StrM
    // написать функцию для сравнения строк
    // отсортировать массив слов пузырьком
    // вывод в результ файл исходника и концовки резульата

    int i = 0;
    int j = 0;
    int num_slova = 0;
    StrM Slova[N];
    while (stroka.massiv[i] != stroka.Marker)
    {
        if (stroka.massiv[i] != ' ')
        {
            Slova[num_slova].massiv[j] = stroka.massiv[i];
            j++;
        }
        else if (stroka.massiv[i] == ' ' && j != 0)
        {
            Slova[num_slova].massiv[j] = stroka.Marker;
            Slova[num_slova].Marker = stroka.Marker;
            num_slova++;
            j = 0;
        }
        i++;
    }
    // sorting
    Bubble_sort(Slova, num_slova);
    StrM result;
    int counter = 0;
    result.Marker = stroka.Marker;
    for (int i = 0; i < num_slova; i++)
    {
        int counter2 = 0;
        while (Slova[i].massiv[counter2] != Slova[i].Marker)
        {
            result.massiv[counter] = Slova[i].massiv[counter2];
            counter++;
            counter2++;
        }
        result.massiv[counter] = ' ';
        counter++;
    }
    result.massiv[counter] = result.Marker;
}

void print(StrM stroka)
{
    int i = 0;
    while (stroka.massiv[i] != stroka.Marker)
    {
        cout << stroka.massiv[i];
        i++;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    string filename2 = "result.txt";
    string filename1 = "in1_1m.txt";
    ofstream res(filename2, ios::out | ios::trunc);
    cout << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;

    StrM s1;
    read_fM(filename1, s1);
    print(s1);
    process_fM(s1);
    return 0;
}