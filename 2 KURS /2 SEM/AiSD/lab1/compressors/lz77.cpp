#include "lz77.h"

using namespace std;

// Функция для поиска совпадений в буфере
std::pair<int, int> findMatching(const std::vector<char> &buffer, int pos)
{
    int maxLength = 0;
    int offset = 0;

    static const int LZ77_WINDOW_SIZE = 256;
    // Ищем максимальное совпадение в окне pos - 4096 -> pos
    int window_start  = pos <= LZ77_WINDOW_SIZE ? 0 : pos - LZ77_WINDOW_SIZE;
    for (int i = window_start; i < pos; ++i)
    {
        int currentLength = 0;
        while (i + currentLength < pos && pos + currentLength < buffer.size() &&
               buffer[i + currentLength] == buffer[pos + currentLength])
        {
            currentLength++;
        }
        if (currentLength > maxLength)
        {
            maxLength = currentLength;
            offset = pos - i; // смещение
        }
    }

    return {offset, maxLength};
}

// Функция для кодирования строки с использованием LZ77
void LZ77_compress(const std::vector<char> &input, std::vector<char> &output)
{
    int pos = 0;
    std::vector<LZ77_Node> tmp;
    while (pos < input.size())
    {
        auto [offset, length] = findMatching(input, pos);                           // ищем слово в словаре
        char nextChar = (pos + length < input.size()) ? input[pos + length] : '\0'; // следующий символ
        tmp.push_back({offset, length, nextChar});                                  // добавляем в ответ очередной блок
        pos += length + 1;                                                          // перемещаемся на длину совпадения + 1
    }
    serialize(output, tmp);
}

// Функция для декодирования списка блоков
void LZ77_decompress(const std::vector<char> &input, std::vector<char> &output)
{
    size_t idx = 0;
    std::vector<LZ77_Node> encoded;
    deserialize(input, encoded, idx);
    for (int j = 0; j < encoded.size(); j++)
    {
        const LZ77_Node &node = encoded[j];
        if (node.length > 0)
        {                                            // если необходим повтор
            int start = output.size() - node.offset; // возвращаемся на символов назад
            for (int i = 0; i < node.length; ++i)
            { // добавляем символы
                output.push_back(output[start + i]);
            }
        }
        if (j == encoded.size() - 1 && (node.length != 0 && node.offset != 0))
            break;
        output.push_back(node.next); // добавляем следующий символ
    }
}

void serialize(std::vector<char> &buffer, const LZ77_Node &val)
{
    serialize(buffer, val.offset);
    serialize(buffer, val.length);
    serialize(buffer, val.next);
}

void deserialize(const std::vector<char> &buffer, LZ77_Node &val, size_t &idx)
{
    deserialize(buffer, val.offset, idx);
    deserialize(buffer, val.length, idx);
    deserialize(buffer, val.next, idx);
}
