#include "lz78.h"
using namespace std;
// Функция для кодирования строки с использованием LZ78
void LZ78_compress(const std::vector<char> &input, std::vector<LZ78_Node> &output)
{
    std::string buffer = "";         // текущий префикс
    std::map<std::string, int> dict; // словарь

    for (size_t i = 0; i < input.size(); ++i)
    {
        // Проверяем, можем ли мы увеличить префикс
        if (dict.find(buffer + input[i]) != dict.end())
        {
            buffer += input[i];
        }
        else
        {
            // Добавляем пару в ответ
            output.push_back({dict[buffer], input[i]});
            // Добавляем слово в словарь
            dict[buffer + input[i]] = dict.size() + 1; // dict.size() + 1, так как индексация начинается с 1
            buffer = "";                               // сбрасываем буфер
        }
    }

    // Если буфер не пуст, добавляем его в словарь
    if (!buffer.empty())
    {
        char last_ch = buffer.back();              // берем последний символ буфера
        buffer.pop_back();                         // удаляем последний символ из буфера
        output.push_back({dict[buffer], last_ch}); // добавляем пару в ответ
    }
}

// Функция для декодирования списка блоков
void LZ78_decompress(const std::vector<LZ78_Node> &encoded, std::vector<char> &output)
{
    std::vector<std::string> dict(1, ""); // словарь, слово с номером 0 — пустая строка

    for (const auto &node : encoded)
    {
        std::string word = dict[node.pos-1] + node.next;           // составляем слово
        output.insert(output.end(), word.begin(), word.end()); // приписываем к ответу
        dict.push_back(word);                                    // добавляем в словарь
    }
}
