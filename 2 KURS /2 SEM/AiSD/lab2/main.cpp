#include "PicoPng.h"
#include "downsampling.h"
#include "serialization.h"
#include "RGB_YCBCR.h"
#include "dct.h"

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <algorithm>

using namespace std;

void readfile(std::string input_filename, std::vector<unsigned char> &buffer)
{
    size_t input_file_size = std::filesystem::file_size(input_filename);
    std::ifstream input_file(input_filename, std::ios::binary);
    buffer.resize(input_file_size);
    input_file.read((char *)buffer.data(), input_file_size);
}

void writefile(std::string output_filename, std::vector<unsigned char> &buffer)
{
    std::ofstream output_file(output_filename, std::ios::binary | std::ios::trunc);
    output_file.write((char *)buffer.data(), buffer.size());
}

int main(int argc, char *argv[])
{
    // std::vector<double> p = {9, 1, 1, 9, 0, 0, 9, 1, 1, 9, 0, 0, 9, 1, 1, 9, 0, 0, 9, 1, 1, 9, 0, 0, 9, 1, 1, 9, 0, 0, 9, 1, 1, 9, 0, 0, 9, 1, 1, 9, 0, 0, 9, 1, 1, 9, 0, 0};
    // std::vector<double> p = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15};
    std::vector<double> p = {};
    for (int i = 0; i < 256; i++)
    {
        p.push_back(i);
        p.push_back(i);
        p.push_back(i);
    }
    unsigned long w = 16;
    unsigned long h = 16;
    std::vector<std::vector<Pixel>> res;
    vector_2matrix(w, h, res, p);
    // downsampling(w, h, res, 8);
    // redownsampling(w, h, res, 2); // рабоатет
    std::vector<Block> res_block;
    blocking(w, h, res, 8, res_block);
    std::vector<Block> dct_block;
    DCT_of_blocks(res_block, dct_block); // тут преобразовываются Сб и Ср каналы
    // надо ли преобразовывать У????
    std::vector<Matrix> Y_matrixes;
    std::vector<Matrix> Cb_matrixes;
    std::vector<Matrix> Cr_matrixes;
    // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    return 0;

    std::vector<unsigned char> input_data;
    std::vector<unsigned char> pixel_data;
    std::vector<double> pixel_data_ycbcr;
    unsigned long image_width = 0;
    unsigned long image_height = 0;
    readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
    decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)

    RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);
    return 0;
}