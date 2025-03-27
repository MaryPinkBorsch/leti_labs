#pragma once

#include <vector>

extern int G_BWT_BLOCK_SIZE;

// БВТ применять только к текстовым данным!!

void BWT_compress(std::vector<char> &input, std::vector<char> &output);
void BWT_decompress(std::vector<char> &input, std::vector<char> &output);
