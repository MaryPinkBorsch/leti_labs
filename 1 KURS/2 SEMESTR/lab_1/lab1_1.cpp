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
    char Marker;
    int len;
};

struct SlovoIdx
{
    int start = -1; // индекс начала слова
    int end = -1;   // индекс конца слова
    int oldidx = -1;
};

// CHTENIE S MARKEROM 1.1.1
bool read_fM(std::string filename, StrM &stroka, ofstream &res)
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
    if (start1 + i == end1 && start2 + i == end2) //????
    {
        return 0; // абсолютно одинаковые слова
    }

    if (start1 + i != end1) // так левое слово длиннее правого т.е. больше го
        return 1;
    else
        return -1;
}

int Compare_slova_idx2(StrL &stroka, int start1, int end1, int start2, int end2) // сравниваем слова
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
        int sdvig = end2 - (start1 + len2);    // na skolko sdvigat (сколько символов переедет за 1 цикл налево на 1 символ)
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

void inplace_swap_slova2(StrL &stroka, int start1, int end1, int start2, int end2)
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
        int sdvig = end2 - (start1 + len2);    // na skolko sdvigat (сколько символов переедет за 1 цикл налево на 1 символ)
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

void Bubble_sort_idx2(StrL &stroka, SlovoIdx *SlovaIdx, int num_slova)
{
    // sorting
    bool swapped = true;
    while (swapped)
    {
        swapped = false;
        for (int i = 0; i < num_slova - 1; i++)
        {
            if (Compare_slova_idx2(stroka, SlovaIdx[i].start, SlovaIdx[i].end, SlovaIdx[i + 1].start, SlovaIdx[i + 1].end) == 1)
            {
                inplace_swap_slova2(stroka, SlovaIdx[i].start, SlovaIdx[i].end, SlovaIdx[i + 1].start, SlovaIdx[i + 1].end);
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

//
void print(StrM stroka, ofstream &res)
{
    int i = 0;
    while (stroka.massiv[i] != stroka.Marker)
    {
        cout << stroka.massiv[i];
        res << stroka.massiv[i];
        i++;
    }
    cout << endl;
    res << endl;
}

void print2(StrL stroka, ofstream &res)
{
    int i = 0;
    // for (i; i < stroka.len || stroka[i] != stroka.Marker; i++)
    // {
    //     cout << stroka.massiv[i];
    // }
    while (stroka.massiv[i] != stroka.Marker || i < stroka.len)
    {
        cout << stroka.massiv[i];
        res << stroka.massiv[i];
        i++;
    }
    res << endl;
    cout << endl;
}

int file_process_1(string filename, StrM s, ofstream &res)
{

    string filename2 = "result.txt";
    // ofstream res(filename2, ios::out | ios::trunc);
    cout << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;
    res << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;

    // StrM s1;
    // read_fM(filename, s); !!!!!

    if (read_fM(filename, s, res) == false)
    {
        cout << "Обработка файла " << filename << " завершена " << endl;
        res << "Обработка файла " << filename << " завершена " << endl;
        return 1;
        abort;
    }
    else
    {

        print(s, res);
        // tut sdelat vivod  v res nashalo/itog
        // process_fM(s1);
        process_inPlace(s);
        print(s, res);

        cout << "Обработка файла " << filename << " завершена " << endl;
        res << "Обработка файла " << filename << " завершена " << endl;
        return 0;
    }
   
}

////////////////////////////////////////////////////////////////////////////////////////////////// 1.1.2.
// CHTENIE S len 1.1.2
bool read_fM2(std::string filename, StrL &stroka, ofstream &res)
{
    ifstream input;
    input.open(filename, std::ios_base::in);
    // // TODO: тут проверить что файл открылся gotovo
    if (input.eof())
    {
        cout << " Файл input пуст, упс 1" << endl;
        res << " Файл input пуст, упс 1" << endl;
        return false;
    }

    int i = 0;
    char s;

    input >> stroka.len; // кол-во символов которые надо считать - длина

    if (stroka.len < 0)
    {
        stroka.len = 0;
        cout << "Отрицательная длина исправлена в 0 в  " << filename << endl;
        res << "Отрицательная длина исправлена в 0 в  " << filename << endl;
        return false;
    }

    if (stroka.len > N)
    {
        stroka.len = N;
        cout << "Слишком большая длина исправлена в " << filename << endl;
        res << "Слишком большая длина исправлена в " << filename << endl;
    }

    input >> stroka.Marker; //  маркер для нашего массива

    if (input.eof())
    {
        cout << " МАркера нет " << endl;
        cout << " Файл " << filename << " пуст, упс 2";
        cout << endl;

        res << " МАркера нет " << endl;
        res << " Файл " << filename << " пуст, упс 2";
        res << endl;

        return false;
    }

    if (input.eof())
    {
        cout << " Файл " << filename << " пуст, упс 3";
        cout << endl;

        res << " Файл " << filename << " пуст, упс 3";
        res << endl;

        return false;
    }

    if (input.eof())
    {
        cout << "stroka pusta v " << filename << endl;
        res << "stroka pusta v " << filename << endl;

        stroka.massiv[0] = stroka.Marker;
    }
    else
    {
        while (1)
        {
            if (i >= N || i >= stroka.len) // i >= ?
            {
                // stroka.massiv[i + 1] = stroka.Marker; //
                break;
            }
            input >> noskipws >> s;
            if (s == '\n')
                break;

            if (input.eof())
                break;
            if (s == stroka.Marker)
                break;

            stroka.massiv[i] = s;
            i++;
            // if (i >= N || i >= stroka.len) // i >= ?
            // {
            //     // stroka.massiv[i + 1] = stroka.Marker; //
            //     break;
            // }
        }
        if (stroka.len > i)
        {
            cout << " реальная длина меньше заданной " << endl;
            res << " реальная длина меньше заданной " << endl;
            stroka.len = i;
            // stroka.massiv[i + 1] = stroka.Marker;
        }

        stroka.massiv[i] = stroka.Marker; //
    }
    if (stroka.massiv[0] == stroka.Marker)
    {
        cout << "stroka pusta v " << filename << endl;
        res << "stroka pusta v " << filename << endl;
    }

    return true;
}

void process_inPlace2(StrL &stroka)
{
    // заполнить массив индексов начала и конца слов в строке
    // отстортировать массив одновременно меняя местами слова

    int i = 0;
    int num_slova = 0;
    SlovoIdx Slova[N];

    while (i < stroka.len || stroka.massiv[i] != stroka.Marker) /////!
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
    Bubble_sort_idx2(stroka, Slova, num_slova);
}

int file_process_2(string filename, StrL s, ofstream &res)
{

    // string filename2 = "result.txt";
    // ofstream res(filename2, ios::out | ios::trunc);
    cout << "Добро пожаловать в lab 1.1.2 Калюжной Марии 3352 " << endl;
    res << "Добро пожаловать в lab 1.1.2 Калюжной Марии 3352 " << endl;

    // StrM s1;
    // read_fM(filename, s); !!!!!

    if (read_fM2(filename, s, res) == false)
    {
        cout << "Обработка файла " << filename << " завершена (2)" << endl;
        res << "Обработка файла " << filename << " завершена (2)" << endl;
        return 1;
        abort;
    }
    else
    {

        print2(s, res);
        // tut sdelat vivod  v res nashalo/itog
        // process_fM(s1);
        process_inPlace2(s);
        print2(s, res);

        cout << "Обработка файла " << filename << " завершена (2)" << endl;
        res << "Обработка файла " << filename << " завершена (2)" << endl;
        return 0;
    }
}

int main(int argc, char *argv[])
{
    string filename2 = "result.txt";
    // string filename1 = "in1_1m.txt";
    ofstream res(filename2, ios::out | ios::trunc);
    // cout << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;

    StrM s1;
    file_process_1("in1_1m.txt", s1, res);
    cout << endl
         << endl
         << endl;

    res << endl
        << endl
        << endl;

    StrM s2;
    file_process_1("in1_1m2.txt", s2, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrM s3;
    file_process_1("in1_1m3.txt", s3, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrM s4;
    file_process_1("in1_1m4.txt", s4, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrM s5;
    file_process_1("in1_1m5.txt", s5, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrM s6;
    file_process_1("in1_1m6.txt", s6, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrM s7;
    file_process_1("in1_1m7.txt", s7, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrM s8;
    file_process_1("in1_1m8.txt", s8, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrM s9;
    file_process_1("in1_1m9.txt", s9, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrM s10;
    file_process_1("in1_1m10.txt", s10, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

        StrM s11;
    file_process_1("in1_1m11.txt", s11, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

        StrM s12;
    file_process_1("in1_1m12.txt", s12, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

        StrM s13;
    file_process_1("in1_1m13.txt", s13, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

        StrM s14;
    file_process_1("in1_1m14.txt", s14, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////// 1.1.2.
    
    StrL ss1;
    file_process_2("in1_2m.txt", ss1, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;
    StrL ss2;
    file_process_2("in1_2m2.txt", ss2, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;
    StrL ss3;
    file_process_2("in1_2m3.txt", ss3, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrL ss4;
    file_process_2("in1_2m4.txt", ss4, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrL ss5;
    file_process_2("in1_2m5.txt", ss5, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrL ss6;
    file_process_2("in1_2m6.txt", ss6, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

    StrL ss7;
    file_process_2("in1_2m7.txt", ss7, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

        StrL ss8;
    file_process_2("in1_2m8.txt", ss8, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

        StrL ss9;
    file_process_2("in1_2m9.txt", ss9, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

StrL ss10;
    file_process_2("in1_2m10.txt", ss10, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

        StrL ss11;
    file_process_2("in1_2m11.txt", ss11, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;

StrL ss12;
    file_process_2("in1_2m12.txt", ss12, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl; 

        StrL ss13;
    file_process_2("in1_2m13.txt", ss13, res);
    cout << endl
         << endl
         << endl;
    res << endl
        << endl
        << endl;  
    return 0;
}