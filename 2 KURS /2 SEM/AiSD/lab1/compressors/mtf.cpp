#include "mtf.h"
using namespace std;

// Move-to-front encoding
// Gives empty encoding if empty alphabet or empty source, or source contains a character not in the alphabet
vector<int> MTF_encode(string source, forward_list<char> alphabet)
{
    vector<int> encoding;

    // Parameter checking
    if (alphabet.begin() != alphabet.end())
    {
        for (string::iterator c = source.begin(); c != source.end(); c++)
        {
            // Search the alphabet for the index of the letter in source
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
                    return vector<int>();
                }

                // Move the element at index to the front of the list
                alphabet.erase_after(character);
                alphabet.push_front(*c);
            }

            // Add the index to the integer sequence encoding
            encoding.push_back(index);
        }
    }

    return encoding;
}

string MTF_decode(vector<int> encoding, forward_list<char> alphabet)
{
    string source = "";

    if (alphabet.begin() != alphabet.end())
    {
        for (vector<int>::iterator index = encoding.begin(); index != encoding.end(); index++)
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

    return source;
}
