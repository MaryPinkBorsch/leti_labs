#include "mtf.h"
#include "serialization.h"

using namespace std;

// !если символа нет в алфавите то вернется пустота

// void MTF_compress(std::vector<char> &input, std::vector<int> &output, forward_list<char> alphabet)
void MTF_compress(std::vector<char> &input, std::vector<char> &output)
{
    forward_list<char> alphabet;
    for (int i = 32; i <= 126; i++)
    {
        alphabet.push_front((char)i);
    }
    for (auto c = input.begin(); c != input.end(); c++)
    {
        // ищем в алфавите индекс символа из инпута
        int index = 0;
        bool found = false;
        forward_list<char>::iterator character = alphabet.begin();
        forward_list<char>::iterator next_character = next(alphabet.begin(), 1);

        if (*character != *c)
        {
            while (!found && next_character != alphabet.end())
            {
                index++;
                if (*next_character == *c)
                {
                    found = true;
                }
                else
                {
                    character++;
                    next_character++;
                }
            }

            if (!found)
            {
                // символа нет в алфавите, ошибка!!
                return;
            }

            // переместить элемент с индексом в начало списка
            alphabet.erase_after(character);
            alphabet.push_front(*c);
        }

        // добавить индекс в аутпут
        size_t last_size = output.size();
        output.resize(output.size() + sizeof(int));
        std::memcpy(&output[last_size], &index, sizeof(int));
    }
}

void MTF_decompress(std::vector<char> &input, std::vector<char> &output)
{
    string source = "";
    source.reserve(input.size());
    forward_list<char> alphabet;
    for (int i = 32; i <= 126; i++)
    {
        alphabet.push_front((char)i);
    }
    if (alphabet.begin() != alphabet.end())
    {
        for (int i = 0; i < input.size(); i += sizeof(int))
        {
            if (!(i % 16*1024))
                std::cout << "MTF_decompress i: " << i << std::endl;
            int index = 0;
            std::memcpy(&index, &input[i], sizeof(int));
            if (index != 0)
            {
                forward_list<char>::iterator character = next(alphabet.begin(), index - 1);
                forward_list<char>::iterator next_character = next(character, 1);

                // добавить символ с индексом в алфавите к исходному тексту
                source = source + *next_character;

                // Переместить элемент с индексом в начало
                alphabet.push_front(*next_character);
                alphabet.erase_after(character);
            }
            else
            {
                source = source + *(alphabet.begin());
            }
        }
    }
    output.insert(output.begin(), source.begin(), source.end());
}
