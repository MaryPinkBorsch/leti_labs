#pragma once

#include <bits/stdc++.h>

// void BWT_compress(std::vector<char> &input, std::vector<char> &output);
// void BWT_decompress(const std::vector<char> &input, std::vector<char> &output);

#include <string>
#include <vector>
#include <forward_list>
#include <fstream>
#include <bitset>

std::vector<int> MTF_encode(std::string source, std::forward_list<char> alphabet);
std::string MTF_decode(std::vector<int> encoding, std::forward_list<char> alphabet);
