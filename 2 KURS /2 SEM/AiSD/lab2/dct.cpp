#include "dct.h"

using namespace std;
double pi = 3.141592654;
const int len = 8;
double c = 1.0 / sqrt(2.0);

// сюда получается передаются блок 8х8 на обраотку ДКТ
// в итоге вернутся блоки  8х8 где на позиции [0][0] будет стоять DC коэффициент
// ДКТ использует для преобразования вспосогательную функцию фдкт

// F - output
// f - input
void DCT(Block &input, Block &output)
{
    output.matrix_data.resize(len);
    for (int y = 0; y < len; y++)
    {
        output.matrix_data[y].resize(len);
    }

    for (int u = 0; u < len; ++u)
    {
        for (int v = 0; v < len; ++v)
        {
            output.matrix_data[v][u].Cb = FDCT(u, v, 0, input);
            output.matrix_data[v][u].Cr = FDCT(u, v, 1, input);
            output.matrix_data[v][u].Y = FDCT(u, v, 2, input);
        }
    }
}

double FDCT(int u, int v, int flag, Block &input)
{
    double Cu = (u == 0) ? c : 1.0;
    double Cv = (v == 0) ? c : 1.0;
    double sum = 0.0;

    // flag == 0 ---> Cb
    // flag == 1 ---> Cr
    // flag == 2 ---> Y

    for (int x = 0; x < len; ++x)
    {
        for (int y = 0; y < len; ++y)
        {
            // sum += f[y][x] * cos(((tmp + 1) * u * pi) / 16) * cos(((y << 1 + 1) * v * pi) / 16);
            double t = 0;
            if (flag == 0)
                t = input.matrix_data[y][x].Cb;
            else if (flag == 1)
                t = input.matrix_data[y][x].Cr;
            else if (flag == 2)
                t = input.matrix_data[y][x].Y;

            sum += t * cos(((2.0 * x + 1.0) * u * pi) / 16.0) * cos(((y * 2.0 + 1.0) * v * pi) / 16.0);
        }
    }
    return 0.25 * Cu * Cv * sum;
}

// обратный ДКТ
double rev_FDCT(int y, int x, int flag, Block &input)
{

    double sum = 0.0;
    int tmp;
    // flag == 0 ---> Cb
    // flag == 1 ---> Cr
    // flag == 2 ---> Y

    for (int u = 0; u < len; ++u)
    {
        for (int v = 0; v < len; ++v)
        {
            double Cu = (u == 0) ? c : 1.0;
            double Cv = (v == 0) ? c : 1.0;
            // sum += f[v][u] * cos(((tmp + 1) * u * pi) / 16) * cos(((v << 1 + 1) * v * pi) / 16);
            double t = 0;
            if (flag == 0)
                t = input.matrix_data[v][u].Cb;
            else if (flag == 1)
                t = input.matrix_data[v][u].Cr;
            else if (flag == 2)
                t = input.matrix_data[v][u].Y;

            // ВООБЩЕ НЕ УВЕРЕНА В этой строчке, но кажется по формуле должно быть так:
            sum += Cu * Cv * t * cos(((2.0 * x + 1.0) * u * pi) / 16.0) * cos(((2.0 * y + 1.0) * v * pi) / 16.0);

            // sum += t * cos(((v + 1) * u * pi) / 16) * cos(((tmp << 1 + 1) * v * pi) / 16);
        }
    }
    return 0.25 * sum;
}

void rev_DCT(Block &input, Block &output)
{
    output.matrix_data.resize(len);
    for (int i = 0; i < len; i++)
    {
        output.matrix_data[i].resize(len);
    }

    for (int j = 0; j < len; ++j)
    {
        for (int k = 0; k < len; ++k)
        {
            output.matrix_data[j][k].Cb = rev_FDCT(j, k, 0, input);
            output.matrix_data[j][k].Cr = rev_FDCT(j, k, 1, input);
            output.matrix_data[j][k].Y = rev_FDCT(j, k, 2, input);
        }
    }
}

// на вход массив блоков, на выход тоже
void DCT_of_blocks(std::vector<Block> &input, std::vector<Block> &output)
{
    if (!input.size())
        abort;
    output.resize(input.size());
    for (int i = 0; i < input.size(); i++)
        DCT(input[i], output[i]);
}

// на вход массив блоков, на выход тоже
void rev_DCT_of_blocks(std::vector<Block> &input, std::vector<Block> &output)
{
    if (!input.size())
        abort;
    output.resize(input.size());
    for (int i = 0; i < input.size(); i++)
        rev_DCT(input[i], output[i]);
}

// по идее надо разбить одну структуру блок на три матрицы где значения только 1го канала из 3х
void block_to_3_matrix(Block &input, std::vector<std::vector<double>> &Y_matrix, std::vector<std::vector<double>> &Cb_matrix, std::vector<std::vector<double>> &Cr_matrix)
{
    Y_matrix.resize(8);
    Cb_matrix.resize(8);
    Cr_matrix.resize(8);
    for (int i = 0; i < 8; i++)
    {
        Y_matrix[i].resize(8);
        Cb_matrix[i].resize(8);
        Cr_matrix[i].resize(8);
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Y_matrix[i][j] = input.matrix_data[i][j].Y;
            Cb_matrix[i][j] = input.matrix_data[i][j].Cb;
            Cr_matrix[i][j] = input.matrix_data[i][j].Cr;
        }
    }
}

void blocks_to_matrixes(std::vector<Block> &input, std::vector<Matrix> &Y_matrix, std::vector<Matrix> &Cb_matrix, std::vector<Matrix> &Cr_matrix)
{
    Y_matrix.resize(input.size());
    Cb_matrix.resize(input.size());
    Cr_matrix.resize(input.size());
    for (int i = 0; i < input.size(); i++)
        block_to_3_matrix(input[i], Y_matrix[i].data, Cb_matrix[i].data, Cr_matrix[i].data);
}

////////////////////////////////////////////////////////////////

// 3 матрицы в один блок
void matrix_to_block(Block &output, std::vector<std::vector<double>> &Y_matrix, std::vector<std::vector<double>> &Cb_matrix, std::vector<std::vector<double>> &Cr_matrix)
{
    output.matrix_data.resize(8);

    for (int i = 0; i < 8; i++)
    {
        output.matrix_data[i].resize(8);
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            output.matrix_data[i][j].Y = Y_matrix[i][j];
            output.matrix_data[i][j].Cb = Cb_matrix[i][j];
            output.matrix_data[i][j].Cr = Cr_matrix[i][j];
        }
    }
}

void matrixes_to_block(std::vector<Block> &output, std::vector<Matrix> &Y_matrix, std::vector<Matrix> &Cb_matrix, std::vector<Matrix> &Cr_matrix)
{
    output.resize(Cb_matrix.size());

    for (int i = 0; i < output.size(); i++)
        matrix_to_block(output[i], Y_matrix[i].data, Cb_matrix[i].data, Cr_matrix[i].data);
}
