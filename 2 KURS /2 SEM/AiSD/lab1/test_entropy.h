#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>

void prob_estimate(std::string &s, std::unordered_map<char, double> &probs);

void entropy(std::string &s, int code_len, double &entr);