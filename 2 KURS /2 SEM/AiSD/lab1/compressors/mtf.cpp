#include "mtf.h"
using namespace std;

// Move-to-front output
// Gives empty output if empty alphabet or empty input, or input contains a character not in the alphabet

void MTF_compress(std::vector<char> &input, std::vector<int> &output, forward_list<char> alphabet)
{
    // Parameter checking
    if (alphabet.begin() != alphabet.end())
    {
        for (auto c = input.begin(); c != input.end(); c++)
        {
            // Search the alphabet for the index of the letter in input
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
                    // character was not in the alphabet provided, something went wrong
                    return;
                }

                // Move the element at index to the front of the list
                alphabet.erase_after(character);
                alphabet.push_front(*c);
            }

            // Add the index to the integer sequence output
            output.push_back(index);
        }
    }
}

void MTF_decompress(std::vector<int> &input, std::vector<char> &output, forward_list<char> alphabet)
{
    string source = "";

    if (alphabet.begin() != alphabet.end())
    {
        for (vector<int>::iterator index = input.begin(); index != input.end(); index++)
        {
            if (*index != 0)
            {
                forward_list<char>::iterator character = next(alphabet.begin(), (*index) - 1);
                forward_list<char>::iterator next_character = next(character, 1);

                // Append the character at index in the alphabet to the source text
                source = source + *next_character;

                // Move the element at index to the front
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
