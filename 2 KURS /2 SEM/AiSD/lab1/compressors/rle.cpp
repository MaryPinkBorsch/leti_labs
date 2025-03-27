#include "rle.h"

using namespace std;

void rle_compress_2_1(const std::vector<unsigned char> &input, std::vector<unsigned char> &output)
{
    //     Пример:
    // 0xCF 0xCF 0xCF 0xCF 0xCF -> 0x05 0xCF
    if (!input.size())
        return;
    unsigned char prev = input[0];
    unsigned char counter = 0;
    int i = 0;
    while (i < input.size())
    {
        if (input[i] == prev) // если символы повторяются
        {
            counter++;
            if (counter == 255) // если превысили размер чара (1 байт)
            {
                output.push_back(254); // выписать 255 с текущим символом и продолжить
                output.push_back(prev);
                counter = 1;
            }
        }
        else // если последовательность повторяющихся кончилась
        {
            output.push_back(counter);
            output.push_back(prev);
            prev = input[i];
            counter = 1;
        }
        i++;
    }
    output.push_back(counter);
    output.push_back(prev);
}
void rle_decompress_2_1(const std::vector<unsigned char> &input, std::vector<unsigned char> &output)
{
    for (int i = 0; i < input.size(); i += 2)
    {
        output.insert(output.end(), input[i], input[i + 1]); // вставляем такое-то кол-во input[i] символов input[i+1]
    }
}

// 2.2 //////////////////////////////////////////////////////////////////////////////

// неповторяющиеся символы кодируются отрицательными!!! чарами, а повторяющиеся - положительными
//(у отрицательных старший бит это 1)

// Входные данные: 1 2 3 4 5 6 7 7 7 7 7 -> -6 "1 2 3 4 5 6" 5 "7" (в кавычках наши символы)

void rle_compress(const std::vector<char> &input, std::vector<char> &output)
{
    //     Пример:
    // Входные данные: 1 2 3 4 5 6 7 7 7 7 7 -> -6 "1 2 3 4 5 6" 5 "7"
    if (!input.size())
        return;

    if (input.size() == 1)
    {
        output.push_back(1);
        output.push_back(input[0]);
        return;
    }
    char prev = input[0];
    // int j = -1; // индекс с которого начинается последовательность НЕповторяющихся символов
    std::vector<char> no_repeat;
    char counter = 0;  // повторяющиеся символы
    char counter2 = 0; // неповторяющиеся символы
    int i = 0;
    bool repeated = false;
    // 1) идти по инпут
    // 2) вести 2 счетчика
    // 3) при смене неповторяющихся и поовторяющийся выводить то что есть в оутпут
    // 4) следить за выходом счетчика за пределы чар ?
    // 5) флаг на некповторяющиеся последовательности

    if (prev == input[i + 1])
    {
        counter = 1;
        prev = input[i];
        repeated = true;
        i++;
    }
    else
    {
        no_repeat.push_back(input[0]);
        counter2 = 1;
        prev = input[i];
        i++;
    }

    while (i < input.size())
    {
        if (prev == input[i])
        {
            if (repeated == false && !no_repeat.empty() && no_repeat.back() == input[i])
            {
                counter2--;
                no_repeat.pop_back();
            }
            if (repeated == false && !no_repeat.empty())
            {
                // выкинуть в аутпут то что есть
                int tmp = 0;
                // если последовательность больше 127 исмволов выписываем кусками
                while (no_repeat.size() - tmp >= 127)
                {
                    output.push_back(-127);
                    for (int k = 0; k < 128; k++)
                    {
                        output.push_back(no_repeat[k + tmp]);
                    }
                    tmp += 127;
                }
                // выписываем все что осталось
                int j = no_repeat.size() - tmp;
                output.push_back(-1 * j);
                for (int k = 0; k < j; k++)
                {
                    output.push_back(no_repeat[k + tmp]);
                }
                no_repeat.clear();
                counter2 = 0;
            }

            if (counter == 127) // если превысили размер чара
            {
                output.push_back(126); // выписать 126 с текущим символом и продолжить
                output.push_back(prev);
                counter = 1;
            }
            counter++;
            repeated = true;
        }
        else
        {
            if (repeated)
            {
                output.push_back(counter);
                output.push_back(prev);
            }
            repeated = false;
            counter = 1;
            prev = input[i];
            no_repeat.push_back(input[i]);
            counter2++;
        }
        i++;
    }
    if (repeated)
    {
        output.push_back(counter);
        output.push_back(prev);
    }
    else if (!no_repeat.empty())
    {
        // выкинуть в аутпут то что есть
        int tmp = 0;
        // если последовательность больше 127 исмволов выписываем кусками
        while (no_repeat.size() - tmp >= 127)
        {
            output.push_back(-127);
            for (int k = 0; k < 128; k++)
            {
                output.push_back(no_repeat[k + tmp]);
            }
            tmp += 127;
        }
        // выписываем все что осталось
        int j = no_repeat.size() - tmp;
        output.push_back(-1 * j);
        for (int k = 0; k < j; k++)
        {
            output.push_back(no_repeat[k + tmp]);
        }
        no_repeat.clear();
        counter2 = 0;
    }
}
void rle_decompress(const std::vector<char> &input, std::vector<char> &output)
{
    int t = 0;
    output.reserve(input.size());
    while (t < input.size())
    {
        char count = input[t];
        if (count > 0)
        {
            output.insert(output.end(), count, input[t + 1]); // вставляем такое-то кол-во input[i] символов input[i+1]
            t += 2;
        }
        else
        {
            count *= -1;
            for (int i = t + 1; i < t + count + 1; i++)
            {
                output.push_back(input[i]);
            }
            t += (count + 1);
        }
    }
}

// 2.3 ////////////////////
