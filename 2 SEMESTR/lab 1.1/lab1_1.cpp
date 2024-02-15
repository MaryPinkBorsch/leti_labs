#include <iostream>
#include <fstream>
#include <iomanip>

// #include <cstring> //!!! only devug

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

struct SlovoIdx
{
    int start = -1; // индекс начала слова
    int end = -1;   // индекс конца слова
    int oldidx = -1;
};

// CHTENIE S MARKEROM 1.1.1
bool read_fM(std::string filename, StrM &stroka)
{
    ifstream input;
    input.open(filename, std::ios_base::in);
    // // TODO: тут проверить что файл открылся gotovo
    // if (input.eof())
    // {
    //     cout << " Файл input пуст, упс" << endl;
    //     return false;
    // }

    int i = 0;
    char s;
    // if (input.eof())
    // {
    //     cout << "Не хватает 1-го маркера, стrока пуста в файле " << filename;
    // }
    input >> stroka.Marker; // первый маркер для нашего массива
    if (input.eof())
    {
        cout << " Файл " << filename << " пуст, упс";
        cout << endl;

        return false;
    }
    char MMarker = -1; // второй маркер для прерывания чтения с файла
    input >> MMarker;

    if (input.eof())
    {
        stroka.massiv[0] = stroka.Marker;
        cout << "stroka pusta v " << filename << endl;
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
    }
    if (stroka.massiv[0] == stroka.Marker && MMarker != -1)
    {
        cout << " Строка пуста, 2 маркера есть v  " << filename << endl;
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

// если start1-end1 слово меньше start2-end2 по алфавиту, то вернут -1
// если start1-end1 слово больше start2-end2 по алфавиту, то вернут 1
// если РАВНЫ вернут 0
int Compare_slova_idx(StrM &stroka, int start1, int end1, int start2, int end2) // сравниваем слова
{
    int i = 0;
    while (start1 + i != end1 && start2 + i != end2)
    {
        if (stroka.massiv[start1 + i] < stroka.massiv[start2 + i])
            return -1;
        if (stroka.massiv[start1 + i] > stroka.massiv[start2 + i])
            return 1;
        i++;
    }
    if (start1 + i == end1 && start2 + i == end2)
    {
        return 0; // абсолютно одинаковые слова
    }

    if (start1 + i != end1) // так левое слово длиннее правого т.е. больше го
        return 1;
    else
        return -1;
}

// эта функция меняет местами подстроки в строке
void inplace_swap_slova(StrM &stroka, int start1, int end1, int start2, int end2)
{
    if (start1 == start2 && end1 == end2)
        return;

    // !!!! слово1 это левое слово то есть end1 < start2
    if (end1 > start2)
    {
        int tmp = start1;
        start1 = start2;
        start2 = tmp;
        tmp = end1;
        end1 = end2;
        end2 = tmp;
    }
    int len1 = end1 - start1 + 1;
    int len2 = end2 - start2 + 1;
    // если слово1 короче слова2 тогда
    if (len1 < len2)
    {
        // меняем буквы пока позволяет длинна слова1
        for (int i = 0; i < len1; i++)
        {
            char tmp = stroka.massiv[start1 + i];
            stroka.massiv[start1 + i] = stroka.massiv[start2 + i];
            stroka.massiv[start2 + i] = tmp;
        }
        // после этого по одной переносим оставшиеся буквы слова два влево и сдвигаем остаток вправо
        // количество сдвигаемых симовлов равно start2 - start1
        int sdvig = start2 - start1; // na skolko sdvigat
        int nado_sdvinut = len2 - len1;
        for (int i = 0; i < nado_sdvinut; i++)
        {
            char tmp = stroka.massiv[start2 + len1 + i];
            for (int j = 0; j < sdvig; j++)
            {
                stroka.massiv[start2 + len1 + i - j] = stroka.massiv[start2 + len1 + i - j - 1]; // прямо как a[i+1] = a[i] pochti СПРАВА НАЛЕВО
            }
            stroka.massiv[end1 + 1 + i] = tmp;
        }
    }
    else
    {
        // иначе если слово2 короче слова1 тогда
        // меняем буквы пока позволяет длинна слова2
        for (int i = 0; i < len2; i++)
        {
            char tmp = stroka.massiv[start1 + i];
            stroka.massiv[start1 + i] = stroka.massiv[start2 + i];
            stroka.massiv[start2 + i] = tmp;
        }
        // после этого по одной переносим оставшиеся буквы слова1  вправо и сдвигаем остаток влево
        int sdvig = end2 - end1 + 1;    // na skolko sdvigat
        int nado_sdvinut = len1 - len2; // кол-во перемещаемых элеменотов
        for (int i = 0; i < nado_sdvinut; i++)
        {
            char tmp = stroka.massiv[start1 + len2];
            for (int j = 0; j < sdvig; j++)
            {
                stroka.massiv[start1 + len2 + j] = stroka.massiv[start1 + len2 + j + 1]; // прямо как a[i+1] = a[i] pochti
            }
            stroka.massiv[end2] = tmp;
        }
    }
}

void Bubble_sort_idx(StrM &stroka, SlovoIdx *SlovaIdx, int num_slova)
{
    // sorting
    bool swapped = true;
    while (swapped)
    {
        swapped = false;
        for (int i = 0; i < num_slova - 1; i++)
        {
            if (Compare_slova_idx(stroka, SlovaIdx[i].start, SlovaIdx[i].end, SlovaIdx[i + 1].start, SlovaIdx[i + 1].end) == 1)
            {
                inplace_swap_slova(stroka, SlovaIdx[i].start, SlovaIdx[i].end, SlovaIdx[i + 1].start, SlovaIdx[i + 1].end);
                int len1 = SlovaIdx[i].end - SlovaIdx[i].start;
                int len2 = SlovaIdx[i + 1].end - SlovaIdx[i + 1].start;
                SlovaIdx[i].end = SlovaIdx[i].start + len2;
                SlovaIdx[i + 1].start += len2 - len1;
                SlovaIdx[i + 1].end = SlovaIdx[i + 1].start + len1;
                swapped = true;
            }
        }
    }
}

void process_inPlace(StrM &stroka)
{
    // заполнить массив индексов начала и конца слов в строке
    // отстортировать массив одновременно меняя местами слова

    int i = 0;
    int num_slova = 0;
    SlovoIdx Slova[N];
    while (stroka.massiv[i] != stroka.Marker)
    {
        if (stroka.massiv[i] != ' ')
        {
            if (Slova[num_slova].start == -1)
            {
                Slova[num_slova].start = i;
                Slova[num_slova].oldidx = num_slova;
            }
        }
        else if (stroka.massiv[i] == ' ') /////// dobavit || stroka.massiv[i] != '\n'???
        {
            if (Slova[num_slova].start != -1)
            {
                Slova[num_slova].end = i - 1;
                num_slova++;
            }
        }
        i++;
    }
    // для случая когда после последнего слова нет пробела
    if (Slova[num_slova].end == -1 && Slova[num_slova].start != -1)
    {
        Slova[num_slova].end = i - 1;
        num_slova++;
    }
    Bubble_sort_idx(stroka, Slova, num_slova);
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
    stroka = result;
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

int file_process_1(string filename, StrM s)
{

    string filename2 = "result.txt";
    ofstream res(filename2, ios::out | ios::trunc);
    cout << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;

    // StrM s1;
    // read_fM(filename, s); !!!!!

    if (read_fM(filename, s) == false)
    {
        cout << "Обработка файла " << filename << " завершена " << endl;
        return 1;
        abort;
    }
    else
    {

        print(s);
        // tut sdelat vivod  v res nashalo/itog
        // process_fM(s1);
        process_inPlace(s);
        print(s);

        cout << "Обработка файла " << filename << " завершена " << endl;
        return 0;
    }
    // print(s);
    // // tut sdelat vivod  v res nashalo/itog
    // // process_fM(s1);
    // process_inPlace(s);
    // print(s);

    // return 0;
}

int main(int argc, char *argv[])
{
    // string filename2 = "result.txt";
    // string filename1 = "in1_1m.txt";
    // ofstream res(filename2, ios::out | ios::trunc);
    // cout << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;

    // // DEBUG!!!!
    // // StrM wtf;
    // // std::strcpy(wtf.massiv, "abcxxxdefgh");
    // // print(wtf);
    // // inplace_swap_slova(wtf, 0, 2, 6, 10);
    // // print(wtf);
    // // cout << endl;

    // // std::strcpy(wtf.massiv, "abcMxxxdefgh");
    // // print(wtf);
    // // inplace_swap_slova(wtf, 0, 3, 7, 11);
    // // print(wtf);
    // // cout << endl;

    // // std::strcpy(wtf.massiv, "abcMxxxdefghxxx");
    // // print(wtf);
    // // inplace_swap_slova(wtf, 0, 3, 7, 11);
    // // print(wtf);
    // // cout << endl;

    // // std::strcpy(wtf.massiv, "defghxxxabc");
    // // print(wtf);
    // // inplace_swap_slova(wtf, 0, 4, 8, 10);
    // // print(wtf);
    // // cout << endl;

    // // std::strcpy(wtf.massiv, "defghxxxabcM");
    // // print(wtf);
    // // inplace_swap_slova(wtf, 0, 4, 8, 11);
    // // print(wtf);
    // // cout << endl;

    // // std::strcpy(wtf.massiv, "defxxxabc");
    // // print(wtf);
    // // inplace_swap_slova(wtf, 0, 2, 6, 8);
    // // print(wtf);
    // // cout << endl;
    // // DEBUG!!!!

    StrM s1;
    file_process_1("in1_1m.txt", s1);
    cout << endl
         << endl
         << endl;

    StrM s2;
    file_process_1("in1_1m2.txt", s2);
    cout << endl
         << endl
         << endl;

    StrM s3;
    file_process_1("in1_1m3.txt", s3);
    cout << endl
         << endl
         << endl;

    StrM s4;
    file_process_1("in1_1m4.txt", s4);
    cout << endl
         << endl
         << endl;

    StrM s5;
    file_process_1("in1_1m5.txt", s5);
    cout << endl
         << endl
         << endl;

    StrM s6;
    file_process_1("in1_1m6.txt", s6);
    cout << endl
         << endl
         << endl;

    StrM s7;
    file_process_1("in1_1m7.txt", s7);
    cout << endl
         << endl
         << endl;

    StrM s8;
    file_process_1("in1_1m8.txt", s8);
    cout << endl
         << endl
         << endl;

    StrM s9;
    file_process_1("in1_1m9.txt", s9);
    cout << endl
         << endl
         << endl;

    StrM s10;
    file_process_1("in1_1m10.txt", s10);
    cout << endl
         << endl
         << endl;
    return 0;
}