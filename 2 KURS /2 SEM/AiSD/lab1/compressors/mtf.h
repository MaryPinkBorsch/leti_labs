#pragma once

#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <forward_list>

void MTF_compress(std::vector<char> &input, std::vector<int> &output, std::forward_list<char> alphabet);
void MTF_decompress(std::vector<int> &input, std::vector<char> &output, std::forward_list<char> alphabet);