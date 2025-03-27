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

extern int G_LZ77_WINDOW_SIZE;

void serialize(std::vector<char> &buffer, const LZ77_Node &val);
void deserialize(const std::vector<char> &buffer, LZ77_Node &val, size_t& idx);

void LZ77_compress(const std::vector<char> &input, std::vector<char> &output);
void LZ77_decompress(const std::vector<char> &input, std::vector<char> &output);

