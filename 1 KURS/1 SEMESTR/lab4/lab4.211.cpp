#include <iostream>
#include <fstream>

using namespace std;

void StaticArrayFromFileStream(std::string filename1, std::string filename2)
{
    // результат работы выводится в файл
    ofstream result_stream;
    result_stream.open("result_static.txt", std::ios_base::app);
    result_stream << "После обработки " << filename1 << " и " << filename2;

    // массив находится на стеке и имеет максимальный размер
    static const int MAX_ARRAY_SIZE = 1024;
    float A[MAX_ARRAY_SIZE];

    // Эта переменная будет содержать фактическую длинну массива после чтения из файла
    int A_len = 0;

    ifstream massiv1;
    massiv1.open(filename1, std::ios_base::in);
    // TODO: тут проверить что файл открылся gotovo
    if (massiv1.eof())
    {
        std::cout << " Файл 1 пуст, упс" << endl;
        result_stream << " Файл 1 пуст, упс" << endl;
        return;
    }
    // сперва считаем количество элементов
    std::string tmp;
    getline(massiv1, tmp);
    if (tmp.empty())
    {
        std::cout << "Файл пуст, упс" << endl;
        result_stream << " Файл пуст, упс" << endl;
        return;
    }
    A_len = atoi(tmp.c_str());
    if (A_len < 0)
    {
        std::cout << "Отрицательных количеств не бывает! Будем считать что это ноль";
        A_len = 0;
        std::cout << "Файл 1 пуст, упс" << endl;
        result_stream << " Файл 1 пуст, упс" << endl;
        return;
    }
    if (A_len == 0)
    {
        std::cout << "Файл 1 пуст, упс" << endl;
        result_stream << " Файл 1 пуст, упс" << endl;
        return;
    }
    if (A_len > MAX_ARRAY_SIZE)
    {
        std::cout << "Похоже в файле больше чисел чем максимальное число элементов в нашем массиве, считаем сколько можем" << endl;
        A_len = MAX_ARRAY_SIZE;
    }
    // result_stream << "Массив 1 статика: " << endl;
    for (int k = 0; k < A_len; k++)
    {
        getline(massiv1, tmp);
        // TODO: тут проверить что строчка не пустая gotov
        if (tmp.empty())
        {
            std::cout << "строка пуста, упс" << endl;
            result_stream << " строка пуста, упс" << endl;
            return;
        }
        A[k] = atof(tmp.c_str());

        //  result_stream << A[k] << endl;
    }
    massiv1.close();

    float B[MAX_ARRAY_SIZE];
    int B_len = 0;

    ifstream massiv2;
    massiv2.open(filename2, std::ios_base::in);
    // TODO: тут проверить что файл открылся gotovo
    if (massiv2.eof())
    {
        std::cout << "Файл 2 пуст, упс" << endl;
        result_stream << " Файл 2 пуст, упс" << endl;
        return;
    }
    // сперва считаем количество элементов
    getline(massiv2, tmp);
    if (tmp.empty())
    {
        std::cout << "Файл пуст, упс" << endl;
        result_stream << " Файл пуст, упс" << endl;
        return;
    }
    B_len = atoi(tmp.c_str());
    if (B_len < 0)
    {
        std::cout << "Отрицательных количеств не бывает! Будем считать что это ноль" << endl;
        B_len = 0;
        std::cout << "Файл 2 пуст, упс" << endl;
        result_stream << "  Файл 2 пуст, упс" << endl;
        return;
    }
    if (B_len == 0)
    {
        std::cout << "Файл 2 пуст, упс" << endl;
        result_stream << " Файл 2 пуст, упс" << endl;
        return;
    }
    if (B_len > MAX_ARRAY_SIZE)
    {
        std::cout << "Похоже в файле больше чисел чем максимальное число элементов в нашем массиве, считаем сколько можем" << endl;
        B_len = MAX_ARRAY_SIZE;
    }

    if (A_len == B_len)
    {
        std::cout << "Массивы не должны быть одного размера!" << endl;
        result_stream << "  Массивы не должны быть одного размера!" << endl;
        return;
    }

    for (int k = 0; k < B_len; k++)
    {
        getline(massiv2, tmp);
        // TODO: тут проверить что строчка не пустая gotov
        if (tmp.empty())
        {
            std::cout << "строка пуста, упс" << endl;
            result_stream << " строка пуста, упс" << endl;
            return;
        }
        B[k] = atof(tmp.c_str());
    }
    massiv2.close();

    // вычисляем ответ
    int result = 0;
    for (int i = 0; i < A_len; i++)
    {
        for (int j = 0; j < B_len; j++)
        {
            if (A[i] * B[j] > 0)
            {
                result++;
            }
        }
    }

    result_stream << " результат с статич. массивами составил: " << result << std::endl;
    result_stream << " Массив 1: "
                  << "(Длинa " << A_len << ")" << endl;
    for (int k = 0; k < A_len; k++)
    {

        result_stream << A[k] << endl;
    }
    result_stream << endl;
    result_stream << " Массив 2: "
                  << "(Длинa " << B_len << ")" << endl;
    for (int k = 0; k < B_len; k++)
    {

        result_stream << B[k] << endl;
    }
    result_stream << endl;
    result_stream << endl;
    std::cout << result << " (result static + file)" << endl;
}

void DynamicArrayFromFileStream(std::string filename1, std::string filename2)
{
    // вывод результатов в файл
    ofstream result_stream;
    result_stream.open("result_dynamic.txt", std::ios_base::app);

    // массив находятся в динамически выделяемой памяти
    int *A = nullptr;
    int A_len = 0;

    // надо прочитать файл два раза, один раз чтобы выяснить
    // количество строчек в нем и выделить динамический массив нужного размера
    // а потом второй раз чтобы считать числа

    ifstream massiv1;
    massiv1.open(filename1.c_str(), std::ios_base::in);
    // TODO: тут проверить что файл открылся gotovo
    if (massiv1.eof())
    {
        std::cout << "Файл 1 пуст, упс";
        result_stream << "Файл 1 пуст, упс" << std::endl;
        return;
    }

    // считаем количество строк в файле
    std::string tmp;

    while (getline(massiv1, tmp))
        ++A_len;
    A = new int[A_len]; // динамически выделяем память под массив из floatov длинной A_len и результат сохраняем в A
                        // result_stream << "После обработки " << filename1 << "Кол-во элементов в масиве" << A_len;

    if (A == nullptr)
    {
        std::cout << "Память не выделилась, упс";
        result_stream << "Память не выделилась, упс" << std::endl;
        return;
    }

    // теперь файл надо "перемотать"
    massiv1.close();
    massiv1.open(filename1.c_str(), std::ios_base::in);

    int counter = 0;
    int tmp1 = 0;
    int *p_a = A;
    while (!massiv1.eof())
    {
        massiv1 >> tmp1;
        *p_a = tmp1;
        ++p_a;
    }
    /*
      while (getline(massiv1, tmp))
         A[counter++] = atoi(tmp.c_str());
     */

    massiv1.close();

    int *B = nullptr;
    int B_len = 0;
    ifstream massiv2;
    massiv2.open(filename2.c_str(), std::ios_base::in);
    // TODO: тут проверить что файл открылся gotovo
    if (massiv2.eof())
    {
        std::cout << "Файл 2 пуст, упс";
        result_stream << "Файл 2 пуст, упс" << std::endl;
        return;
    }
    ////////////////////////////////////////////////////tut ya pitays schitat norm massiv
    /*
         while (massiv2.is_open())
         {
             ++B_len;
         }*/

    ////////////////////////////////////////////////////////

    while (getline(massiv2, tmp))
        ++B_len;

    B = new int[B_len]; // динамически выделяем память под массив из интов длинной B_len и результат сохраняем в B

    if (B == nullptr)
    {
        std::cout << "Память не выделилась, упс";
        result_stream << "Память не выделилась, упс" << std::endl;
        return;
    }
    // теперь файл надо "перемотать"
    massiv2.close();
    massiv2.open(filename2.c_str(), std::ios_base::in);

    counter = 0;

    int tmp2 = 0;
    int *p_b = B;
    while (!massiv2.eof())
    {
        massiv2 >> tmp2;
        *p_b = tmp2;
        ++p_b;
    }
    /*
         while (getline(massiv2, tmp))
            B[counter++] = atoi(tmp.c_str());
        massiv2.close();
        */
    if (A_len == B_len)
    {
        std::cout << "Массивы не должны быть одного размера!" << endl;
        result_stream << "После обработки " << filename1 << " и " << filename2 << "  Массивы не должны быть одного размера!" << endl;
        return;
    }

    int result = 0;
    p_a = A;
    p_b = B;
    for (int i = 0; i < A_len; i++)
    {
        for (int j = 0; j < B_len; j++)
        {
            if ((*p_a) * (*p_b) > 0) // то же самое было бы if ((*(A + i))) * (*(B + j))) > 0)
            {
                result++;
            }
            ++p_b;
        }
        ++p_a;
    }

    // любую память\ресурс которая была выделена надо освободить (чтобы избежать утечек памяти)

    std::cout << result << " (result dynamic + file)" << endl;

    result_stream << "После обработки " << filename1 << " и " << filename2 << " результат с динамич. массивами составил: " << result << std::endl;

    result_stream << " Массив 1: "
                  << "(Длинa " << A_len << ")" << endl;
    for (int k = 0; k < A_len; k++)
    {

        result_stream << A[k] << endl;
    }
    result_stream << endl;
    result_stream << " Массив 2: "
                  << "(Длинa " << B_len << ")" << endl;

    int *pB = B;
    for (int k = 0; k < B_len; k++)
    {
        result_stream << *(pB + k) << endl;
        // result_stream << B[k] << endl; :/
    }
    result_stream << endl;
    result_stream << endl;
    delete[] A; // delete[] для удаления памяти выделенной под массив (а не под один инт)
    delete[] B;
}

int main(int argc, char *argv[])
{
    // в начале работы перезапишем файлы с результатами
    {
        ofstream result_stream;
        result_stream.open("result_static.txt", std::ios_base::trunc);
    }
    {
        ofstream result_stream;
        result_stream.open("result_dynamic.txt", std::ios_base::trunc);
    }
    StaticArrayFromFileStream("static_array1.txt", "static_array2.txt");
    StaticArrayFromFileStream("static_array2.txt", "static_array1.txt");
    StaticArrayFromFileStream("static_array1.txt", "tests1.txt");
    StaticArrayFromFileStream("static_array1.txt", "tests2.txt");
    StaticArrayFromFileStream("tests1.txt", "tests2.txt");
    StaticArrayFromFileStream("tests1.txt", "static_array2.txt");
    StaticArrayFromFileStream("static_array1.txt", "tests3.txt");
    StaticArrayFromFileStream("static_array2.txt", "static_array2.txt");
    StaticArrayFromFileStream("static_array1.txt", "tests4.txt");
    StaticArrayFromFileStream("static_array1.txt", "tests5.txt");
    StaticArrayFromFileStream("static_array1.txt", "tests6.txt");
    StaticArrayFromFileStream("static_array1.txt", "tests7.txt");
    StaticArrayFromFileStream("static_array1.txt", "tests8.txt");
    StaticArrayFromFileStream("tests7.txt", "tests7.txt");
    StaticArrayFromFileStream("tests7.txt", "tests8.txt");
    StaticArrayFromFileStream("static_array1.txt", "tests9.txt");
    StaticArrayFromFileStream("tests9.txt", "tests11.txt");

    /////////////////////////////////////////////////////////////////

    DynamicArrayFromFileStream("dynamic_array1.txt", "dynamic_array2.txt");
    DynamicArrayFromFileStream("dynamic_array1.txt", "testd1.txt");
    DynamicArrayFromFileStream("dynamic_array1.txt", "testd2.txt");
    DynamicArrayFromFileStream("testd2.txt", "dynamic_array1.txt");

    DynamicArrayFromFileStream("testd6.txt", "dynamic_array1.txt");
    DynamicArrayFromFileStream("testd7.txt", "testd7.txt");
    DynamicArrayFromFileStream("testd1.txt", "dynamic_array2.txt");
    DynamicArrayFromFileStream("testd11.txt", "dynamic_array2.txt");

    return 0;
}

// Массив A: 1 2 3 4 5 6 6 -1 -2 -3
// индекс этого массива примем за i

// Массив B: -1 -2 -3 -5 5 4 3 2 1
// индекс этого массива примем за j

// От нас нужно найти количество всех пар индексов i и j таких что A[i] и B[j] имеют одинаковый знак

//-----------------------------------------

/*

// ЭТО БАЗА ДЛЯ СТАТИЧЕСКОГО МАССИВА
void StaticArray()
{
    // если надо считывать из файлов в статический массив надо заранее завести массив большого размера и
    // считывать туда только в том случае если количество элементов в файле меньше или равно (влезает)
    // int MAX_ELEMENTS = 1024;
    // int A[MAX_ELEMENTS]

    // массивы находятся на стеке!
    int A[10] = {1, 2, 3, 4, 5, -5, -6, -7, -8, -9};
    // количество элементов в статическом массиве это его размер деленный на размер одного элемента
    int A_l = sizeof(A) / sizeof(int);
    int B[8] = {2, 2, -2, -2, -3, 4, 4, 5};
    int B_l = sizeof(B) / sizeof(int);

    int result = 0;
    for (int i = 0; i < A_l; i++)
    {
        for (int j = 0; j < B_l; j++)
        {
            if (A[i] * B[j] > 0)
            {
                result++;
            }
        }
    }
    cout << result << " result basic static" << endl;
}
// ЭТО БАЗА ДЛЯ ДИНАМИЧЕСКОГО МАССИВА
void DynamicArray()
{
    // массивы находятся в динамически выделяемой памяти
    int *A = nullptr;
    // количество элементов в динамическом массиве
    int A_l = 10;
    A = new int[A_l]; // динамически выделяем память под массив из интов длинной A_l и результат сохраняем в A
    A[0] = 1;
    A[1] = 2;
    A[2] = 3;
    A[3] = 4;
    A[4] = 5;
    A[5] = -5;
    A[6] = -6;
    A[7] = -7;
    A[8] = -8;
    A[9] = -9;

    int *B = nullptr;
    int B_l = 8;
    B = new int[B_l]; // динамически выделяем память под массив из интов длинной B_l и результат сохраняем в B
    B[0] = 2;
    B[1] = 2;
    B[2] = -2;
    B[3] = -2;
    B[4] = -3;
    B[5] = 4;
    B[6] = 4;
    B[7] = 5;

    int result = 0;
    for (int i = 0; i < A_l; i++)
    {
        for (int j = 0; j < B_l; j++)
        {
            if (A[i] * B[j] > 0)
            {
                result++;
            }
        }
    }
    // любую память\ресурс которая была выделена надо освободить (чтобы избежать утечек памяти)
    delete[] A; // delete[] для удаления памяти выделенной под массив (а не под один инт)
    delete[] B;

    cout << result << endl;
}

*/