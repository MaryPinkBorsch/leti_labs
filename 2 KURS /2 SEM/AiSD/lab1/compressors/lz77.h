#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "serialization.h"

struct LZ77_Node
{
    int offset; // смещение
    int length; // длина
    char next;  // следующий символ
};

void serialize(std::deque<char> &buffer, const LZ77_Node &val);
void deserialize(const std::deque<char> &buffer, LZ77_Node &val, size_t& idx);

void LZ77_compress(const std::vector<char> &input, std::vector<LZ77_Node> &output);
void LZ77_decompress(const std::vector<LZ77_Node> &encoded, std::vector<char> &output);