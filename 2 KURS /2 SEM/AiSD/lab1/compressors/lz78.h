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

void serialize(std::deque<char> &buffer, const LZ78_Node &val);
void deserialize(const std::deque<char> &buffer, LZ78_Node &val, size_t& idx);

void LZ78_compress(const std::vector<char> &input, std::vector<LZ78_Node> &output);
void LZ78_decompress(const std::vector<LZ78_Node> &encoded, std::vector<char> &output);

