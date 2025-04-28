#include <iostream>
#include <fstream>
#include <math.h>
#include "downsampling.h"

extern double pi = 3.141592654;
extern const int len = 8;

// double f[len][len];
// double F[len][len];
double c = 1 / sqrt(2);

// bool input(double arr[][len]);
// void output(double arr[][len]);
void DCT(Block input, Block &output);
double FDCT(int u, int v, bool flag, Block input);
void DCT_of_blocks();