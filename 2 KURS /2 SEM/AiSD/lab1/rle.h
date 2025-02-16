#pragma once

#include <vector>

//первое задание 2.1
void rle_compress_2_1(const std::vector<unsigned char> &input, std::vector<unsigned char> &output);
void rle_decompress_2_1(const std::vector<unsigned char> &input, std::vector<unsigned char> &output);

//второе задание 2.2
void rle_compress_2_2(const std::vector<unsigned char> &input, std::vector<unsigned char> &output);
void rle_decompress_2_2(const std::vector<unsigned char> &input, std::vector<unsigned char> &output);

// 2.3
//параметр М в битах (1 символ = 1 Байт = 8 ьит) это сколько символов РАЗНЫХ
// может быть в повторяющейся последовательности

// пример: abcabcabc ---> M =24 ---> 3 "abc"