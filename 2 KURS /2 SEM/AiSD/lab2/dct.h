#include <iostream>
#include <fstream>
#include <math.h>
#include "downsampling.h"

// double pi;
// const int len = 8;
// double c = 1 / sqrt(2);

// bool input(double arr[][len]);
// void output(double arr[][len]);

struct Matrix
{
    std::vector<std::vector<double>> data;
};

void DCT(Block input, Block &output);
double FDCT(int u, int v, bool flag, Block input);
void DCT_of_blocks(std::vector<Block> input, std::vector<Block> &output);
void block_to_3_matrix(Block input, std::vector<std::vector<double>> &Y_matrix, std::vector<std::vector<double>> &Cb_matrix, std::vector<std::vector<double>> &Cr_matrix);

void blocks_to_matrixes(std::vector<Block> input, std::vector<Matrix> &Y_matrix, std::vector<Matrix> &Cb_matrix, std::vector<Matrix> &Cr_matrix);
