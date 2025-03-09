#include <iostream>
#include <algorithm>

#include "huffman.h"

using namespace std;

struct HAT_node
{
    HAT_node *L = nullptr;      // левый ребенок
    HAT_node *R = nullptr;      // правый ребенок
    string symb;                // символьная последовательность
    int freq = 0;               // значение частоы
    HAT_node *parent = nullptr; // родитель

    HAT_node() : freq(0), L(nullptr), R(nullptr), parent(nullptr) {}
    HAT_node(char symbol, int x) : freq(x), L(nullptr), R(nullptr), parent(nullptr) { symb.push_back(symbol); }
    HAT_node(char symbol, int x, HAT_node *L, HAT_node *R, HAT_node *par) : freq(x), L(L),
                                                                            R(R), parent(par) { symb.push_back(symbol); }
};

// вспомогательный метод для создания таблицы хаффмана
void HA_make_table(const std::vector<char> &input, std::vector<HuffmanCode> &huffman_table)
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
                tmp.code += '0'; // идем влево пишем 0 по алгоритму
            }
            else if (cur->R)
            {
                cur = cur->R;
                tmp.code += '1'; // идем вправо пишем 1
            }
        }
        tmp.value = s;
        huffman_table.push_back(tmp);
    }
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
            if (l.code.size() < r.code.size())
                return true;
            if (l.code.size() == r.code.size() && l.value < r.value)
                return true;
            return false; });
    for (auto &it : huffman_table)
    {
        cout << "Символ: " << it.value << " ---> " << it.code << endl;
    }
}
