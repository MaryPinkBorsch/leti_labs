#include "lz77.h" 

using namespace std;

// Функция для поиска совпадений в буфере
std::pair<int, int> findMatching(const std::vector<char>& buffer, int pos) {
    int maxLength = 0;
    int offset = 0;

    // Ищем максимальное совпадение
    for (int i = 0; i < pos; ++i) {
        int currentLength = 0;
        while (i + currentLength < pos && pos + currentLength < buffer.size() &&
               buffer[i + currentLength] == buffer[pos + currentLength]) {
            currentLength++;
        }
        if (currentLength > maxLength) {
            maxLength = currentLength;
            offset = pos - i;  // смещение
        }
    }

    return {offset, maxLength};
}

// Функция для кодирования строки с использованием LZ77
void LZ77_compress(const std::vector<char> &input, std::vector<LZ77_Node> & output) {
    int pos = 0;
    while (pos < input.size()) {
        auto [offset, length] = findMatching(input, pos); // ищем слово в словаре
        char nextChar = (pos + length < input.size()) ? input[pos + length] : '\0'; // следующий символ
        output.push_back({offset, length, nextChar}); // добавляем в ответ очередной блок
        pos += length + 1; // перемещаемся на длину совпадения + 1
    }
}

// Функция для декодирования списка блоков
void LZ77_decompress(const std::vector<LZ77_Node>& encoded, std::vector<char> &output) {

    for (int j = 0; j < encoded.size(); j++) {
        const LZ77_Node & node = encoded[j];
        if (node.length > 0) { // если необходим повтор
            int start = output.size() - node.offset; // возвращаемся на символов назад
            for (int i = 0; i < node.length; ++i) { // добавляем символы
                output.push_back(output[start + i]);
            }
        }
        if (j == encoded.size()-1 && (node.length != 0 && node.offset != 0))
            break;
        output.push_back(node.next); // добавляем следующий символ
    }
}
