#include <iostream>
#include <fstream>
#include <iomanip>

// Переставить последовательно в строке подходящие слова следующим образом:
//  по последнему символу слова найти к нему ближайшее справа,
//  начинающееся на этот же символ, провести слияние слов
//(правое переносится после левого, символ остается один).

using namespace std;
static const int N = 100;
struct StrM
{
    char A[N + 1];
    char Mark;
};

struct StrL
{
    char A[N];
    char Mark;
    int len;
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
    input >> stroka.Mark; // первый маркер для нашего массива
    if (input.eof())
    {
        cout << " Файл " << filename << " пуст, упс";
        cout << endl;

        res << " Файл " << filename << " пуст, упс";
        res << endl;

        return false;
    }
    char MMark = -1; // второй маркер для прерывания чтения с файла
    input >> MMark;

    if (input.eof())
    {
        stroka.A[0] = stroka.Mark;
        cout << "stroka pusta v " << filename << endl;
        res << "stroka pusta v " << filename << endl;
    }
    else
    {
        while (1)
        {
            input >> noskipws >> s;
            if (s == MMark)
                break;
            if (s == '\n')
                break;

            if (input.eof())
                break;

            stroka.A[i] = s;
            i++;
            if (i >= N)
            {
                break;
            }
        }
        stroka.A[i] = stroka.Mark;
    }

    if (stroka.A[0] == stroka.Mark && MMark == -1)
    {
        cout << "Не хватает 2-го маркера  в файле " << filename;
        cout << endl;

        res << "Не хватает 2-го маркера  в файле " << filename;
        res << endl;
    }
    if (stroka.A[0] == stroka.Mark && MMark != -1)
    {
        cout << " Строка пуста, 2 маркера есть v  " << filename << endl;
        res << " Строка пуста, 2 маркера есть v  " << filename << endl;
    }
    return true;
}

void print(StrM stroka, ofstream &res)
{
    int i = 0;
    while (stroka.A[i] != stroka.Mark)
    {
        cout << stroka.A[i];
        res << stroka.A[i];
        i++;
    }
    cout << endl;
    res << endl;
}

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
            char tmp = stroka.A[start1 + i];
            stroka.A[start1 + i] = stroka.A[start2 + i];
            stroka.A[start2 + i] = tmp;
        }
        // после этого по одной переносим оставшиеся буквы слова два влево и сдвигаем остаток вправо
        // количество сдвигаемых симовлов равно start2 - start1
        int sdvig = start2 - start1; // na skolko sdvigat
        int nado_sdvinut = len2 - len1;
        for (int i = 0; i < nado_sdvinut; i++)
        {
            char tmp = stroka.A[start2 + len1 + i];
            for (int j = 0; j < sdvig; j++)
            {
                stroka.A[start2 + len1 + i - j] = stroka.A[start2 + len1 + i - j - 1]; // прямо как a[i+1] = a[i] pochti СПРАВА НАЛЕВО
            }
            stroka.A[end1 + 1 + i] = tmp;
        }
    }
    else
    {
        // иначе если слово2 короче слова1 тогда
        // меняем буквы пока позволяет длинна слова2
        for (int i = 0; i < len2; i++)
        {
            char tmp = stroka.A[start1 + i];
            stroka.A[start1 + i] = stroka.A[start2 + i];
            stroka.A[start2 + i] = tmp;
        }
        // после этого по одной переносим оставшиеся буквы слова1  вправо и сдвигаем остаток влево
        int sdvig = end2 - (start1 + len2); // na skolko sdvigat (сколько символов переедет за 1 цикл налево на 1 символ)
        int nado_sdvinut = len1 - len2;     // кол-во перемещаемых элеменотов
        for (int i = 0; i < nado_sdvinut; i++)
        {
            char tmp = stroka.A[start1 + len2];
            for (int j = 0; j < sdvig; j++)
            {
                stroka.A[start1 + len2 + j] = stroka.A[start1 + len2 + j + 1]; // прямо как a[i+1] = a[i] pochti
            }
            stroka.A[end2] = tmp;
        }
    }
}

bool read_fM3(StrM &a1, ofstream &res, std::string filename)
{
    ifstream input;
    input.open(filename, std::ios_base::in);
    if (input.eof())
    {
        cout << " Файл " << filename << " пуст, упс";
        cout << endl;

        res << " Файл " << filename << " пуст, упс";
        res << endl;

        return false;
    }

    int i = 0;
    char s;
    input >> a1.Mark; // первый маркер для нашего массива
    if (input.eof())
    {
        cout << " Файл " << filename << " пуст, упс";
        cout << endl;

        res << " Файл " << filename << " пуст, упс";
        res << endl;

        return false;
    }
    char MMark = -1; // второй маркер для прерывания чтения с файла
    input >> MMark;

    if (input.eof())
    {
        a1.A[0] = a1.Mark;
        cout << "stroka pusta v " << filename << endl;
        res << "stroka pusta v " << filename << endl;
    }
    else
    {
        while (1)
        {
            input >> noskipws >> s;
            if (s == MMark)
                break;
            if (s == '\n')
                break;

            if (input.eof())
                break;

            a1.A[i] = s;
            i++;
            if (i >= N)
            {
                break;
            }
        }
        a1.A[i] = a1.Mark;
    }

    if (a1.A[0] == a1.Mark && MMark == -1)
    {
        cout << "Не хватает 2-го маркера  в файле " << filename;
        cout << endl;

        res << "Не хватает 2-го маркера  в файле " << filename;
        res << endl;
    }
    if (a1.A[0] == a1.Mark && MMark != -1)
    {
        cout << " Строка пуста, 2 маркера есть v  " << filename << endl;
        res << " Строка пуста, 2 маркера есть v  " << filename << endl;
    }
    return true;
}

void process(StrM &a1, ofstream &res)
{
    int k, i = 0;
    int i_start = 0, i_end = 0;
    int j = 0, j_start = 0, j_end = 0;
    int spaceStart = 0, spaceEnd = 0;

    char FirstS2 = 0;

    char LastS1 = 0;

    int z = 0;

    char LastSymbol;
    int spaces = 0;
    int spaces1 = 0;
    k = N;
    while (a1.A[i] != a1.Mark || i < k)
    {
        // again = false;
        if (i == 0 && a1.A[i] != ' ')
        {
            // detectedWord = 0;
            spaces = 0;
            // FirstS1 = a1.A[i];
            i_start = i;

            j = i + 1;
            while (j <= k)
            {
                if ((a1.A[j] == ' ') || a1.A[j] == a1.Mark)
                {
                    spaces++;
                    if (a1.A[j - 1] != ' ')
                    {

                        i_end = j - 1;
                        LastS1 = a1.A[j - 1];
                        spaceStart = j;

                        // while (a1.A[i_end] != ' ')
                        // {
                        //     i_end++;
                        // }
                        // i_end--;
                        spaceStart = i_end + 1;
                        z = spaceStart;

                        while (a1.A[z] == ' ' && a1.A[z] != a1.Mark)
                        {
                            z++;
                        }
                        spaceEnd = z - 1;
                        int tmp = z;
                        while (1)
                        {
                            if (a1.A[z] == a1.Mark)
                                break;
                            if (a1.A[z] == LastS1 && (a1.A[z - 1] == ' '))
                            {
                                j_start = z;
                                j_end = z;
                                break;
                            }

                            if (a1.A[z] != LastS1 && a1.A[z] != a1.Mark)
                            {
                                spaceEnd++;
                                z++;
                            }
                            else
                            {
                                j_start = tmp;
                                j_end = z;
                                break;
                            }
                        }
                        // j_start = z;
                        // j_end = z;
                        if (a1.A[j_start] == a1.Mark)
                        {
                            --j_start;
                            j_end = j_start;
                        }
                        if (j_start > j_end)
                        {
                            j_start = j_end;
                        }
                        FirstS2 = a1.A[j_start];
                        while (a1.A[z] != ' ' && a1.A[z] != a1.Mark)
                        {
                            j_end++;
                            z++;
                        }
                        j_end--;
                        if (j_start > j_end)
                        {
                            j_start = j_end;
                        }
                        if (spaceEnd < j_start)
                        {
                            inplace_swap_slova(a1, spaceStart, spaceEnd, j_start, j_end);
                        }
                    }
                }
                j++;
            }
        }
        i++;
    }
    cout << "Результат:" << endl;
    res << "Результат:" << endl;
    print(a1, res);
}

int file_process_1(string filename, StrM s, ofstream &res)
{

    string filename2 = "result.txt";
    // ofstream res(filename2, ios::out | ios::trunc);
    cout << "Добро пожаловать в lab 1.1.1 3352 " << endl;
    res << "Добро пожаловать в lab 1.1.1 3352 " << endl;
    cout << "Исходник: " << endl;
    res << endl
        << "Исходник: " << endl;

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
        cout << endl;
        res << endl;
        process(s, res);
        // print(s, res);

        cout << "Обработка файла " << filename << " завершена " << endl;
        res << "Обработка файла " << filename << " завершена " << endl;
        return 0;
    }
}
////////////////////////////////////////////////////////////////////1.1.2 Len

void print2(StrL stroka, ofstream &res)
{
    int i = 0;

    while (stroka.A[i] != stroka.Mark || i < stroka.len)
    {
        cout << stroka.A[i];
        res << stroka.A[i];
        i++;
    }
    res << endl;
    cout << endl;
}
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

    input >> stroka.Mark; //  маркер для нашего массива

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

        stroka.A[0] = stroka.Mark;
    }
    else
    {
        while (1)
        {
            if (i >= N || i >= stroka.len) // i >= ?
            {
                // stroka.A[i + 1] = stroka.Mark; //
                break;
            }
            input >> noskipws >> s;
            if (s == '\n')
                break;

            if (input.eof())
                break;
            if (s == stroka.Mark)
                break;

            stroka.A[i] = s;
            i++;
            // if (i >= N || i >= stroka.len) // i >= ?
            // {
            //     // stroka.A[i + 1] = stroka.Mark; //
            //     break;
            // }
        }
        if (stroka.len > i)
        {
            cout << " реальная длина меньше заданной " << endl;
            res << " реальная длина меньше заданной " << endl;
            stroka.len = i;
            // stroka.A[i + 1] = stroka.Mark;
        }

        stroka.A[i] = stroka.Mark; //
    }
    if (stroka.A[0] == stroka.Mark)
    {
        cout << "stroka pusta v " << filename << endl;
        res << "stroka pusta v " << filename << endl;
    }

    return true;
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
            char tmp = stroka.A[start1 + i];
            stroka.A[start1 + i] = stroka.A[start2 + i];
            stroka.A[start2 + i] = tmp;
        }
        // после этого по одной переносим оставшиеся буквы слова два влево и сдвигаем остаток вправо
        // количество сдвигаемых симовлов равно start2 - start1
        int sdvig = start2 - start1; // na skolko sdvigat
        int nado_sdvinut = len2 - len1;
        for (int i = 0; i < nado_sdvinut; i++)
        {
            char tmp = stroka.A[start2 + len1 + i];
            for (int j = 0; j < sdvig; j++)
            {
                stroka.A[start2 + len1 + i - j] = stroka.A[start2 + len1 + i - j - 1]; // прямо как a[i+1] = a[i] pochti СПРАВА НАЛЕВО
            }
            stroka.A[end1 + 1 + i] = tmp;
        }
    }
    else
    {
        // иначе если слово2 короче слова1 тогда
        // меняем буквы пока позволяет длинна слова2
        for (int i = 0; i < len2; i++)
        {
            char tmp = stroka.A[start1 + i];
            stroka.A[start1 + i] = stroka.A[start2 + i];
            stroka.A[start2 + i] = tmp;
        }
        // после этого по одной переносим оставшиеся буквы слова1  вправо и сдвигаем остаток влево
        int sdvig = end2 - (start1 + len2); // na skolko sdvigat (сколько символов переедет за 1 цикл налево на 1 символ)
        int nado_sdvinut = len1 - len2;     // кол-во перемещаемых элеменотов
        for (int i = 0; i < nado_sdvinut; i++)
        {
            char tmp = stroka.A[start1 + len2];
            for (int j = 0; j < sdvig; j++)
            {
                stroka.A[start1 + len2 + j] = stroka.A[start1 + len2 + j + 1]; // прямо как a[i+1] = a[i] pochti
            }
            stroka.A[end2] = tmp;
        }
    }
}

void process2(StrL &a1, ofstream &res)
{
    int k, i = 0;
    int i_start = 0, i_end = 0;
    int j = 0, j_start = 0, j_end = 0;
    int spaceStart = 0, spaceEnd = 0;

    char FirstS2 = 0;

    char LastS1 = 0;

    int z = 0;

    char LastSymbol;
    int spaces = 0;
    int spaces1 = 0;
    k = N;
    while (i < a1.len)
    {
        // again = false;
        if (i == 0 && a1.A[i] != ' ')
        {
            // detectedWord = 0;
            spaces = 0;
            // FirstS1 = a1.A[i];
            i_start = i;

            j = i + 1;
            while (j <= a1.len)
            {
                if ((a1.A[j] == ' ') || j == a1.len)
                {
                    spaces++;
                    if (a1.A[j - 1] != ' ')
                    {

                        i_end = j - 1;
                        LastS1 = a1.A[j - 1];
                        spaceStart = j;

                        spaceStart = i_end + 1;
                        z = spaceStart;

                        while (a1.A[z] == ' ' && z < a1.len)
                        {
                            z++;
                        }
                        spaceEnd = z - 1;
                        int tmp = z;
                        while (1)
                        {
                            if (z == a1.len)
                                break;
                            if (a1.A[z] == LastS1 && (a1.A[z - 1] == ' '))
                            {
                                j_start = z;
                                j_end = z;
                                break;
                            }

                            if (a1.A[z] != LastS1 && z < a1.len)
                            {
                                spaceEnd++;
                                z++;
                            }
                            else
                            {
                                j_start = tmp;
                                j_end = z;
                                break;
                            }
                        }
                        // j_start = z;
                        // j_end = z;
                        if (j_start == a1.len)
                        {
                            --j_start;
                            j_end = j_start;
                        }
                        if (j_start > j_end)
                        {
                            j_start = j_end;
                        }
                        FirstS2 = a1.A[j_start];
                        while (a1.A[z] != ' ' && z < a1.len)
                        {
                            j_end++;
                            z++;
                        }
                        j_end--;
                        if (j_start > j_end)
                        {
                            j_start = j_end;
                        }
                        if (spaceEnd < j_start)
                        {
                            inplace_swap_slova2(a1, spaceStart, spaceEnd, j_start, j_end);
                        }
                    }
                }
                j++;
            }
        }
        i++;
    }
    cout << "Результат:" << endl;
    res << "Результат:" << endl;
    print2(a1, res);
}

int file_process_2(string filename, StrL s, ofstream &res)
{

    // string filename2 = "result.txt";
    // ofstream res(filename2, ios::out | ios::trunc);
    cout << "Добро пожаловать в lab 1.1.2  " << endl;
    res << "Добро пожаловать в lab 1.1.2  " << endl;

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

        process2(s, res);
        // print2(s, res);

        cout << "Обработка файла " << filename << " завершена (2)" << endl;
        res << "Обработка файла " << filename << " завершена (2)" << endl;
        return 0;
    }
}
////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    string filename2 = "result3.txt";
    // string filename1 = "in1_1m.txt";
    ofstream res(filename2, ios::out | ios::trunc);
    // cout << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;

    StrM s1;
    file_process_1("in1.txt", s1, res);
    cout << endl
         << endl
         << endl;

    res << endl
        << endl
        << endl;

    StrM s2;
    file_process_1("in2.txt", s2, res);
    cout << endl
         << endl
         << endl;

    res << endl
        << endl
        << endl;

    StrM s3;
    file_process_1("in3.txt", s3, res);
    cout << endl
         << endl
         << endl;

    res << endl
        << endl
        << endl;

    StrM s4;
    file_process_1("in4.txt", s4, res);
    cout << endl
         << endl
         << endl;

    res << endl
        << endl
        << endl;

    StrM s5;
    file_process_1("in5.txt", s5, res);
    cout << endl
         << endl
         << endl;

    res << endl
        << endl
        << endl;
    //////////////////////////////////////////1.1.2
    StrL s11;
    file_process_2("in11.txt", s11, res);
    cout << endl
         << endl
         << endl;

    res << endl
        << endl
        << endl;

    StrL s12;
    file_process_2("in12.txt", s12, res);
    cout << endl
         << endl
         << endl;

    res << endl
        << endl
        << endl;

         StrL s13;
    file_process_2("in13.txt", s13, res);
    cout << endl
         << endl
         << endl;

    res << endl
        << endl
        << endl;


         StrL s14;
    file_process_2("in14.txt", s14, res);
    cout << endl
         << endl
         << endl;

    res << endl
        << endl
        << endl;
}