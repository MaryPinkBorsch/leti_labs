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

struct Bitmap
{
    // сторадж содержит битовые данные в блоках по 64 (размер size_t в битах)
    std::vector<size_t> storage;
    int num_bits = 0; // кол-во хранимых бит в сторадже

    // добавляет новый кусок кода в сторадж
    void add_code(const HuffmanCode &code_to_add);

    // добывает символ из стораджа по индексу = кол-во битов, с которых надо начинать считывать код
    // кодировка берется из таблицы Хаффмана, возвращает считанный символ symb и обновляет индекс
    //  увеличивая его на длину кода считанного символа
    void get_next_symbol(int &idx, std::vector<HuffmanCode> &huffman_table, char &symb);
};

// вспомогательный метод для создания таблицы хаффмана
void HA_make_table(const std::vector<char> &input, std::vector<HuffmanCode> &huffman_table);
void HA_print_table(std::vector<HuffmanCode> &huffman_table);

// сжатие

// нужнен output = вектоор из size_t, куда мы будем писать коды закодированных символов по таблице
// в 1м size_t может быть несколько кодов, в зависимости от длин кодов
//(один код может разделиться на 2 size_t!!! проверка при чтении)
void HA_compress(const std::vector<char> &input, std::vector<char> &output, const std::vector<HuffmanCode> &huffman_table);

// разжатие
void HA_decompress(const std::vector<char> &input, std::vector<char> &output, const std::vector<HuffmanCode> &huffman_table);
