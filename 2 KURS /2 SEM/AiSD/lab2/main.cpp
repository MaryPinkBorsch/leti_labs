#include "PicoPng.h"
#include "downsampling.h"
#include "serialization.h"
#include "RGB_YCBCR.h"
#include "dct.h"
#include "quantify.h"
#include "diff_coding.h"
#include "rle.h"
#include "lodepng.h"

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

void test_compr()
{
    std::vector<unsigned char> input_data;
    std::vector<unsigned char> pixel_data;
    std::vector<unsigned char> output_data;
    std::vector<double> pixel_data_ycbcr;
    unsigned long image_width = 0;
    unsigned long image_height = 0;
    readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
    decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    unsigned long w = image_width;
    unsigned long h = image_height;
    std::vector<std::vector<Pixel>> pixel_arr;
    vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    std::vector<Block> res_block;
    blocking(w, h, pixel_arr, 8, res_block);

    std::vector<Block> dct_block;
    std::vector<Block> rev_dct_block;
    DCT_of_blocks(res_block, dct_block);
    // //
    std::vector<Matrix> Y_matrixes;
    std::vector<Matrix> Cb_matrixes;
    std::vector<Matrix> Cr_matrixes;
    // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    // квантую с коэфиицентом качестваы
    quantify_vec(Y_matrixes, 20, 1);
    quantify_vec(Cb_matrixes, 20, 0);
    quantify_vec(Cr_matrixes, 20, 0);
    //

    std::vector<double> Y_dc;
    std::vector<double> Cb_dc;
    std::vector<double> Cr_dc;
    get_DC(Y_matrixes, Y_dc);
    get_DC(Cb_matrixes, Cb_dc);
    get_DC(Cr_matrixes, Cr_dc);
    // кодирую DC
    diff_code(Y_dc);
    diff_code(Cb_dc);
    diff_code(Cr_dc);

    std::vector<double> Y_ac;
    std::vector<double> Cb_ac;
    std::vector<double> Cr_ac;
    get_AC(Y_matrixes, Y_ac);
    get_AC(Cb_matrixes, Cb_ac);
    get_AC(Cr_matrixes, Cr_ac);

    // RLE AC
    std::vector<double> Y_ac_rle;
    std::vector<double> Cb_ac_rle;
    std::vector<double> Cr_ac_rle;

    rle_AC(Y_ac, Y_ac_rle);
    rle_AC(Cb_ac, Cb_ac_rle);
    rle_AC(Cr_ac, Cr_ac_rle);

    // кодирую AC и ДС переменным кодированием
    std::vector<var_pair> Y_dc_var;
    std::vector<var_pair> Cb_dc_var;
    std::vector<var_pair> Cr_dc_var;
    var_code(Y_dc, Y_dc_var, 0);
    var_code(Cb_dc, Cb_dc_var, 0);
    var_code(Cr_dc, Cr_dc_var, 0);

    std::vector<var_pair> Y_ac_var;
    std::vector<var_pair> Cb_ac_var;
    std::vector<var_pair> Cr_ac_var;
    var_code(Y_ac_rle, Y_ac_var, 1);
    var_code(Cb_ac_rle, Cb_ac_var, 1);
    var_code(Cr_ac_rle, Cr_ac_var, 1);

    //
    std::vector<double> Y_ac_var2;
    std::vector<double> Cb_ac_var2;
    std::vector<double> Cr_ac_var2;
    var_decode(Y_ac_var, Y_ac_var2, 1);
    var_decode(Cb_ac_var, Cb_ac_var2, 1);
    var_decode(Cr_ac_var, Cr_ac_var2, 1);

    std::vector<double> Y_dc_var2;
    std::vector<double> Cb_dc_var2;
    std::vector<double> Cr_dc_var2;
    var_decode(Y_dc_var, Y_dc_var2, 0);
    var_decode(Cb_dc_var, Cb_dc_var2, 0);
    var_decode(Cr_dc_var, Cr_dc_var2, 0);

    std::vector<double> Y_ac_rle2;
    std::vector<double> Cb_ac_rle2;
    std::vector<double> Cr_ac_rle2;

    rev_rle_AC(Y_ac_var2, Y_ac_rle2);
    rle_AC(Cb_ac_var2, Cb_ac_rle2);
    rle_AC(Cr_ac_var2, Cr_ac_rle2);

    diff_decode(Y_dc_var2);
    diff_decode(Cb_dc_var2);
    diff_decode(Cr_dc_var2);

    std::vector<Matrix> Y_matrixes3;
    std::vector<Matrix> Cb_matrixes3;
    std::vector<Matrix> Cr_matrixes3;
    put_DC(Y_matrixes3, Y_dc_var2);
    put_DC(Cb_matrixes3, Cb_dc_var2);
    put_DC(Cr_matrixes3, Cr_dc_var2);

    put_AC(Y_matrixes3, Y_ac_rle2);
    put_AC(Cb_matrixes3, Cb_ac_rle2);
    put_AC(Cr_matrixes3, Cr_ac_rle2);

    // обраьное квантование
    // std::vector<Matrix> Y_matrixes2;
    // std::vector<Matrix> Cb_matrixes2;
    // std::vector<Matrix> Cr_matrixes2;
    dequantify_vec(Y_matrixes3, 20, 1);
    dequantify_vec(Cb_matrixes3, 20, 0);
    dequantify_vec(Cr_matrixes3, 20, 0);
    //

    //
    std::vector<Block> dct_block3;
    matrixes_to_block(dct_block3, Y_matrixes3, Cb_matrixes3, Cr_matrixes3);

    rev_DCT_of_blocks(dct_block3, rev_dct_block);

    unsigned long w2 = image_width;
    unsigned long h2 = image_height;
    std::vector<std::vector<Pixel>> pixel_arr2;
    deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    std::vector<double> pixel_data_ycbcr2;
    matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_c1.png", output_data.data(), w, h);
}

void test1()
{
    std::vector<unsigned char> input_data;
    std::vector<unsigned char> pixel_data;
    std::vector<unsigned char> output_data;
    std::vector<double> pixel_data_ycbcr;
    unsigned long image_width = 0;
    unsigned long image_height = 0;
    readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
    decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    unsigned long w = image_width;
    unsigned long h = image_height;
    std::vector<std::vector<Pixel>> pixel_arr;
    vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    std::vector<Block> res_block;
    blocking(w, h, pixel_arr, 8, res_block);

    std::vector<Block> dct_block;
    std::vector<Block> rev_dct_block;
    DCT_of_blocks(res_block, dct_block);
    // //
    std::vector<Matrix> Y_matrixes;
    std::vector<Matrix> Cb_matrixes;
    std::vector<Matrix> Cr_matrixes;
    // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    // квантую с коэфиицентом качестваы
    quantify_vec(Y_matrixes, 20, 1);
    quantify_vec(Cb_matrixes, 20, 0);
    quantify_vec(Cr_matrixes, 20, 0);
    //

    std::vector<double> Y_dc;
    std::vector<double> Cb_dc;
    std::vector<double> Cr_dc;
    get_DC(Y_matrixes, Y_dc);
    get_DC(Cb_matrixes, Cb_dc);
    get_DC(Cr_matrixes, Cr_dc);
    // кодирую DC
    diff_code(Y_dc);
    diff_code(Cb_dc);
    diff_code(Cr_dc);

    std::vector<double> Y_ac;
    std::vector<double> Cb_ac;
    std::vector<double> Cr_ac;
    get_AC(Y_matrixes, Y_ac);
    get_AC(Cb_matrixes, Cb_ac);
    get_AC(Cr_matrixes, Cr_ac);

    // RLE AC
    std::vector<double> Y_ac_rle;
    std::vector<double> Cb_ac_rle;
    std::vector<double> Cr_ac_rle;

    rle_AC(Y_ac, Y_ac_rle);
    rle_AC(Cb_ac, Cb_ac_rle);
    rle_AC(Cr_ac, Cr_ac_rle);

    // кодирую AC и ДС переменным кодированием
    std::vector<var_pair> Y_dc_var;
    std::vector<var_pair> Cb_dc_var;
    std::vector<var_pair> Cr_dc_var;
    var_code(Y_dc, Y_dc_var, 0);
    var_code(Cb_dc, Cb_dc_var, 0);
    var_code(Cr_dc, Cr_dc_var, 0);

    std::vector<var_pair> Y_ac_var;
    std::vector<var_pair> Cb_ac_var;
    std::vector<var_pair> Cr_ac_var;
    var_code(Y_ac_rle, Y_ac_var, 1);
    var_code(Cb_ac_rle, Cb_ac_var, 1);
    var_code(Cr_ac_rle, Cr_ac_var, 1);

    //
    std::vector<double> Y_ac_var2;
    std::vector<double> Cb_ac_var2;
    std::vector<double> Cr_ac_var2;
    var_decode(Y_ac_var, Y_ac_var2, 1);
    var_decode(Cb_ac_var, Cb_ac_var2, 1);
    var_decode(Cr_ac_var, Cr_ac_var2, 1);

    std::vector<double> Y_dc_var2;
    std::vector<double> Cb_dc_var2;
    std::vector<double> Cr_dc_var2;
    var_decode(Y_dc_var, Y_dc_var2, 0);
    var_decode(Cb_dc_var, Cb_dc_var2, 0);
    var_decode(Cr_dc_var, Cr_dc_var2, 0);

    std::vector<double> Y_ac_rle2;
    std::vector<double> Cb_ac_rle2;
    std::vector<double> Cr_ac_rle2;

    rev_rle_AC(Y_ac_var2, Y_ac_rle2);
    rle_AC(Cb_ac_var2, Cb_ac_rle2);
    rle_AC(Cr_ac_var2, Cr_ac_rle2);

    diff_decode(Y_dc_var2);
    diff_decode(Cb_dc_var2);
    diff_decode(Cr_dc_var2);

    std::vector<Matrix> Y_matrixes3;
    std::vector<Matrix> Cb_matrixes3;
    std::vector<Matrix> Cr_matrixes3;
    put_DC(Y_matrixes3, Y_dc_var2);
    put_DC(Cb_matrixes3, Cb_dc_var2);
    put_DC(Cr_matrixes3, Cr_dc_var2);

    put_AC(Y_matrixes3, Y_ac_rle2);
    put_AC(Cb_matrixes3, Cb_ac_rle2);
    put_AC(Cr_matrixes3, Cr_ac_rle2);

    // обраьное квантование
    // std::vector<Matrix> Y_matrixes2;
    // std::vector<Matrix> Cb_matrixes2;
    // std::vector<Matrix> Cr_matrixes2;
    dequantify_vec(Y_matrixes3, 20, 1);
    dequantify_vec(Cb_matrixes3, 20, 0);
    dequantify_vec(Cr_matrixes3, 20, 0);
    //

    //
    std::vector<Block> dct_block3;
    matrixes_to_block(dct_block3, Y_matrixes3, Cb_matrixes3, Cr_matrixes3);

    rev_DCT_of_blocks(dct_block3, rev_dct_block);

    unsigned long w2 = image_width;
    unsigned long h2 = image_height;
    std::vector<std::vector<Pixel>> pixel_arr2;
    deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    std::vector<double> pixel_data_ycbcr2;
    matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna9.png", output_data.data(), w, h);
}

void test2()
{
    std::vector<unsigned char> input_data;
    std::vector<unsigned char> pixel_data;
    std::vector<unsigned char> output_data;
    std::vector<double> pixel_data_ycbcr;
    unsigned long image_width = 0;
    unsigned long image_height = 0;
    readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
    decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    unsigned long w = image_width;
    unsigned long h = image_height;
    std::vector<std::vector<Pixel>> pixel_arr;
    vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    std::vector<Block> res_block;
    blocking(w, h, pixel_arr, 8, res_block);

    std::vector<Block> dct_block;
    std::vector<Block> rev_dct_block;
    DCT_of_blocks(res_block, dct_block);
    // //
    std::vector<Matrix> Y_matrixes;
    std::vector<Matrix> Cb_matrixes;
    std::vector<Matrix> Cr_matrixes;
    // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    // квантую с коэфиицентом качестваы
    quantify_vec(Y_matrixes, 20, 1);
    quantify_vec(Cb_matrixes, 20, 0);
    quantify_vec(Cr_matrixes, 20, 0);
    //

    std::vector<double> Y_dc;
    std::vector<double> Cb_dc;
    std::vector<double> Cr_dc;
    get_DC(Y_matrixes, Y_dc);
    get_DC(Cb_matrixes, Cb_dc);
    get_DC(Cr_matrixes, Cr_dc);

    std::vector<double> Y_dc3; // test NOT WORK
    get_DC(Y_matrixes, Y_dc3);

    // кодирую DC
    diff_code(Y_dc);
    diff_code(Cb_dc);
    diff_code(Cr_dc);

    std::vector<double> Y_ac;
    std::vector<double> Cb_ac;
    std::vector<double> Cr_ac;
    get_AC(Y_matrixes, Y_ac);
    get_AC(Cb_matrixes, Cb_ac);
    get_AC(Cr_matrixes, Cr_ac);

    // RLE AC
    std::vector<double> Y_ac_rle;
    std::vector<double> Cb_ac_rle;
    std::vector<double> Cr_ac_rle;

    rle_AC(Y_ac, Y_ac_rle);
    rle_AC(Cb_ac, Cb_ac_rle);
    rle_AC(Cr_ac, Cr_ac_rle);

    // кодирую AC и ДС переменным кодированием
    std::vector<var_pair> Y_dc_var;
    std::vector<var_pair> Cb_dc_var;
    std::vector<var_pair> Cr_dc_var;
    var_code(Y_dc, Y_dc_var, 0);
    var_code(Cb_dc, Cb_dc_var, 0);
    var_code(Cr_dc, Cr_dc_var, 0);

    std::vector<var_pair> Y_ac_var;
    std::vector<var_pair> Cb_ac_var;
    std::vector<var_pair> Cr_ac_var;
    var_code(Y_ac_rle, Y_ac_var, 1);
    var_code(Cb_ac_rle, Cb_ac_var, 1);
    var_code(Cr_ac_rle, Cr_ac_var, 1);

    //
    std::vector<double> Y_ac_var2;
    std::vector<double> Cb_ac_var2;
    std::vector<double> Cr_ac_var2;
    var_decode(Y_ac_var, Y_ac_var2, 1);
    var_decode(Cb_ac_var, Cb_ac_var2, 1);
    var_decode(Cr_ac_var, Cr_ac_var2, 1);

    if (Y_ac_var2 != Y_ac_rle)
        cout << "err 1 // var_code AC   ";

    std::vector<double> Y_dc_var2;
    std::vector<double> Cb_dc_var2;
    std::vector<double> Cr_dc_var2;
    var_decode(Y_dc_var, Y_dc_var2, 0);
    var_decode(Cb_dc_var, Cb_dc_var2, 0);
    var_decode(Cr_dc_var, Cr_dc_var2, 0);
    if (Y_dc_var2 != Y_dc)
        cout << "err 2 // var_code DC   ";

    std::vector<double> Y_ac_rle2;
    std::vector<double> Cb_ac_rle2;
    std::vector<double> Cr_ac_rle2;

    rev_rle_AC(Y_ac_var2, Y_ac_rle2);
    rev_rle_AC(Cb_ac_var2, Cb_ac_rle2);
    rev_rle_AC(Cr_ac_var2, Cr_ac_rle2);

    if (Y_ac_rle2 != Y_ac)
        cout << "err 3 // RLE   ";

    diff_decode(Y_dc_var2);
    diff_decode(Cb_dc_var2);
    diff_decode(Cr_dc_var2);
    if (Y_dc_var2 != Y_dc3)
        cout << "err 4 // diff dc   ";

    std::vector<Matrix> Y_matrixes3;
    std::vector<Matrix> Cb_matrixes3;
    std::vector<Matrix> Cr_matrixes3;
    put_DC(Y_matrixes3, Y_dc_var2);
    put_DC(Cb_matrixes3, Cb_dc_var2);
    put_DC(Cr_matrixes3, Cr_dc_var2);

    put_AC(Y_matrixes3, Y_ac_rle2);
    put_AC(Cb_matrixes3, Cb_ac_rle2);
    put_AC(Cr_matrixes3, Cr_ac_rle2);

    // обраьное квантование
    // std::vector<Matrix> Y_matrixes2;
    // std::vector<Matrix> Cb_matrixes2;
    // std::vector<Matrix> Cr_matrixes2;
    dequantify_vec(Y_matrixes3, 20, 1);
    dequantify_vec(Cb_matrixes3, 20, 0);
    dequantify_vec(Cr_matrixes3, 20, 0);
    //

    //
    std::vector<Block> dct_block3;
    matrixes_to_block(dct_block3, Y_matrixes3, Cb_matrixes3, Cr_matrixes3);

    rev_DCT_of_blocks(dct_block3, rev_dct_block);

    unsigned long w2 = image_width;
    unsigned long h2 = image_height;
    std::vector<std::vector<Pixel>> pixel_arr2;
    deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    std::vector<double> pixel_data_ycbcr2;
    matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna999.png", output_data.data(), w, h);
}

void q_test()
{
    {
        std::vector<unsigned char> input_data;
        std::vector<unsigned char> pixel_data;
        std::vector<unsigned char> output_data;
        std::vector<double> pixel_data_ycbcr;
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
        decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
        // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
        // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
        RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

        unsigned long w = image_width;
        unsigned long h = image_height;
        std::vector<std::vector<Pixel>> pixel_arr;
        vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

        std::vector<Block> res_block;
        blocking(w, h, pixel_arr, 8, res_block);

        std::vector<Block> dct_block;
        std::vector<Block> rev_dct_block;
        DCT_of_blocks(res_block, dct_block);
        // //
        std::vector<Matrix> Y_matrixes;
        std::vector<Matrix> Cb_matrixes;
        std::vector<Matrix> Cr_matrixes;
        // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
        blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
        // квантую с коэфиицентом качестваы
        quantify_vec(Y_matrixes, 100, 1);
        quantify_vec(Cb_matrixes, 100, 0);
        quantify_vec(Cr_matrixes, 100, 0);

        // обраьное квантование
        std::vector<Matrix> Y_matrixes2;
        std::vector<Matrix> Cb_matrixes2;
        std::vector<Matrix> Cr_matrixes2;
        dequantify_vec(Y_matrixes, 100, 1);
        dequantify_vec(Cb_matrixes, 100, 0);
        dequantify_vec(Cr_matrixes, 100, 0);
        //
        std::vector<Block> dct_block3;
        matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

        rev_DCT_of_blocks(dct_block3, rev_dct_block);

        unsigned long w2 = image_width;
        unsigned long h2 = image_height;
        std::vector<std::vector<Pixel>> pixel_arr2;
        deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

        std::vector<double> pixel_data_ycbcr2;
        matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
        YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
        lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_q1.png", output_data.data(), w, h);
    }
    // {
    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> pixel_data;
    //     std::vector<unsigned char> output_data;
    //     std::vector<double> pixel_data_ycbcr;
    //     unsigned long image_width = 0;
    //     unsigned long image_height = 0;
    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_black.png", input_data);
    //     decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    //     // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    //     // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    //     RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    //     unsigned long w = image_width;
    //     unsigned long h = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr;
    //     vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    //     std::vector<Block> res_block;
    //     blocking(w, h, pixel_arr, 8, res_block);

    //     std::vector<Block> dct_block;
    //     std::vector<Block> rev_dct_block;
    //     DCT_of_blocks(res_block, dct_block);
    //     // //
    //     std::vector<Matrix> Y_matrixes;
    //     std::vector<Matrix> Cb_matrixes;
    //     std::vector<Matrix> Cr_matrixes;
    //     // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    //     blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    //     // квантую с коэфиицентом качестваы
    //     quantify_vec(Y_matrixes, 100, 1);
    //     quantify_vec(Cb_matrixes, 100, 0);
    //     quantify_vec(Cr_matrixes, 100, 0);

    //     // обраьное квантование
    //     std::vector<Matrix> Y_matrixes2;
    //     std::vector<Matrix> Cb_matrixes2;
    //     std::vector<Matrix> Cr_matrixes2;
    //     dequantify_vec(Y_matrixes, 100, 1);
    //     dequantify_vec(Cb_matrixes, 100, 0);
    //     dequantify_vec(Cr_matrixes, 100, 0);
    //     //
    //     std::vector<Block> dct_block3;
    //     matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

    //     rev_DCT_of_blocks(dct_block3, rev_dct_block);

    //     unsigned long w2 = image_width;
    //     unsigned long h2 = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr2;
    //     deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    //     std::vector<double> pixel_data_ycbcr2;
    //     matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    //     YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    //     lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_black_q1.png", output_data.data(), w, h);
    // }
    // {
    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> pixel_data;
    //     std::vector<unsigned char> output_data;
    //     std::vector<double> pixel_data_ycbcr;
    //     unsigned long image_width = 0;
    //     unsigned long image_height = 0;
    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image.png", input_data);
    //     decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    //     // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    //     // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    //     RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    //     unsigned long w = image_width;
    //     unsigned long h = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr;
    //     vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    //     std::vector<Block> res_block;
    //     blocking(w, h, pixel_arr, 8, res_block);

    //     std::vector<Block> dct_block;
    //     std::vector<Block> rev_dct_block;
    //     DCT_of_blocks(res_block, dct_block);
    //     // //
    //     std::vector<Matrix> Y_matrixes;
    //     std::vector<Matrix> Cb_matrixes;
    //     std::vector<Matrix> Cr_matrixes;
    //     // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    //     blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    //     // квантую с коэфиицентом качестваы
    //     quantify_vec(Y_matrixes, 100, 1);
    //     quantify_vec(Cb_matrixes, 100, 0);
    //     quantify_vec(Cr_matrixes, 100, 0);

    //     // обраьное квантование
    //     std::vector<Matrix> Y_matrixes2;
    //     std::vector<Matrix> Cb_matrixes2;
    //     std::vector<Matrix> Cr_matrixes2;
    //     dequantify_vec(Y_matrixes, 100, 1);
    //     dequantify_vec(Cb_matrixes, 100, 0);
    //     dequantify_vec(Cr_matrixes, 100, 0);
    //     //
    //     std::vector<Block> dct_block3;
    //     matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

    //     rev_DCT_of_blocks(dct_block3, rev_dct_block);

    //     unsigned long w2 = image_width;
    //     unsigned long h2 = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr2;
    //     deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    //     std::vector<double> pixel_data_ycbcr2;
    //     matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    //     YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    //     lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image_q1.png", output_data.data(), w, h);
    // }
    // {
    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> pixel_data;
    //     std::vector<unsigned char> output_data;
    //     std::vector<double> pixel_data_ycbcr;
    //     unsigned long image_width = 0;
    //     unsigned long image_height = 0;
    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image_gray.png", input_data);
    //     decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    //     // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    //     // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    //     RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    //     unsigned long w = image_width;
    //     unsigned long h = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr;
    //     vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    //     std::vector<Block> res_block;
    //     blocking(w, h, pixel_arr, 8, res_block);

    //     std::vector<Block> dct_block;
    //     std::vector<Block> rev_dct_block;
    //     DCT_of_blocks(res_block, dct_block);
    //     // //
    //     std::vector<Matrix> Y_matrixes;
    //     std::vector<Matrix> Cb_matrixes;
    //     std::vector<Matrix> Cr_matrixes;
    //     // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    //     blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    //     // квантую с коэфиицентом качестваы
    //     quantify_vec(Y_matrixes, 100, 1);
    //     quantify_vec(Cb_matrixes, 100, 0);
    //     quantify_vec(Cr_matrixes, 100, 0);

    //     // обраьное квантование
    //     std::vector<Matrix> Y_matrixes2;
    //     std::vector<Matrix> Cb_matrixes2;
    //     std::vector<Matrix> Cr_matrixes2;
    //     dequantify_vec(Y_matrixes, 100, 1);
    //     dequantify_vec(Cb_matrixes, 100, 0);
    //     dequantify_vec(Cr_matrixes, 100, 0);
    //     //
    //     std::vector<Block> dct_block3;
    //     matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

    //     rev_DCT_of_blocks(dct_block3, rev_dct_block);

    //     unsigned long w2 = image_width;
    //     unsigned long h2 = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr2;
    //     deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    //     std::vector<double> pixel_data_ycbcr2;
    //     matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    //     YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    //     lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image_gray_q1.png", output_data.data(), w, h);
    // }
    // {
    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> pixel_data;
    //     std::vector<unsigned char> output_data;
    //     std::vector<double> pixel_data_ycbcr;
    //     unsigned long image_width = 0;
    //     unsigned long image_height = 0;
    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image_black.png", input_data);
    //     decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    //     // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    //     // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    //     RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    //     unsigned long w = image_width;
    //     unsigned long h = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr;
    //     vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    //     std::vector<Block> res_block;
    //     blocking(w, h, pixel_arr, 8, res_block);

    //     std::vector<Block> dct_block;
    //     std::vector<Block> rev_dct_block;
    //     DCT_of_blocks(res_block, dct_block);
    //     // //
    //     std::vector<Matrix> Y_matrixes;
    //     std::vector<Matrix> Cb_matrixes;
    //     std::vector<Matrix> Cr_matrixes;
    //     // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    //     blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    //     // квантую с коэфиицентом качестваы
    //     quantify_vec(Y_matrixes, 100, 1);
    //     quantify_vec(Cb_matrixes, 100, 0);
    //     quantify_vec(Cr_matrixes, 100, 0);

    //     // обраьное квантование
    //     std::vector<Matrix> Y_matrixes2;
    //     std::vector<Matrix> Cb_matrixes2;
    //     std::vector<Matrix> Cr_matrixes2;
    //     dequantify_vec(Y_matrixes, 100, 1);
    //     dequantify_vec(Cb_matrixes, 100, 0);
    //     dequantify_vec(Cr_matrixes, 100, 0);
    //     //
    //     std::vector<Block> dct_block3;
    //     matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

    //     rev_DCT_of_blocks(dct_block3, rev_dct_block);

    //     unsigned long w2 = image_width;
    //     unsigned long h2 = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr2;
    //     deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    //     std::vector<double> pixel_data_ycbcr2;
    //     matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    //     YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    //     lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image_black_q1.png", output_data.data(), w, h);
    // }
    // {
    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> pixel_data;
    //     std::vector<unsigned char> output_data;
    //     std::vector<double> pixel_data_ycbcr;
    //     unsigned long image_width = 0;
    //     unsigned long image_height = 0;
    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
    //     decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    //     // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    //     // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    //     RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    //     unsigned long w = image_width;
    //     unsigned long h = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr;
    //     vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    //     std::vector<Block> res_block;
    //     blocking(w, h, pixel_arr, 8, res_block);

    //     std::vector<Block> dct_block;
    //     std::vector<Block> rev_dct_block;
    //     DCT_of_blocks(res_block, dct_block);
    //     // //
    //     std::vector<Matrix> Y_matrixes;
    //     std::vector<Matrix> Cb_matrixes;
    //     std::vector<Matrix> Cr_matrixes;
    //     // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    //     blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    //     // квантую с коэфиицентом качестваы
    //     quantify_vec(Y_matrixes, 80, 1);
    //     quantify_vec(Cb_matrixes, 80, 0);
    //     quantify_vec(Cr_matrixes, 80, 0);

    //     // обраьное квантование
    //     std::vector<Matrix> Y_matrixes2;
    //     std::vector<Matrix> Cb_matrixes2;
    //     std::vector<Matrix> Cr_matrixes2;
    //     dequantify_vec(Y_matrixes, 80, 1);
    //     dequantify_vec(Cb_matrixes, 80, 0);
    //     dequantify_vec(Cr_matrixes, 80, 0);
    //     //
    //     std::vector<Block> dct_block3;
    //     matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

    //     rev_DCT_of_blocks(dct_block3, rev_dct_block);

    //     unsigned long w2 = image_width;
    //     unsigned long h2 = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr2;
    //     deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    //     std::vector<double> pixel_data_ycbcr2;
    //     matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    //     YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    //     lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_q2.png", output_data.data(), w, h);
    // }
    // {
    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> pixel_data;
    //     std::vector<unsigned char> output_data;
    //     std::vector<double> pixel_data_ycbcr;
    //     unsigned long image_width = 0;
    //     unsigned long image_height = 0;
    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
    //     decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    //     // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    //     // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    //     RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    //     unsigned long w = image_width;
    //     unsigned long h = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr;
    //     vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    //     std::vector<Block> res_block;
    //     blocking(w, h, pixel_arr, 8, res_block);

    //     std::vector<Block> dct_block;
    //     std::vector<Block> rev_dct_block;
    //     DCT_of_blocks(res_block, dct_block);
    //     // //
    //     std::vector<Matrix> Y_matrixes;
    //     std::vector<Matrix> Cb_matrixes;
    //     std::vector<Matrix> Cr_matrixes;
    //     // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    //     blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    //     // квантую с коэфиицентом качестваы
    //     quantify_vec(Y_matrixes, 60, 1);
    //     quantify_vec(Cb_matrixes, 60, 0);
    //     quantify_vec(Cr_matrixes, 60, 0);

    //     // обраьное квантование
    //     std::vector<Matrix> Y_matrixes2;
    //     std::vector<Matrix> Cb_matrixes2;
    //     std::vector<Matrix> Cr_matrixes2;
    //     dequantify_vec(Y_matrixes, 60, 1);
    //     dequantify_vec(Cb_matrixes, 60, 0);
    //     dequantify_vec(Cr_matrixes, 60, 0);
    //     //
    //     std::vector<Block> dct_block3;
    //     matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

    //     rev_DCT_of_blocks(dct_block3, rev_dct_block);

    //     unsigned long w2 = image_width;
    //     unsigned long h2 = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr2;
    //     deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    //     std::vector<double> pixel_data_ycbcr2;
    //     matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    //     YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    //     lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_q3.png", output_data.data(), w, h);
    // }
    // {
    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> pixel_data;
    //     std::vector<unsigned char> output_data;
    //     std::vector<double> pixel_data_ycbcr;
    //     unsigned long image_width = 0;
    //     unsigned long image_height = 0;
    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
    //     decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    //     // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    //     // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    //     RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    //     unsigned long w = image_width;
    //     unsigned long h = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr;
    //     vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    //     std::vector<Block> res_block;
    //     blocking(w, h, pixel_arr, 8, res_block);

    //     std::vector<Block> dct_block;
    //     std::vector<Block> rev_dct_block;
    //     DCT_of_blocks(res_block, dct_block);
    //     // //
    //     std::vector<Matrix> Y_matrixes;
    //     std::vector<Matrix> Cb_matrixes;
    //     std::vector<Matrix> Cr_matrixes;
    //     // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    //     blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    //     // квантую с коэфиицентом качестваы
    //     quantify_vec(Y_matrixes, 40, 1);
    //     quantify_vec(Cb_matrixes, 40, 0);
    //     quantify_vec(Cr_matrixes, 40, 0);

    //     // обраьное квантование
    //     std::vector<Matrix> Y_matrixes2;
    //     std::vector<Matrix> Cb_matrixes2;
    //     std::vector<Matrix> Cr_matrixes2;
    //     dequantify_vec(Y_matrixes, 40, 1);
    //     dequantify_vec(Cb_matrixes, 40, 0);
    //     dequantify_vec(Cr_matrixes, 40, 0);
    //     //
    //     std::vector<Block> dct_block3;
    //     matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

    //     rev_DCT_of_blocks(dct_block3, rev_dct_block);

    //     unsigned long w2 = image_width;
    //     unsigned long h2 = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr2;
    //     deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    //     std::vector<double> pixel_data_ycbcr2;
    //     matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    //     YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    //     lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_q4.png", output_data.data(), w, h);
    // }
    // {
    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> pixel_data;
    //     std::vector<unsigned char> output_data;
    //     std::vector<double> pixel_data_ycbcr;
    //     unsigned long image_width = 0;
    //     unsigned long image_height = 0;
    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
    //     decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    //     // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    //     // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    //     RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    //     unsigned long w = image_width;
    //     unsigned long h = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr;
    //     vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    //     std::vector<Block> res_block;
    //     blocking(w, h, pixel_arr, 8, res_block);

    //     std::vector<Block> dct_block;
    //     std::vector<Block> rev_dct_block;
    //     DCT_of_blocks(res_block, dct_block);
    //     // //
    //     std::vector<Matrix> Y_matrixes;
    //     std::vector<Matrix> Cb_matrixes;
    //     std::vector<Matrix> Cr_matrixes;
    //     // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    //     blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    //     // квантую с коэфиицентом качестваы
    //     quantify_vec(Y_matrixes, 20, 1);
    //     quantify_vec(Cb_matrixes, 20, 0);
    //     quantify_vec(Cr_matrixes, 20, 0);

    //     // обраьное квантование
    //     std::vector<Matrix> Y_matrixes2;
    //     std::vector<Matrix> Cb_matrixes2;
    //     std::vector<Matrix> Cr_matrixes2;
    //     dequantify_vec(Y_matrixes, 20, 1);
    //     dequantify_vec(Cb_matrixes, 20, 0);
    //     dequantify_vec(Cr_matrixes, 20, 0);
    //     //
    //     std::vector<Block> dct_block3;
    //     matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

    //     rev_DCT_of_blocks(dct_block3, rev_dct_block);

    //     unsigned long w2 = image_width;
    //     unsigned long h2 = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr2;
    //     deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    //     std::vector<double> pixel_data_ycbcr2;
    //     matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    //     YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    //     lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_q5.png", output_data.data(), w, h);
    // }
    // {
    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> pixel_data;
    //     std::vector<unsigned char> output_data;
    //     std::vector<double> pixel_data_ycbcr;
    //     unsigned long image_width = 0;
    //     unsigned long image_height = 0;
    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
    //     decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    //     // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    //     // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    //     RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    //     unsigned long w = image_width;
    //     unsigned long h = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr;
    //     vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    //     std::vector<Block> res_block;
    //     blocking(w, h, pixel_arr, 8, res_block);

    //     std::vector<Block> dct_block;
    //     std::vector<Block> rev_dct_block;
    //     DCT_of_blocks(res_block, dct_block);
    //     // //
    //     std::vector<Matrix> Y_matrixes;
    //     std::vector<Matrix> Cb_matrixes;
    //     std::vector<Matrix> Cr_matrixes;
    //     // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    //     blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    //     // квантую с коэфиицентом качестваы
    //     quantify_vec(Y_matrixes, 0, 1);
    //     quantify_vec(Cb_matrixes, 0, 0);
    //     quantify_vec(Cr_matrixes, 0, 0);

    //     // обраьное квантование
    //     std::vector<Matrix> Y_matrixes2;
    //     std::vector<Matrix> Cb_matrixes2;
    //     std::vector<Matrix> Cr_matrixes2;
    //     dequantify_vec(Y_matrixes, 0, 1);
    //     dequantify_vec(Cb_matrixes, 0, 0);
    //     dequantify_vec(Cr_matrixes, 0, 0);
    //     //
    //     std::vector<Block> dct_block3;
    //     matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

    //     rev_DCT_of_blocks(dct_block3, rev_dct_block);

    //     unsigned long w2 = image_width;
    //     unsigned long h2 = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr2;
    //     deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    //     std::vector<double> pixel_data_ycbcr2;
    //     matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    //     YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    //     lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_q6.png", output_data.data(), w, h);
    // }
    // /////////////////////
    // {
    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> pixel_data;
    //     std::vector<unsigned char> output_data;
    //     std::vector<double> pixel_data_ycbcr;
    //     unsigned long image_width = 0;
    //     unsigned long image_height = 0;
    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_gray.png", input_data);
    //     decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
    //     // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
    //     // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
    //     RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

    //     unsigned long w = image_width;
    //     unsigned long h = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr;
    //     vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

    //     std::vector<Block> res_block;
    //     blocking(w, h, pixel_arr, 8, res_block);

    //     std::vector<Block> dct_block;
    //     std::vector<Block> rev_dct_block;
    //     DCT_of_blocks(res_block, dct_block);
    //     // //
    //     std::vector<Matrix> Y_matrixes;
    //     std::vector<Matrix> Cb_matrixes;
    //     std::vector<Matrix> Cr_matrixes;
    //     // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
    //     blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
    //     // квантую с коэфиицентом качестваы
    //     quantify_vec(Y_matrixes, 100, 1);
    //     quantify_vec(Cb_matrixes, 100, 0);
    //     quantify_vec(Cr_matrixes, 100, 0);

    //     // обраьное квантование
    //     std::vector<Matrix> Y_matrixes2;
    //     std::vector<Matrix> Cb_matrixes2;
    //     std::vector<Matrix> Cr_matrixes2;
    //     dequantify_vec(Y_matrixes, 100, 1);
    //     dequantify_vec(Cb_matrixes, 100, 0);
    //     dequantify_vec(Cr_matrixes, 100, 0);
    //     //
    //     std::vector<Block> dct_block3;
    //     matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

    //     rev_DCT_of_blocks(dct_block3, rev_dct_block);

    //     unsigned long w2 = image_width;
    //     unsigned long h2 = image_height;
    //     std::vector<std::vector<Pixel>> pixel_arr2;
    //     deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

    //     std::vector<double> pixel_data_ycbcr2;
    //     matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
    //     YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
    //     lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_gray_q1.png", output_data.data(), w, h);
    // }
}

void Test()
{
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

        // !!!! надо сделать обработку если там нечетко делится на блоки Н!!!!

        // downsampling(w, h, res, 8);

        // redownsampling(w, h, res, 2); // рабоатет

        std::vector<Block> res_block;
        blocking(w, h, res, 8, res_block);
        std::vector<Block> dct_block;
        DCT_of_blocks(res_block, dct_block); //
        //
        std::vector<Matrix> Y_matrixes;
        std::vector<Matrix> Cb_matrixes;
        std::vector<Matrix> Cr_matrixes;
        // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
        blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
        // квантую с коэфиицентом качестваы
        quantify_vec(Y_matrixes, 99, 1);
        quantify_vec(Cb_matrixes, 99, 0);
        quantify_vec(Cr_matrixes, 99, 0);

        // подготовка к разностному кодированию DC
        std::vector<double> Y_dc;
        std::vector<double> Cb_dc;
        std::vector<double> Cr_dc;
        get_DC(Y_matrixes, Y_dc);
        get_DC(Cb_matrixes, Cb_dc);
        get_DC(Cr_matrixes, Cr_dc);
        // кодирую DC
        diff_code(Y_dc);
        diff_code(Cb_dc);
        diff_code(Cr_dc);

        std::vector<double> Y_ac;
        std::vector<double> Cb_ac;
        std::vector<double> Cr_ac;
        get_AC(Y_matrixes, Y_ac);
        get_AC(Cb_matrixes, Cb_ac);
        get_AC(Cr_matrixes, Cr_ac);

        // RLE AC
        std::vector<double> Y_ac_rle;
        std::vector<double> Cb_ac_rle;
        std::vector<double> Cr_ac_rle;

        rle_AC(Y_ac, Y_ac_rle);
        rle_AC(Cb_ac, Cb_ac_rle);
        rle_AC(Cr_ac, Cr_ac_rle);

        // кодирую AC и ДС переменным кодированием
        std::vector<var_pair> Y_dc_var;
        std::vector<var_pair> Cb_dc_var;
        std::vector<var_pair> Cr_dc_var;
        var_code(Y_dc, Y_dc_var, 0);
        var_code(Cb_dc, Cb_dc_var, 0);
        var_code(Cr_dc, Cr_dc_var, 0);

        std::vector<var_pair> Y_ac_var;
        std::vector<var_pair> Cb_ac_var;
        std::vector<var_pair> Cr_ac_var;
        var_code(Y_ac_rle, Y_ac_var, 1);
        var_code(Cb_ac_rle, Cb_ac_var, 1);
        var_code(Cr_ac_rle, Cr_ac_var, 1);
        std::vector<double> tmp;
        // var_decode(Cr_ac_var, tmp, 1); // ОНО РАБОТАЕТ!!!
        // if (tmp != Cr_ac_rle)
        //     cout << "ERROE" << endl;
    }
}

void test_22()
{
    {
        std::vector<unsigned char> input_data;
        std::vector<unsigned char> pixel_data;
        std::vector<unsigned char> output_data;
        std::vector<double> pixel_data_ycbcr;
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
        decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
        // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
        // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
        RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

        unsigned long w = image_width;
        unsigned long h = image_height;
        std::vector<std::vector<Pixel>> pixel_arr;
        vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

        std::vector<Block> res_block;
        blocking(w, h, pixel_arr, 8, res_block);

        std::vector<Block> dct_block;
        std::vector<Block> rev_dct_block;
        DCT_of_blocks(res_block, dct_block);
        // //
        std::vector<Matrix> Y_matrixes;
        std::vector<Matrix> Cb_matrixes;
        std::vector<Matrix> Cr_matrixes;
        // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
        blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
        // квантую с коэфиицентом качестваы
        quantify_vec(Y_matrixes, 20, 1);
        quantify_vec(Cb_matrixes, 20, 0);
        quantify_vec(Cr_matrixes, 20, 0);

        // обраьное квантование
        std::vector<Matrix> Y_matrixes2;
        std::vector<Matrix> Cb_matrixes2;
        std::vector<Matrix> Cr_matrixes2;
        dequantify_vec(Y_matrixes, 20, 1);
        dequantify_vec(Cb_matrixes, 20, 0);
        dequantify_vec(Cr_matrixes, 20, 0);
        //
        std::vector<Block> dct_block3;
        matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

        rev_DCT_of_blocks(dct_block3, rev_dct_block);

        unsigned long w2 = image_width;
        unsigned long h2 = image_height;
        std::vector<std::vector<Pixel>> pixel_arr2;
        deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

        std::vector<double> pixel_data_ycbcr2;
        matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
        YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
        lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_222.png", output_data.data(), w, h);
    }
}

void test_11()
{
    {
        std::vector<unsigned char> input_data;
        std::vector<unsigned char> pixel_data;
        std::vector<unsigned char> output_data;
        std::vector<double> pixel_data_ycbcr;
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
        decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
        // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
        // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
        RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

        unsigned long w = image_width;
        unsigned long h = image_height;
        std::vector<std::vector<Pixel>> pixel_arr;
        vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

        std::vector<Block> res_block;
        blocking(w, h, pixel_arr, 8, res_block);

        std::vector<Block> dct_block;
        std::vector<Block> rev_dct_block;
        DCT_of_blocks(res_block, dct_block);

        std::vector<Matrix> Y_matrixes;
        std::vector<Matrix> Cb_matrixes;
        std::vector<Matrix> Cr_matrixes;
        // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
        blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
        // квантую с коэфиицентом качестваы
        quantify_vec(Y_matrixes, 100, 1);
        quantify_vec(Cb_matrixes, 100, 0);
        quantify_vec(Cr_matrixes, 100, 0);

        // обраьное квантование
        std::vector<Matrix> Y_matrixes2;
        std::vector<Matrix> Cb_matrixes2;
        std::vector<Matrix> Cr_matrixes2;
        dequantify_vec(Y_matrixes, 100, 1);
        dequantify_vec(Cb_matrixes, 100, 0);
        dequantify_vec(Cr_matrixes, 100, 0);
        //
        std::vector<Block> dct_block3;
        matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

        rev_DCT_of_blocks(dct_block, rev_dct_block);

        unsigned long w2 = image_width;
        unsigned long h2 = image_height;
        std::vector<std::vector<Pixel>> pixel_arr2;
        deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

        std::vector<double> pixel_data_ycbcr2;
        matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
        YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
        lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_111.png", output_data.data(), w, h);
    }
    {
        std::vector<unsigned char> input_data;
        std::vector<unsigned char> pixel_data;
        std::vector<unsigned char> output_data;
        std::vector<double> pixel_data_ycbcr;
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_gray.png", input_data);
        decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
        // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
        // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
        RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

        unsigned long w = image_width;
        unsigned long h = image_height;
        std::vector<std::vector<Pixel>> pixel_arr;
        vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

        std::vector<Block> res_block;
        blocking(w, h, pixel_arr, 8, res_block);

        std::vector<Block> dct_block;
        std::vector<Block> rev_dct_block;
        DCT_of_blocks(res_block, dct_block);

        std::vector<Matrix> Y_matrixes;
        std::vector<Matrix> Cb_matrixes;
        std::vector<Matrix> Cr_matrixes;
        // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
        blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
        // квантую с коэфиицентом качестваы
        quantify_vec(Y_matrixes, 100, 1);
        quantify_vec(Cb_matrixes, 100, 0);
        quantify_vec(Cr_matrixes, 100, 0);

        // обраьное квантование
        std::vector<Matrix> Y_matrixes2;
        std::vector<Matrix> Cb_matrixes2;
        std::vector<Matrix> Cr_matrixes2;
        dequantify_vec(Y_matrixes, 100, 1);
        dequantify_vec(Cb_matrixes, 100, 0);
        dequantify_vec(Cr_matrixes, 100, 0);
        //
        std::vector<Block> dct_block3;
        matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

        rev_DCT_of_blocks(dct_block, rev_dct_block);

        unsigned long w2 = image_width;
        unsigned long h2 = image_height;
        std::vector<std::vector<Pixel>> pixel_arr2;
        deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

        std::vector<double> pixel_data_ycbcr2;
        matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
        YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
        lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_gray_111.png", output_data.data(), w, h);
    }
    {
        std::vector<unsigned char> input_data;
        std::vector<unsigned char> pixel_data;
        std::vector<unsigned char> output_data;
        std::vector<double> pixel_data_ycbcr;
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_black.png", input_data);
        decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
        // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
        // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
        RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

        unsigned long w = image_width;
        unsigned long h = image_height;
        std::vector<std::vector<Pixel>> pixel_arr;
        vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

        std::vector<Block> res_block;
        blocking(w, h, pixel_arr, 8, res_block);

        std::vector<Block> dct_block;
        std::vector<Block> rev_dct_block;
        DCT_of_blocks(res_block, dct_block);

        std::vector<Matrix> Y_matrixes;
        std::vector<Matrix> Cb_matrixes;
        std::vector<Matrix> Cr_matrixes;
        // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
        blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
        // квантую с коэфиицентом качестваы
        quantify_vec(Y_matrixes, 100, 1);
        quantify_vec(Cb_matrixes, 100, 0);
        quantify_vec(Cr_matrixes, 100, 0);

        // обраьное квантование
        std::vector<Matrix> Y_matrixes2;
        std::vector<Matrix> Cb_matrixes2;
        std::vector<Matrix> Cr_matrixes2;
        dequantify_vec(Y_matrixes, 100, 1);
        dequantify_vec(Cb_matrixes, 100, 0);
        dequantify_vec(Cr_matrixes, 100, 0);
        //
        std::vector<Block> dct_block3;
        matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

        rev_DCT_of_blocks(dct_block, rev_dct_block);

        unsigned long w2 = image_width;
        unsigned long h2 = image_height;
        std::vector<std::vector<Pixel>> pixel_arr2;
        deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

        std::vector<double> pixel_data_ycbcr2;
        matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
        YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
        lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna_black_111.png", output_data.data(), w, h);
    }
    {
        std::vector<unsigned char> input_data;
        std::vector<unsigned char> pixel_data;
        std::vector<unsigned char> output_data;
        std::vector<double> pixel_data_ycbcr;
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image_black.png", input_data);
        decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
        // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
        // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
        RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

        unsigned long w = image_width;
        unsigned long h = image_height;
        std::vector<std::vector<Pixel>> pixel_arr;
        vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

        std::vector<Block> res_block;
        blocking(w, h, pixel_arr, 8, res_block);

        std::vector<Block> dct_block;
        std::vector<Block> rev_dct_block;
        DCT_of_blocks(res_block, dct_block);

        std::vector<Matrix> Y_matrixes;
        std::vector<Matrix> Cb_matrixes;
        std::vector<Matrix> Cr_matrixes;
        // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
        blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
        // квантую с коэфиицентом качестваы
        quantify_vec(Y_matrixes, 100, 1);
        quantify_vec(Cb_matrixes, 100, 0);
        quantify_vec(Cr_matrixes, 100, 0);

        // обраьное квантование
        std::vector<Matrix> Y_matrixes2;
        std::vector<Matrix> Cb_matrixes2;
        std::vector<Matrix> Cr_matrixes2;
        dequantify_vec(Y_matrixes, 100, 1);
        dequantify_vec(Cb_matrixes, 100, 0);
        dequantify_vec(Cr_matrixes, 100, 0);
        //
        std::vector<Block> dct_block3;
        matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

        rev_DCT_of_blocks(dct_block, rev_dct_block);

        unsigned long w2 = image_width;
        unsigned long h2 = image_height;
        std::vector<std::vector<Pixel>> pixel_arr2;
        deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

        std::vector<double> pixel_data_ycbcr2;
        matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
        YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
        lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image_black_111.png", output_data.data(), w, h);
    }
    {
        std::vector<unsigned char> input_data;
        std::vector<unsigned char> pixel_data;
        std::vector<unsigned char> output_data;
        std::vector<double> pixel_data_ycbcr;
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image.png", input_data);
        decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
        // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
        // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
        RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

        unsigned long w = image_width;
        unsigned long h = image_height;
        std::vector<std::vector<Pixel>> pixel_arr;
        vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

        std::vector<Block> res_block;
        blocking(w, h, pixel_arr, 8, res_block);

        std::vector<Block> dct_block;
        std::vector<Block> rev_dct_block;
        DCT_of_blocks(res_block, dct_block);

        std::vector<Matrix> Y_matrixes;
        std::vector<Matrix> Cb_matrixes;
        std::vector<Matrix> Cr_matrixes;
        // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
        blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
        // квантую с коэфиицентом качестваы
        quantify_vec(Y_matrixes, 100, 1);
        quantify_vec(Cb_matrixes, 100, 0);
        quantify_vec(Cr_matrixes, 100, 0);

        // обраьное квантование
        std::vector<Matrix> Y_matrixes2;
        std::vector<Matrix> Cb_matrixes2;
        std::vector<Matrix> Cr_matrixes2;
        dequantify_vec(Y_matrixes, 100, 1);
        dequantify_vec(Cb_matrixes, 100, 0);
        dequantify_vec(Cr_matrixes, 100, 0);
        //
        std::vector<Block> dct_block3;
        matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

        rev_DCT_of_blocks(dct_block, rev_dct_block);

        unsigned long w2 = image_width;
        unsigned long h2 = image_height;
        std::vector<std::vector<Pixel>> pixel_arr2;
        deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

        std::vector<double> pixel_data_ycbcr2;
        matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
        YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
        lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image_111.png", output_data.data(), w, h);
    }
    {
        std::vector<unsigned char> input_data;
        std::vector<unsigned char> pixel_data;
        std::vector<unsigned char> output_data;
        std::vector<double> pixel_data_ycbcr;
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image_gray.png", input_data);
        decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
        // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
        // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)
        RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);

        unsigned long w = image_width;
        unsigned long h = image_height;
        std::vector<std::vector<Pixel>> pixel_arr;
        vector_2matrix(w, h, pixel_arr, pixel_data_ycbcr);

        std::vector<Block> res_block;
        blocking(w, h, pixel_arr, 8, res_block);

        std::vector<Block> dct_block;
        std::vector<Block> rev_dct_block;
        DCT_of_blocks(res_block, dct_block);

        std::vector<Matrix> Y_matrixes;
        std::vector<Matrix> Cb_matrixes;
        std::vector<Matrix> Cr_matrixes;
        // // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
        blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
        // квантую с коэфиицентом качестваы
        quantify_vec(Y_matrixes, 100, 1);
        quantify_vec(Cb_matrixes, 100, 0);
        quantify_vec(Cr_matrixes, 100, 0);

        // обраьное квантование
        std::vector<Matrix> Y_matrixes2;
        std::vector<Matrix> Cb_matrixes2;
        std::vector<Matrix> Cr_matrixes2;
        dequantify_vec(Y_matrixes, 100, 1);
        dequantify_vec(Cb_matrixes, 100, 0);
        dequantify_vec(Cr_matrixes, 100, 0);
        //
        std::vector<Block> dct_block3;
        matrixes_to_block(dct_block3, Y_matrixes, Cb_matrixes, Cr_matrixes);

        rev_DCT_of_blocks(dct_block, rev_dct_block);

        unsigned long w2 = image_width;
        unsigned long h2 = image_height;
        std::vector<std::vector<Pixel>> pixel_arr2;
        deblocking(w2, h2, pixel_arr2, 8, rev_dct_block);

        std::vector<double> pixel_data_ycbcr2;
        matrix2vector(w, h, pixel_data_ycbcr2, pixel_arr2);
        YCBRCR_to_RGB_vector(pixel_data_ycbcr2, output_data);
        lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/test_image_gray_111.png", output_data.data(), w, h);
    }
}

void test33()
{

    {
        vector<double> p = {0, 35, -88, 5.3};
        vector<double> p1 = p;

        diff_code(p1);
        diff_decode(p1);

        if (p1 != p)
            cout << " ERR 5 // diff code" << endl;
    }

    { // DC test
        std::vector<double> input = {1, -2, 3};
        std::vector<var_pair> output;
        var_code(input, output, 0);
        vector<double> p = {};
        var_decode(output, p, 0);

        if (input != p)
            cout << " ERR 6 //var DC code" << endl;
    }
    { // AC test
        std::vector<double> input = {1, 2, 3, -5};
        std::vector<var_pair> output;
        var_code(input, output, 1);
        vector<double> p = {};
        var_decode(output, p, 1);

        if (input != p)
            cout << " ERR 7 //var AC code" << endl;
    }

    { // AC test 2
        std::vector<double> input = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, -5};
        std::vector<var_pair> output;
        std::vector<double> rle = {};
        std::vector<double> res = {};

        rle_AC(input, rle);
        var_code(rle, output, 1);
        vector<double> p = {};
        var_decode(output, p, 1);
        rev_rle_AC(p, res);

        if (input != res)
            cout << " ERR 8 //var AC code+RLE" << endl;
    }
    {
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

        // !!!! надо сделать обработку если там нечетко делится на блоки Н!!!!

        // downsampling(w, h, res, 8);

        // redownsampling(w, h, res, 2); // рабоатет

        std::vector<Block> res_block;
        blocking(w, h, res, 8, res_block);
        std::vector<Block> dct_block;
        DCT_of_blocks(res_block, dct_block); //
        //
        std::vector<Matrix> Y_matrixes;
        std::vector<Matrix> Cb_matrixes;
        std::vector<Matrix> Cr_matrixes;
        std::vector<Block> dct_block1;
        // перед квантованием я все 3 канала для каждого блока переведу в раздельные матрицы
        blocks_to_matrixes(dct_block, Y_matrixes, Cb_matrixes, Cr_matrixes);
        matrixes_to_block(dct_block1, Y_matrixes, Cb_matrixes, Cr_matrixes);

        if (dct_block[0].matrix_data[0][0].Y != dct_block1[0].matrix_data[0][0].Y)
            cout << " ERR 9 // matr" << endl;
    }
}

int main(int argc, char *argv[])
{

    // test1();
    test_11();
    // test33();
    // test_22();
    // test2();

    return 0;
}
