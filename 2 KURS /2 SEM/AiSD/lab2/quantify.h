#pragma once

#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include "downsampling.h"

// extern std::vector<std::vector<int>> q_table;

void dequantify_vec(std::vector<Matrix> &matrixes, int q_lvl);
void dequantify(Matrix &matrix, int q_lvl);

void quantify(Matrix &matrix, int q_lvl);
void quantify_vec(std::vector<Matrix> &matrixes, int q_lvl);
void get_DC(std::vector<Matrix> &matrixes, std::vector<double> &DCs);
