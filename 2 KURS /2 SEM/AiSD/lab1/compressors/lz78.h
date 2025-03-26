#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "serialization.h"

struct LZ78_Node
{
    int pos;   // номер слова в словаре
    char next; // следующий символ
};

void serialize(std::vector<char> &buffer, const LZ78_Node &val);
void deserialize(const std::vector<char> &buffer, LZ78_Node &val, size_t &idx);

void LZ78_compress(const std::vector<char> &input, std::vector<char> &output);
void LZ78_decompress(const std::vector<char> &input, std::vector<char> &output);
