#pragma once
#include <vector>
#include <algorithm>

void RGB2YCBCR(unsigned char rgb[3], double ycbcr[3]);
void YCBCR2RGB(double ycbcr[3], unsigned char rgb[3]);

void RGB_to_YCBRCR_vector(std::vector<unsigned char> &input, std::vector<double> &output);
void YCBRCR_to_RGB_vector(std::vector<double> &input, std::vector<unsigned char> &output);
