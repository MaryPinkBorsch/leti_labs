#pragma once
#include <iostream>
#include <string>
#include <vector>

struct LZ77_Node
{
    int offset; // смещение
    int length; // длина
    char next;  // следующий символ
};

void LZ77_compress(const std::vector<char> &input, std::vector<LZ77_Node> &output);
void LZ77_decompress(const std::vector<LZ77_Node> &encoded, std::vector<char> &output);