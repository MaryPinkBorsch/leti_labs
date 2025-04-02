#pragma once

#include <vector>

extern int G_BWT_BLOCK_SIZE;

void BWT_MTF_compress(std::vector<char> &input, std::vector<char> &output);
void BWT_MTF_decompress(std::vector<char> &input, std::vector<char> &output);

void BWT_compress(std::vector<char> &input, std::vector<char> &output, bool mtf = false);
void BWT_decompress(std::vector<char> &input, std::vector<char> &output, bool mtf = false);
