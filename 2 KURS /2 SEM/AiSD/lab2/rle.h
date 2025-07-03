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

// это специальное РЛЕ для АС коэффициентов
#include <iostream>
#include <vector>

// Функция для кодирования RLE
std::vector<std::pair<int, int>> rle_encode(const std::vector<int> &ac_coefficients)
{
    // Если входной вектор пуст, возвращаем пустой вектор
    if (ac_coefficients.empty())
    {
        return {};
    }

    std::vector<std::pair<int, int>> encoded;
    int count = 0;

    for (size_t i = 0; i < ac_coefficients.size(); ++i)
    {
        int value = ac_coefficients[i];

        if (value == 0)
        {

            bool all_zeros = true;
            for (size_t j = i; j < ac_coefficients.size(); ++j)
            {
                if (ac_coefficients[j] != 0)
                {
                    all_zeros = false;
                    break;
                }
            }

            if (all_zeros)
            {
                encoded.push_back(std::make_pair(0, 0)); // EOB (End of Block)
                return encoded;
            }
            count++;
        }
        else
        {
            // Если count == 0, то добавляем (0, value)
            if (count == 0)
            {
                encoded.push_back(std::make_pair(0, value));
            }
            else
            {
                // Если count > 0, обрабатываем последовательность нулей
                while (count > 0)
                {
                    if (count > 15)
                    {
                        encoded.push_back(std::make_pair(15, 0)); // ZRL (Zero Run Length)
                        count -= 16;                              // Уменьшаем счетчик на 16, т.к. 15 мы уже закодировали
                    }
                    else
                    {
                        encoded.push_back(std::make_pair(count, value)); // Кодируем счетчик и значение
                        count = 0;                                       // Сбрасываем счетчик
                    }
                }
            }
        }
    }

    return encoded;
}

std::vector<std::vector<int>> rle_decode(const std::vector<std::vector<std::pair<int, int>>> &encoded_blocks)
{
    std::vector<std::vector<int>> decoded_blocks;

    for (const auto &encoded : encoded_blocks)
    {
        std::vector<int> decoded;
        for (const auto &pair : encoded)
        {
            int count = pair.first;
            int value = pair.second;

            if (count == 0 && value == 0)
            {
                // EOB (End of Block): Заполняем оставшуюся часть блока нулями
                decoded.insert(decoded.end(), 63 - decoded.size(), 0); // Эквивалентно decoded.extend([0] * (63 - len(decoded)))
                goto next_encoded;                                     // Переходим к следующему закодированному блоку
            }

            if (count > 0)
            {
                // Добавляем 'count' нулей
                decoded.insert(decoded.end(), count, 0); // Эквивалентно decoded.extend([0] * count)
            }

            decoded.push_back(value); // Добавляем значение
        }

        // Если длина декодированного блока меньше 63, заполняем нулями
        if (decoded.size() < 63)
        {
            decoded.insert(decoded.end(), 63 - decoded.size(), 0); // Эквивалентно decoded.extend([0] * (63 - len(decoded)))
        }

        decoded_blocks.push_back(decoded); // Добавляем декодированный блок в результирующий вектор

    next_encoded:; // Метка для оператора goto
    }

    return decoded_blocks;
}
