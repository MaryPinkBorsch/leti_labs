#pragma once

#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include "downsampling.h"

// bool flag_Y = 0 ----> Cr Cb
// bool flag_Y = 1 ----> Y

void dequantify_vec(std::vector<Matrix> &matrixes, int q_lvl, bool flag_Y);
void dequantify(Matrix &matrix, int q_lvl, bool flag_Y);

void quantify(Matrix &matrix, int q_lvl, bool flag_Y);
void quantify_vec(std::vector<Matrix> &matrixes, int q_lvl, bool flag_Y);
void get_DC(std::vector<Matrix> &matrixes, std::vector<double> &DCs);
void get_AC(std::vector<Matrix> &matrixes, std::vector<double> &ACs);

void table(std::vector<std::vector<int>> &q_table, std::vector<std::vector<int>> &chroma_q_table);

void put_AC(std::vector<Matrix> &matrixes, std::vector<double> &ACs);
void put_DC(std::vector<Matrix> &matrixes, std::vector<double> &DCs);

void dequantify2(Matrix &matrix, int q_lvl, bool flag_Y);
void dequantize_block(Matrix &matrix, int quality_level, bool flag_Y);

void quantize_block(Matrix &matrix, int quality_level, bool flag_Y);