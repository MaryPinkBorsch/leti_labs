#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

struct LZ78_Node
{
    int pos;   // номер слова в словаре
    char next; // следующий символ
};

void LZ78_compress(const std::vector<char> &input, std::vector<LZ78_Node> &output);
void LZ78_decompress(const std::vector<LZ78_Node> &encoded, std::vector<char> &output);
