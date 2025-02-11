#include "rle.h"

using namespace std;

void rle_compress_2_1(const std::vector<unsigned char> &input, std::vector<unsigned char> &output)
{
    //     Пример:
    // 0xCF 0xCF 0xCF 0xCF 0xCF -> 0x05 0xCF
    if (!input.size())
        return;
    unsigned char prev = input[0];
    unsigned char counter = 0;
    int i = 0;
    while (i < input.size())
    {
        if (input[i] == prev) // если символы повторяются
        {
            counter++;
            if (counter == 255) // если превысили размер чара (1 байт)
            {
                output.push_back(254); // выписать 255 с текущим символом и продолжить
                output.push_back(prev);
                counter = 1;
            }
        }
        else // если последовательность повторяющихся кончилась
        {
            output.push_back(counter);
            output.push_back(prev);
            prev = input[i];
            counter = 1;
        }
        i++;
    }
    output.push_back(counter);
    output.push_back(prev);
}
void rle_decompress_2_1(const std::vector<unsigned char> &input, std::vector<unsigned char> &output)
{
    for (int i = 0; i < input.size(); i += 2)
    {
        output.insert(output.end(), input[i], input[i + 1]); // вставляем такое-то кол-во input[i] символов input[i+1]
    }
}