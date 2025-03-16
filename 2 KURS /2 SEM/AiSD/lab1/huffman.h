#pragma once

#include <vector>
#include <unordered_map>
#include <cstring>
#include <string>

struct HuffmanCode
{
    static const size_t MAX_BIT_LEN = 64;
    size_t code = 0; // закодированное значение символа (в битах)
    size_t bits_len = 0;
    // длинна в битах кода хаффмана
    // std::string code;
    char value; // какому символу соответствует этот код

    void add_bit_to_code(int val);
};

// вспомогательный метод для создания таблицы хаффмана
void HA_make_table(const std::vector<char> &input, std::vector<HuffmanCode> &huffman_table);
void HA_print_table(std::vector<HuffmanCode> &huffman_table);

// сжатие
void HA_compress(const std::vector<char> &input, std::vector<char> &output, const std::vector<HuffmanCode> &huffman_table);

// разжатие
void HA_decompress(const std::vector<char> &input, std::vector<char> &output, const std::vector<HuffmanCode> &huffman_table);
