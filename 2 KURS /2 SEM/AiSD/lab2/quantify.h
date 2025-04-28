#pragma once

#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include "downsampling.h"

// extern std::vector<std::vector<int>> q_table;

void quantify(Matrix &matrix, int q_lvl);
void dequantify();

void quantify_vec(std::vector<Matrix> &matrixes, int q_lvl);