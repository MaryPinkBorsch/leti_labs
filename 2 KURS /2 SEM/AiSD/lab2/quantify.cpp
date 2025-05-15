#include "quantify.h"
using namespace std;

void table(std::vector<std::vector<int>> &q_table, std::vector<std::vector<int>> &chroma_q_table)
{
    // таблица для У
    q_table.resize(8);

    q_table[0] = {16, 11, 10, 16, 124, 140, 151, 161};
    q_table[1] = {12, 12, 14, 19, 126, 158, 160, 155};
    q_table[2] = {14, 13, 16, 24, 140, 157, 169, 156};
    q_table[3] = {14, 17, 22, 29, 151, 187, 180, 162};
    q_table[4] = {18, 22, 37, 56, 168, 109, 103, 177};
    q_table[5] = {24, 35, 55, 64, 181, 104, 113, 192};
    q_table[6] = {49, 64, 78, 87, 103, 121, 120, 101};
    q_table[7] = {72, 92, 95, 98, 112, 100, 103, 199};

    // для Сб и Ср
    chroma_q_table = {
        {17, 18, 24, 47, 99, 99, 99, 99},
        {18, 21, 26, 66, 99, 99, 99, 99},
        {24, 26, 56, 99, 99, 99, 99, 99},
        {47, 66, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99}};
}

// q_lvl = уровень качества от 0 до 100
void quantify(Matrix &matrix, int q_lvl, bool flag_Y)
{

    // bool flag_Y = 0 ----> Cr Cb
    // bool flag_Y = 1 ----> Y
    std::vector<std::vector<int>> q_table;
    std::vector<std::vector<int>> chroma_q_table;
    table(q_table, chroma_q_table);

    for (int i = 0; i < matrix.data.size(); i++)
    {
        for (int j = 0; j < matrix.data[i].size(); j++)
        {

            double k = 0;
            // double s = 0;
            // if (q_lvl < 50)
            // {
            //     if (q_lvl == 0)
            //         s = 5000.0;
            //     else
            //         s = 5000.0 / q_lvl;
            // }
            // else
            // {
            //     s = 200.0 - 2.0 * q_lvl;
            // }
            // if (flag_Y)
            //     k = ((q_table[i][j] * s + 50.0) / 100.0);
            // else
            //     k = ((chroma_q_table[i][j] * s + 50.0) / 100.0);

            if (flag_Y)
                k = (q_table[i][j]);
            else
                k = (chroma_q_table[i][j]);

            //matrix.data[i][j] = round(matrix.data[i][j] / k * 100000.0) / 100000.0;
            matrix.data[i][j] = round(matrix.data[i][j] / k);
        }
    }
}

void quantify_vec(std::vector<Matrix> &matrixes, int q_lvl, bool flag_Y)
{
    for (int i = 0; i < matrixes.size(); i++)
        quantify(matrixes[i], q_lvl, flag_Y);
}

void dequantify(Matrix &matrix, int q_lvl, bool flag_Y)
{
    // bool flag_Y = 0 ----> Cr Cb
    // bool flag_Y = 1 ----> Y
    std::vector<std::vector<int>> q_table;
    std::vector<std::vector<int>> chroma_q_table;
    table(q_table, chroma_q_table);

    for (int i = 0; i < matrix.data.size(); i++)
    {
        for (int j = 0; j < matrix.data[i].size(); j++)
        {

            // double s = 0;
            // double k = 0;
            // if (q_lvl < 50)
            // {
            //     if (q_lvl == 0)
            //         s = 5000.0;
            //     else
            //         s = 5000.0 / q_lvl;
            // }
            // else
            // {
            //     s = 200.0 - 2 * q_lvl;
            // }
            // if (flag_Y)
            //     k = ((q_table[i][j] * s + 50.0) / 100.0);
            // else
            //     k = ((chroma_q_table[i][j] * s + 50.0) / 100.0);

            // matrix.data[i][j] = (matrix.data[i][j]) * k;

            double k = 0;
            if (flag_Y)
                k = (q_table[i][j]);
            else
                k = (chroma_q_table[i][j]);

            matrix.data[i][j] = (matrix.data[i][j]) * k;

        }
    }
}

void dequantify2(Matrix &matrix, int q_lvl, bool flag_Y)
{
    // bool flag_Y = 0 ----> Cr Cb
    // bool flag_Y = 1 ----> Y
    std::vector<std::vector<int>> q_table;
    std::vector<std::vector<int>> chroma_q_table;
    table(q_table, chroma_q_table);

    for (int i = 0; i < matrix.data.size(); i++)
    {
        for (int j = 0; j < matrix.data[i].size(); j++)
        {

            double k = 0;

            if (flag_Y)
                k = (q_table[i][j]);
            else
                k = (chroma_q_table[i][j]);

            matrix.data[i][j] = (matrix.data[i][j]) * k;
        }
    }
}

void dequantify_vec(std::vector<Matrix> &matrixes, int q_lvl, bool flag_Y)
{
    for (int i = 0; i < matrixes.size(); i++)
        dequantify(matrixes[i], q_lvl, flag_Y);
}

// функция чтоб получить все ДС коэффициенты (позиция [0][0] в каждой матрице цветовгого канала)
void get_DC(std::vector<Matrix> &matrixes, std::vector<double> &DCs)
{
    for (int i = 0; i < matrixes.size(); i++)
    {
        DCs.push_back(matrixes[i].data[0][0]);
    }
}

void put_DC(std::vector<Matrix> &matrixes, std::vector<double> &DCs)
{
    matrixes.resize(DCs.size());
    for (auto &m : matrixes)
        for (int i = 0; i < 8; i++)
        {
            m.data.resize(8);
            for (int j = 0; j < 8; j++)
            {
                m.data[i].resize(8);
            }
        }
    for (int i = 0; i < DCs.size(); i++)
    {
        matrixes[i].data[0][0] = DCs[i];
    }
}

// функция чтоб получить все АС коэффициенты ( все позиции кроме позиция [0][0] в каждой матрице цветовгого канала)
void get_AC(std::vector<Matrix> &matrixes, std::vector<double> &ACs)
{
    matrixes.resize(ACs.size() / 63);
    for (auto &m : matrixes)
        for (int i = 0; i < 8; i++)
        {
            m.data.resize(8);
            for (int j = 0; j < 8; j++)
            {
                m.data[i].resize(8);
            }
        }
    for (int i = 0; i < matrixes.size(); i++)
    {
        for (int j = 0; j < matrixes[i].data.size(); j++)
            for (int k = 0; k < matrixes[i].data[j].size(); k++) // не берем позицию 0 0 т.к это ДС коэффициент
            {
                if (j == 0 && k == 0)
                    continue;
                ACs.push_back(matrixes[i].data[j][k]);
            }
    }
}

void put_AC(std::vector<Matrix> &matrixes, std::vector<double> &ACs)
{

    for (int i = 0; i < ACs.size(); i++)
    {
        int matrix_idx = i / 63;
        int offset = i % 63;

        if (offset == 0)
        {
            matrixes.push_back({});
        }
        offset++;
        matrixes[matrix_idx].data[offset / 8][offset % 8] = ACs[i];
    }
}

///////////////////

void quantize_block(Matrix &matrix, int quality_level, bool flag_Y)
{
    // bool flag_Y = 0 ----> Cr Cb
    // bool flag_Y = 1 ----> Y
    std::vector<std::vector<int>> q_table;
    std::vector<std::vector<int>> chroma_q_table;
    table(q_table, chroma_q_table);
    // Проверка размеров матрицы (предполагаем, что размеры q_table и
    // chroma_q_table совпадают с matrix.data)
    if (matrix.data.size() != q_table.size() ||
        (matrix.data.size() > 0 && matrix.data[0].size() != q_table[0].size()))
    {
        std::cerr << "Ошибка: Несовместимые размеры матрицы и таблицы квантования."
                  << std::endl;
        return; // Или выбросить исключение
    }

    // Вычисление масштабного коэффициента (s)
    double s;
    if (quality_level < 50)
    {
        s = (quality_level == 0) ? 5000.0 : 5000.0 / quality_level;
    }
    else
    {
        s = 200.0 - 2.0 * quality_level;
    }

    for (size_t i = 0; i < matrix.data.size(); ++i)
    {
        for (size_t j = 0; j < matrix.data[i].size(); ++j)
        {
            double k; // Значение квантования для текущего элемента
            if (flag_Y)
            {
                k = ((q_table[i][j] * s + 50) / 100.0);
            }
            else
            {
                k = ((chroma_q_table[i][j] * s + 50) / 100.0);
            }

            // Проверка деления на ноль
            if (k == 0.0)
            {
                std::cerr << "Ошибка: Попытка деления на ноль. Измените таблицы "
                             "квантования."
                          << std::endl;
                return; // Или присвоить k минимальное допустимое значение (например, 1)
            }

            matrix.data[i][j] =
                matrix.data[i][j] / k; // Квантование (деление на k)
        }
    }
}

void dequantize_block(Matrix &matrix, int quality_level, bool flag_Y)
{

    // bool flag_Y = 0 ----> Cr Cb
    // bool flag_Y = 1 ----> Y
    std::vector<std::vector<int>> q_table;
    std::vector<std::vector<int>> chroma_q_table;
    table(q_table, chroma_q_table);
    // Проверка размеров матрицы (предполагаем, что размеры q_table и
    // chroma_q_table совпадают с matrix.data)
    if (matrix.data.size() != q_table.size() ||
        (matrix.data.size() > 0 && matrix.data[0].size() != q_table[0].size()))
    {
        std::cerr << "Ошибка: Несовместимые размеры матрицы и таблицы деквантования."
                  << std::endl;
        return; // Или выбросить исключение
    }

    // Вычисление масштабного коэффициента (s)
    double s;
    if (quality_level < 50)
    {
        s = (quality_level == 0) ? 5000.0 : 5000.0 / quality_level;
    }
    else
    {
        s = 200.0 - 2.0 * quality_level;
    }

    for (size_t i = 0; i < matrix.data.size(); ++i)
    {
        for (size_t j = 0; j < matrix.data[i].size(); ++j)
        {
            double k; // Значение деквантования для текущего элемента
            if (flag_Y)
            {
                k = ((q_table[i][j] * s + 50) / 100.0);
            }
            else
            {
                k = ((chroma_q_table[i][j] * s + 50) / 100.0);
            }

            // Проверка деления на ноль
            if (k == 0.0)
            {
                std::cerr << "Ошибка: k равно нулю при деквантовании" << std::endl;
                return; // Или присвоить k минимальное допустимое значение (например, 1)
            }
            matrix.data[i][j] =
                matrix.data[i][j] * k; // Деквантование (умножение на k)

            // Ограничение значений (предполагаем, что допустимый диапазон 0-255)
            matrix.data[i][j] = std::min(255.0, std::max(0.0, matrix.data[i][j]));
        }
    }
}