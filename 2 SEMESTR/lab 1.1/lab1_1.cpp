#include <iostream>
#include <fstream>
#include <iomanip>

#include <cstring> //!!! only devug

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
                SlovoIdx tmp = SlovaIdx[i];
                SlovaIdx[i] = SlovaIdx[i + 1];
                SlovaIdx[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}

// эта функция меняет местами подстроки в строке
void inplace_swap_slova(StrM &stroka, int start1, int end1, int start2, int end2)
{
    if (start1 == start2 && end1 == end2)
        return;

    // !!!! слово1 это левое слово то есть end1 < start2
    if (end1 > start2)
    {
        cout << "NE TAK SLOVA raspologeni" << endl;
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

void process_inPlace(StrM &stroka)
{
    // -------- МАСИНЫ СТРАДАНИЯ ------------

    // пройтись по массиву и "запомнитьь" каждый 1й и последн. символ слова (т.е. запомнить каждый индекс начала слова)
    // если 2 первых символа одинаковы запустить проверку всего слова до того момента пока не станет яясно какое слово меньще
    // если 2 слова индентичны записать их по очереди

    // после сравнить первыйе символы и найти самое "маленткое слово" (по алфавиту)
    // найти его длину в символах
    // найти место куда вставить
    // сдвинуть с того места весь массив на Н символов вправо и вставить слово

    // -------- eshe СТРАДАНИЯ ------------

    // заполнить массив индексов начала и конца слов в строке
    // сделать копию массива и отстортировать копию
    // завести массив булов для того чтобы отмечать какие слова уже меняли местами
    // поменять слова местами

    int i = 0;
    int num_slova = 0;
    SlovoIdx Slova[N];
    SlovoIdx SlovaSorted[N];
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
        else if (stroka.massiv[i] == ' ')
        {
            if (Slova[num_slova].start != -1)
            {
                Slova[num_slova].end = i - 1;
                num_slova++;
            }
        }
        i++;
    }
    bool Swapped[N];
    for (int i = 0; i < num_slova; ++i)
    {
        SlovaSorted[i] = Slova[i];
        Swapped[i] = false;
    }

    Bubble_sort_idx(stroka, SlovaSorted, num_slova);
    for (int i = 0; i < num_slova; ++i)
    {
        if (Swapped[i] == false)
        {
            inplace_swap_slova(stroka, Slova[i].start, Slova[i].end, SlovaSorted[i].start, SlovaSorted[i].end);
            Swapped[Slova[i].oldidx] = true;
            Swapped[SlovaSorted[i].oldidx] = true;
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

int main(int argc, char *argv[])
{
    string filename2 = "result.txt";
    string filename1 = "in1_1m.txt";
    ofstream res(filename2, ios::out | ios::trunc);
    cout << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;

    // DEBUG!!!!
    // StrM wtf;
    // std::strcpy(wtf.massiv, "abcxxxdefgh");
    // print(wtf);
    // inplace_swap_slova(wtf, 0, 2, 6, 10);
    // print(wtf);
    // cout << endl;

    // std::strcpy(wtf.massiv, "abcMxxxdefgh");
    // print(wtf);
    // inplace_swap_slova(wtf, 0, 3, 7, 11);
    // print(wtf);
    // cout << endl;

    // std::strcpy(wtf.massiv, "abcMxxxdefghxxx");
    // print(wtf);
    // inplace_swap_slova(wtf, 0, 3, 7, 11);
    // print(wtf);
    // cout << endl;

    // std::strcpy(wtf.massiv, "defghxxxabc");
    // print(wtf);
    // inplace_swap_slova(wtf, 0, 4, 8, 10);
    // print(wtf);
    // cout << endl;

    // std::strcpy(wtf.massiv, "defghxxxabcM");
    // print(wtf);
    // inplace_swap_slova(wtf, 0, 4, 8, 11);
    // print(wtf);
    // cout << endl;

    // std::strcpy(wtf.massiv, "defxxxabc");
    // print(wtf);
    // inplace_swap_slova(wtf, 0, 2, 6, 8);
    // print(wtf);
    // cout << endl;
    // DEBUG!!!!

    StrM s1;
    read_fM(filename1, s1);
    print(s1);
    // process_fM(s1);
    process_inPlace(s1);
    print(s1);
    return 0;
}