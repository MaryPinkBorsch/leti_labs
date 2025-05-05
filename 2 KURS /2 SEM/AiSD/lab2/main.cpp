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

int main(int argc, char *argv[])
{
    // {
    //     int t = 0;
    //     int num = 51;
    //     while (num)
    //     {
    //         t = num % 2;
    //         num = num / 2;

    //         std::cout << t << " ";
    //     }
    //     cout << endl;

    //     vector<int> arr = {1, 1, 0, 0, 1, 1};
    //     double tmp = 0;
    //     int p = arr.size() - 1; // счетчик степени
    //     for (int j = 0; j < arr.size(); j++)
    //     {
    //         if (arr[j])
    //             tmp += pow(2, p);

    //         p--;
    //     }
    //     cout << tmp << endl;
    // }
    // return 0;

    // ТЕСТЫ
    {

        //     DCT_of_blocks(res_block, dct_block); // ПРИНМАЕТ ТОЛЬКО 8на8 БЛОКИ!
        //     rev_DCT_of_blocks(dct_block, rev);   //!!! ОН ОЧЕНЬ ПОЛОХО ПЕРЕВОДИТ ОТТУДА ОБРАТНО
        //     // типо погрешность есть, но что-то она большая слишком

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
            quantify_vec(Y_matrixes, 99,1);
            quantify_vec(Cb_matrixes, 99,0);
            quantify_vec(Cr_matrixes, 99,0);

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

        //return 0;

        std::vector<unsigned char> input_data;
        std::vector<unsigned char> pixel_data;
        std::vector<double> pixel_data_ycbcr;
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
        decodePNG(pixel_data, image_width, image_height, input_data.data(), input_data.size(), false);
        // перевели нашу пнг в массив, 1 элемент массива - 1 цветовой канал ргб, три канала подряд - 1 пиксель
        // там image_height строк, каждая длиной image_width, где каждый пиксель = 3 чара (на ргб)

        lodepng_encode24_file("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna2.png", pixel_data.data(), image_width, image_height);


        RGB_to_YCBRCR_vector(pixel_data, pixel_data_ycbcr);
        return 0;
    }
}