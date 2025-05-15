#pragma once

#include <iostream>
#include <fstream>
#include <math.h>
#include "downsampling.h"

// extern double pi;
// extern const int len = 8;
// extern double c = 1 / sqrt(2);

extern double pi;
extern const int len;
extern double c;

void DCT(Block &input, Block &output);
double FDCT(int u, int v, int flag, Block &input);
void DCT_of_blocks(std::vector<Block> &input, std::vector<Block> &output);

void rev_DCT(Block &input, Block &output);
double rev_FDCT(int u, int v, int flag, Block &input);
void rev_DCT_of_blocks(std::vector<Block> &input, std::vector<Block> &output);

void block_to_3_matrix(Block &input, std::vector<std::vector<double>> &Y_matrix, std::vector<std::vector<double>> &Cb_matrix, std::vector<std::vector<double>> &Cr_matrix);
void blocks_to_matrixes(std::vector<Block> &input, std::vector<Matrix> &Y_matrix, std::vector<Matrix> &Cb_matrix, std::vector<Matrix> &Cr_matrix);

void matrixes_to_block(std::vector<Block> &output, std::vector<Matrix> &Y_matrix, std::vector<Matrix> &Cb_matrix, std::vector<Matrix> &Cr_matrix);
void matrix_to_block(Block &output, std::vector<std::vector<double>> &Y_matrix, std::vector<std::vector<double>> &Cb_matrix, std::vector<std::vector<double>> &Cr_matrix);