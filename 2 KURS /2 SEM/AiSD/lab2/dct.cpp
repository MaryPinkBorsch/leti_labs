#include "dct.h"

using namespace std;

// сюда получается передаются блок 8х8 на обраотку ДКТ
// в итоге вернутся блоки  8х8 где на позиции [0][0] будет стоять DC коэффициент
// ДКТ использует для преобразования вспосогательную функцию фдкт

// F - output
// f - input
void DCT(Block input, Block &output)
{
    for (int j = 0; j < len; ++j)
    {
        for (int k = 0; k < len; ++k)
        {
            // F[j][k] = FDCT(j, k);
            output.matrix_data[j][k].Cb = FDCT(j, k, 0, input);
            output.matrix_data[j][k].Cr = FDCT(j, k, 1, input);
            // cur.matrix_data[j][k] = FDCT(j, k);
        }
    }
}

double FDCT(int u, int v, bool flag, Block input)
{
    double Cu = (u == 0) ? c : 1.0;
    double Cv = (v == 0) ? c : 1.0;
    double dct_const = (Cu * Cv) / 4;
    double sum = 0.0;
    int tmp;
    // flag == 0 ---> Cb
    // flag == 1 ---> Cr

    for (int j = 0; j < len; ++j)
    {
        tmp = j << 1;
        for (int k = 0; k < len; ++k)
        {
            // sum += f[j][k] * cos(((tmp + 1) * u * pi) / 16) * cos(((k << 1 + 1) * v * pi) / 16);
            double t = 0;
            if (flag)
                t = input.matrix_data[j][k].Cr;
            else
                t = input.matrix_data[j][k].Cb;
            sum += t * cos(((tmp + 1) * u * pi) / 16) * cos(((k << 1 + 1) * v * pi) / 16);
        }
    }
    return dct_const * sum;
}

// на вход массив блоков, на выход тоже
void DCT_of_blocks(std::vector<Block> input, std::vector<Block> &output)
{
    for (int i = 0; i < input.size(); i++)
        DCT(input[i], output[i]);
}
