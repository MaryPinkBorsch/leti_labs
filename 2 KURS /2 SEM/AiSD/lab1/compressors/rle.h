#pragma once

#include <vector>
#include <cstring>

// второе задание 2.2 - будет использоваться как основной вариант RLE для лабораторной
// он позволяет кодировать кодировать как повторяющиеся символы так и последовательности неповторяющихся симоволов
void rle_compress(const std::vector<char> &input, std::vector<char> &output);
void rle_decompress(const std::vector<char> &input, std::vector<char> &output);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// остальные варианты останутся на память
// первое задание 2.1
void rle_compress_2_1(const std::vector<unsigned char> &input, std::vector<unsigned char> &output);
void rle_decompress_2_1(const std::vector<unsigned char> &input, std::vector<unsigned char> &output);

// 2.3
// параметр М в битах (1 символ = M/8 Байт = M бит) это сколько битов в одном символе
template <int M>
void rle_compress_2_3(const std::vector<unsigned char> &input, std::vector<unsigned char> &output)
{
    // ставим константу нашего размера символа (последовательность символов которая повторяется)
    // как кол-во байт (для этого делим М на 8)
    static const int SYMBOL_SIZE = M / __CHAR_BIT__;

    // обозначаем новый тип СИМВОЛ который является массивом из  symbol_size байт
    using SYMBOL = unsigned char[SYMBOL_SIZE];

    if (!input.size())
        return;

    // лямба функции
    auto SYMBOL_read = [&](SYMBOL &sym, const unsigned char &read_from)
    {
        // в sym из read_from копируется кол-во = SYMBOL_SIZE байтов
        std::memcpy(&sym[0], &read_from, sizeof(SYMBOL));
    };
    auto SYMBOL_compare = [&](SYMBOL &sym, const unsigned char &compare_to)
    {
        // сравнение кусков памяти длиной SYMBOL_SIZE
        return !std::memcmp(&sym[0], &compare_to, sizeof(SYMBOL));
    };
    auto SYMBOL_write = [&](SYMBOL &sym, unsigned char &write_to)
    {
        // запись из sym в write_to
        std::memcpy(&write_to, &sym[0], sizeof(SYMBOL));
    };

    SYMBOL prev;
    SYMBOL_read(prev, input[0]); // в прев считывается с input[0] 1 СИМВОЛ ( SYMBOL_SIZE байт)

    unsigned char counter = 0;
    int i = 0;
    while (i < input.size())
    {
        if (SYMBOL_compare(prev, input[i])) // если символы повторяются
        {
            counter++;
            if (counter == 255) // если превысили размер чара (1 байт)
            {
                output.push_back(254);                      // выписать 255 с текущим символом и продолжить
                output.resize(output.size() + SYMBOL_SIZE); // меняем размер чтобы не вылететь заграницы
                SYMBOL_write(prev, output[output.size() - SYMBOL_SIZE]);
                counter = 1;
            }
        }
        else // если последовательность повторяющихся кончилась
        {
            output.push_back(counter);
            output.resize(output.size() + SYMBOL_SIZE); // меняем размер чтобы не вылететь заграницы
            SYMBOL_write(prev, output[output.size() - SYMBOL_SIZE]);
            SYMBOL_read(prev, input[i]);
            counter = 1;
        }
        i += SYMBOL_SIZE;
    }
    output.push_back(counter);
    output.resize(output.size() + SYMBOL_SIZE);
    SYMBOL_write(prev, output[output.size() - SYMBOL_SIZE]);
}

template <int M>
void rle_decompress_2_3(const std::vector<unsigned char> &input, std::vector<unsigned char> &output)
{
    static const int symbol_size = M / __CHAR_BIT__;
    // обозначаем новый тип СИМВОЛ который является массивом из  symbol_size байт
    using SYMBOL = unsigned char[symbol_size];

    auto SYMBOL_read = [&](SYMBOL &sym, const unsigned char &read_from)
    {
        std::memcpy(&sym[0], &read_from, sizeof(SYMBOL));
    };
    auto SYMBOL_compare = [&](SYMBOL &sym, const unsigned char &compare_to)
    {
        return !std::memcmp(&sym[0], &compare_to, sizeof(SYMBOL));
    };
    auto SYMBOL_write = [&](SYMBOL &sym, unsigned char &write_to)
    {
        std::memcpy(&write_to, &sym[0], sizeof(SYMBOL));
    };
    for (int i = 0; i < input.size(); i += 1 /*1 для счетчика*/ + symbol_size)
    {
        SYMBOL next;
        SYMBOL_read(next, input[i + 1]);                       // считали символ
        int start = output.size();                             // то куда надо начтаь записывать символы
        output.resize(output.size() + input[i] * symbol_size); // готовим место для новых символов
        // вставляем
        for (int j = 0; j < input[i]; ++j)
        {
            SYMBOL_write(next, output[start + j * symbol_size]);
        }
    }
}

// пример: abcabcabc ---> M =24 бит---> 3 "abc" (повторябщийся символ это "abc" занимает 3 байта или 24 бита)
