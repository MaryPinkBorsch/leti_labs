#include "lz78.h"

#include "serialization.h"

using namespace std;

// Функция для кодирования строки с использованием LZ78
void LZ78_compress(const std::vector<char> &input, std::vector<char> &output)
{
    std::vector<LZ78_Node> tmp;
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
            tmp.push_back({dict[buffer], input[i]});
            // Добавляем слово в словарь
            dict[buffer + input[i]] = dict.size() + 1; // dict.size() + 1, так как индексация начинается с 1
            buffer = "";                               // сбрасываем буфер
        }
    }

    // Если буфер не пуст, добавляем его в словарь
    if (!buffer.empty())
    {
        char last_ch = buffer.back();           // берем последний символ буфера
        buffer.pop_back();                      // удаляем последний символ из буфера
        tmp.push_back({dict[buffer], last_ch}); // добавляем пару в ответ
    }
    serialize(output, tmp);
}

// Функция для декодирования списка блоков
void LZ78_decompress(const std::vector<char> &input, std::vector<char> &output)
{
    std::vector<std::string> dict(1, ""); // словарь, слово с номером 0 — пустая строка
    std::vector<LZ78_Node> encoded;
    size_t offset = 0;
    deserialize(input, encoded, offset);

    for (const auto &node : encoded)
    {
        size_t idx = node.pos ? node.pos - 1 : 0;
        std::string word = dict[idx] + node.next; // составляем слово
        output.insert(output.end(), word.begin(), word.end()); // приписываем к ответу
        dict.push_back(word);                                  // добавляем в словарь
    }
}

void serialize(std::vector<char> &buffer, const LZ78_Node &val)
{
    serialize(buffer, val.pos);
    serialize(buffer, val.next);
}

void deserialize(const std::vector<char> &buffer, LZ78_Node &val, size_t &idx)
{
    deserialize(buffer, val.pos, idx);
    deserialize(buffer, val.next, idx);
}