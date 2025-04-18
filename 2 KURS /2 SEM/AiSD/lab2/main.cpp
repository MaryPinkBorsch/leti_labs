#include "PicoPng.h"
#include "serialization.h"
#include "RGB_YCBCR.h"

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