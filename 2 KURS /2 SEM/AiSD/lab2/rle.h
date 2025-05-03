#pragma once

#include "downsampling.h"
#include "diff_coding.h"

// это специальное РЛЕ для АС коэффициентов
template <typename T>
void rle_AC(std::vector<T> &input, std::vector<T> &output)
{
    //     Пример:
    // 0xCF 0xCF 0xCF 0xCF 0xCF -> 0x05 0xCF
    if (!input.size())
        return;
    T prev = input[0];
    T counter = 0;
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

template <typename T>
void rev_rle_AC(std::vector<T> &input, std::vector<T> &output)
{
    for (int i = 0; i < input.size(); i += 2)
    {
        output.insert(output.end(), input[i], input[i + 1]); // вставляем такое-то кол-во input[i] символов input[i+1]
    }
}