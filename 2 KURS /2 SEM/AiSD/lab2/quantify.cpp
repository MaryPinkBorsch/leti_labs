#include "quantify.h"
using namespace std;

void table(std::vector<std::vector<int>> &q_table)
{

    q_table.resize(8); 

    q_table[0] = {16, 11, 10, 16, 124, 140, 151, 161};
    q_table[1] = {12, 12, 14, 19, 126, 158, 160, 155};
    q_table[2] = {14, 13, 16, 24, 140, 157, 169, 156};
    q_table[3] = {14, 17, 22, 29, 151, 187, 180, 162};
    q_table[4] = {18, 22, 37, 56, 168, 109, 103, 177};
    q_table[5] = {24, 35, 55, 64, 181, 104, 113, 192};
    q_table[6] = {49, 64, 78, 87, 103, 121, 120, 101};
    q_table[7] = {72, 92, 95, 98, 112, 100, 103, 199};
}

void quantify()
{
}
void dequantify() { ; }