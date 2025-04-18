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

void readfile(std::string input_filename, std::vector<char> &buffer)
{
    size_t input_file_size = std::filesystem::file_size(input_filename);
    std::ifstream input_file(input_filename, std::ios::binary);
    buffer.resize(input_file_size);
    input_file.read((char *)buffer.data(), input_file_size);
}

void writefile(std::string output_filename, std::vector<char> &buffer)
{
    std::ofstream output_file(output_filename, std::ios::binary | std::ios::trunc);
    output_file.write((char *)buffer.data(), buffer.size());
}

int main(int c, char *argv[])
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab2/data/Lenna.png", input_data);
}