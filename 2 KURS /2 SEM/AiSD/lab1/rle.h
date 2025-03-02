#pragma once

#include <vector>
#include <cstring>

//первое задание 2.1
void rle_compress_2_1(const std::vector<unsigned char> &input, std::vector<unsigned char> &output);
void rle_decompress_2_1(const std::vector<unsigned char> &input, std::vector<unsigned char> &output);

//второе задание 2.2
void rle_compress_2_2(const std::vector<unsigned char> &input, std::vector<unsigned char> &output);
void rle_decompress_2_2(const std::vector<unsigned char> &input, std::vector<unsigned char> &output);

// 2.3
//параметр М в битах (1 символ = 1 Байт = 8 ьит) это сколько чарсимволов РАЗНЫХ
// может быть в повторяющейся последовательности
template <int M>
void rle_compress_2_3(const std::vector<unsigned char> &input, std::vector<unsigned char> &output)
{
    // ставим константу нашего размера символа (последовательность символов которая повторяется)
    //как кол-во байт (для этого делим М на 8)
    static const int symbol_size = M/__CHAR_BIT__;

    //обозначаем новый тип СИМВОЛ который является массивом из  symbol_size байт
    using SYMBOL = unsigned char[symbol_size];
    //     Пример:
    // 0xCF 0xCF 0xCF 0xCF 0xCF -> 0x05 0xCF
    if (!input.size())
        return;

    auto SYMBOL_read = [&](SYMBOL & sym, const unsigned char & read_from) 
    {
        std::memcpy(&sym[0],&read_from, sizeof(SYMBOL));
    };
    auto SYMBOL_compare = [&](SYMBOL & sym, const unsigned char & compare_to)
    {
        return !std::memcmp(&sym[0],&compare_to, sizeof(SYMBOL));
    };
    auto SYMBOL_write = [&](SYMBOL & sym, unsigned char & write_to)
    {
        std::memcpy(&write_to, &sym[0], sizeof(SYMBOL));
    };

    SYMBOL prev;
    SYMBOL_read(prev, input[0]);

    unsigned char counter = 0;
    int i = 0;
    while (i < input.size())
    {
        if (SYMBOL_compare(prev, input[i])) // если символы повторяются
        {
            counter++;
            if (counter == 255) // если превысили размер чара (1 байт)
            {
                output.push_back(254); // выписать 255 с текущим символом и продолжить
                output.resize(output.size() + symbol_size);
                SYMBOL_write(prev, output[output.size() - symbol_size]);
                counter = 1;
            }
        }
        else // если последовательность повторяющихся кончилась
        {
            output.push_back(counter);
            output.resize(output.size() + symbol_size);
            SYMBOL_write(prev, output[output.size() - symbol_size]);
            SYMBOL_read(prev, input[i]);
            counter = 1;
        }
        i += symbol_size;
    }
    output.push_back(counter);
    output.resize(output.size() + symbol_size);
    SYMBOL_write(prev, output[output.size() - symbol_size]);
}

template <int M>
void rle_decompress_2_3(const std::vector<unsigned char> &input, std::vector<unsigned char> &output)
{
    static const int symbol_size = M/__CHAR_BIT__;
    //обозначаем новый тип СИМВОЛ который является массивом из  symbol_size байт
    using SYMBOL = unsigned char[symbol_size];

    auto SYMBOL_read = [&](SYMBOL & sym, const unsigned char & read_from) 
    {
        std::memcpy(&sym[0],&read_from, sizeof(SYMBOL));
    };
    auto SYMBOL_compare = [&](SYMBOL & sym, const unsigned char & compare_to)
    {
        return !std::memcmp(&sym[0],&compare_to, sizeof(SYMBOL));
    };
    auto SYMBOL_write = [&](SYMBOL & sym, unsigned char & write_to)
    {
        std::memcpy(&write_to, &sym[0], sizeof(SYMBOL));
    };
    for (int i = 0; i < input.size(); i += 1 /*1 для счетчика*/ + symbol_size)
    {
        SYMBOL next;
        SYMBOL_read(next, input[i+1]);
        int start = output.size();
        output.resize(output.size() + input[i]*symbol_size);
        // вставляем такое-то кол-во input[i] символов input[i+1]
        for (int j = 0; j < input[i]; ++ j) 
        {
            SYMBOL_write(next, output[start + j*symbol_size]);
        }
    }
}

// пример: abcabcabc ---> M =24 бит---> 3 "abc" (повторябщийся символ это "abc" занимает 3 байта или 24 бита)
// М надо найти самим в процесее