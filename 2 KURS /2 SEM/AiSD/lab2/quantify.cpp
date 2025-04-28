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

// q_lvl = уровень качества от 0 до 100
void quantify(Matrix &matrix, int q_lvl)
{
    std::vector<std::vector<int>> q_table;
    table(q_table);

    for (int i = 0; i < matrix.data.size(); i++)
    {
        for (int j = 0; j < matrix.data[i].size(); j++)
        {

            double s = 0;
            if (q_lvl < 50)
            {
                s = 5000 / q_lvl;
            }
            else
            {
                s = 200 - 2 * q_lvl;
            }
            double k = round((q_table[i][j] * s + 50) / 100);

            matrix.data[i][j] = floor(matrix.data[i][j] / k) * k;
        }
    }
}

void quantify_vec(std::vector<Matrix> &matrixes, int q_lvl)
{
    for (int i = 0; i < matrixes.size(); i++)
        quantify(matrixes[i], q_lvl);
}

void dequantify()
{
    std::vector<std::vector<int>> q_table;
    table(q_table);
}

// функция чтоб получить все ДС коэффициенты (позиция [0][0] в каждой матрице цветовгого канала)
void get_DC(std::vector<Matrix> &matrixes, std::vector<double> &DCs)
{
    for (int i = 0; i < matrixes.size(); i++)
    {
        DCs.push_back(matrixes[i].data[0][0]);
    }
}