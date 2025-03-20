#include <iostream>
#include <algorithm>

#include "huffman.h"

using namespace std;

// вспомогательный метод для создания таблицы хаффмана
void HA_make_table(const std::vector<char> &input, std::vector<HuffmanCode> &huffman_table, HAT_node *& out_root)
{
    std::unordered_map<char, int> freq;
    if (input.empty())
    {
        cout << "empty input!!" << endl;
        return;
    }
    for (auto &it : input)
    {
        freq[it] += 1;
    }
    vector<HAT_node *> nodes;
    for (auto &it : freq)
    {
        nodes.push_back(new HAT_node(it.first, it.second)); // кладем все частоты в массив из узлов
    }
    // лямбда функция возвращает тру если Л больше Р!!!!
    // лямбда функция используется для сортировки нод
    sort(nodes.begin(), nodes.end(), [&](HAT_node *&l, HAT_node *&r)
         {
        if (l->freq > r->freq) // сначала сравниваем по частоте
            return true;
        else if (l->freq == r->freq && l->symb > r->symb) 
            return true;
        return false; });

    // 1. взять 2 узла с минимальным значением частоты из конца вектора nodes (удалить оттуда)
    // 2. создать им общего родителя у которого частота = сумма частот детей,
    // а "символ" = сумма символов детей
    // 3. детей положить в дерево
    // 4. положить родителя в вектор nodes и отсортировать
    // 5. повторять пока в вектор nodes не останется 1 элемент который и будет корнем дерева Хаффмана
    HAT_node *root = nullptr;
    while (nodes.size() > 1)
    {
        HAT_node *l = nodes.back();
        nodes.pop_back();
        HAT_node *r = nodes.back();
        nodes.pop_back();
        HAT_node *tmp = new HAT_node();
        tmp->symb = l->symb + r->symb;
        tmp->freq = l->freq + r->freq;
        tmp->L = l;
        tmp->R = r;
        l->parent = tmp;
        r->parent = tmp;
        nodes.push_back(tmp);

        sort(nodes.begin(), nodes.end(), [&](HAT_node *&l, HAT_node *&r)
             {
        if (l->freq > r->freq) // сначала сравниваем по частоте
            return true;
        else if (l->freq == r->freq && l->symb > r->symb) 
            return true;
        return false; });
    }
    root = nodes.back();
    HAT_node *cur;
    for (auto &it : freq)
    {
        HuffmanCode tmp;
        char s = it.first; // символ для которого строим код Хаффмана
        cur = root;
        while (cur->L || cur->R)
        {
            if (cur->L && (cur->L->symb.find(s) != std::string::npos))
            {
                cur = cur->L;
                tmp.add_bit_to_code(0); // идем влево пишем 0 по алгоритму
            }
            else if (cur->R)
            {
                cur = cur->R;
                tmp.add_bit_to_code(1); // идем вправо пишем 1
            }
        }
        tmp.value = s;
        huffman_table.push_back(tmp);
    }
    out_root = root;
}

// сжатие
void HA_compress(const std::vector<char> &input, std::vector<char> &output, const std::vector<HuffmanCode> &huffman_table)
{
}

// разжатие
void HA_decompress(const std::vector<char> &input, std::vector<char> &output, const std::vector<HuffmanCode> &huffman_table)
{
}

void HA_print_table(std::vector<HuffmanCode> &huffman_table)
{
    std::sort(huffman_table.begin(), huffman_table.end(), [&](const HuffmanCode &l, const HuffmanCode &r)
              {
            if (l.code < r.code)
                return true;
            if (l.code == r.code && l.value < r.value)
                return true;
            return false; });
    for (auto &it : huffman_table)
    {
        cout << "Символ: " << it.value << " ---> " << it.code << " (" << it.bits_len << " битов)" << endl;
    }
}

void HuffmanCode::add_bit_to_code(int val)
{
    if (bits_len == 64)
    {
        std::cout << "Слишком длинный код!!! Abort!" << std::endl;
        std::abort();
    }
    // если поставить 1
    // на тмп сдвинуть влево 1 на индекс bit_len и сделать побитовое ИЛИ с текущей последовательностью битов

    // если ноль
    // на тмп сдвинуть влево 1 на индекс bit_len и сделать побитовое НЕ (отрицание) на маске,
    // потом побитовое И маски с текущей последовательностью битов

    if (val) // если надо добавить 1
    {
        code |= (1UL << bits_len); // типо 1 сдвинули на длину, сделали ИЛИ битовоес текущим кодом
        bits_len++;
    }
    else
    {
        size_t tmp = (1UL << bits_len);
        tmp = ~tmp; // побитовое отрицание
        code &= tmp;
        bits_len++;
    }
};

// добавляет новый кусок кода в сторадж
void Bitmap::add_code(const HuffmanCode &code_to_add)
{
    // смотрим сколько свободных ьитов в последнем заполненном элементе size_t из стораджа
    int free_bits_in_last_size_t = 64 - num_bits % 64;

    // если code_to_add влезает в текущий size_t из стораджа целиком
    if (free_bits_in_last_size_t >= code_to_add.bits_len)
    {
        int last_size_t_idx = num_bits / 64; // индекс size_t в сторадже куда будем писать
        size_t tmp = storage[last_size_t_idx];
        size_t tmp1 = code_to_add.code;
        tmp1 <<= num_bits % 64; // сдвигаем тмп1 на количество уже заполненных битов
        // в текущем элементе size_t из стораджа
        tmp |= tmp1;
        storage[last_size_t_idx] = tmp;
        num_bits += code_to_add.bits_len;
        if (num_bits % 64ULL == 0)
            storage.push_back(0);

    }
    else // если код не влезает целиком в текущий size_t из стораджа
    {
        storage.push_back(0);
        int remaining_bits = code_to_add.bits_len - free_bits_in_last_size_t; // то что не влезло (кол-во в битах)

        int last_size_t_idx = num_bits / 64; // индекс size_t в сторадже куда будем писать
        size_t tmp = storage[last_size_t_idx];
        size_t tmp1 = code_to_add.code;
        tmp1 <<= num_bits % 64; // сдвигаем тмп1 на количество уже заполненных битов
        // в текущем элементе size_t из стораджа
        tmp |= tmp1;
        storage[last_size_t_idx] = tmp;

        // обрабатывает не влезшие биты
        tmp1 = code_to_add.code;
        tmp1 >>= free_bits_in_last_size_t; // "отрезаем" уже добавленные биты от code_to_add
        last_size_t_idx++;
        storage[last_size_t_idx] = tmp1;
        num_bits += code_to_add.bits_len;
    }
}

size_t Bitmap::get_bit(size_t idx)
{
    size_t mask = 1ULL << (idx % 64ULL);
    size_t res = storage[idx / 64ULL];
    res &= mask;
    return res;
}

// добывает символ из стораджа по индексу = кол-во битов, с которых надо начинать считывать код
// кодировка берется из таблицы Хаффмана, возвращает считанный символ symb и обновляет индекс
//  увеличивая его на длину кода считанного символа
// idx = сколько битов уже считали т.е. (с какого бита надо начинать считывать следующий символ)
void Bitmap::get_next_symbol(int &idx, std::vector<HuffmanCode> &huffman_table, HAT_node *&root, char &symb)
{
    HAT_node *cur = root;
    if (!cur)
        return;
    while (cur->R || cur->L)
    {
        size_t tmp = get_bit(idx);
        if (tmp)
            cur = cur->R;
        else
            cur = cur->L;
        ++idx;
    }
    symb = cur->symb[0];
}
