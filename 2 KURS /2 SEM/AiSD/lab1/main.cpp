#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>

#include "rle.h"

using namespace std;
void print(const vector<unsigned char> &tmp)
{
    for (int i = 0; i < tmp.size(); i++) 
    {
        printf("%.2X, ", tmp[i]);
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    vector<unsigned char> tmp = {0xCF, 0xCF, 0xCF, 0xCF, 0xCF};
    vector<unsigned char> tmp1;
    vector<unsigned char> tmp2;

    rle_compress_2_1(tmp, tmp1);
    cout << "исходная строка: ";
    print(tmp);
    cout << "сжатая строка: ";
    print(tmp1);

    rle_decompress_2_1(tmp1, tmp2);
    
    cout << "разжатая строка: ";
    print(tmp2);

    if (argc == 4) 
    {
        int mode = std::atoi(argv[1]); // 1 == compress, 2 == decompress
        std::string input_filename = argv[2];
        std::string output_filename = argv[3];
        size_t input_file_size = std::filesystem::file_size(input_filename);

        std::ifstream input_file( input_filename, std::ios::binary );

        std::vector<unsigned char> input_data;
        std::vector<unsigned char> output_data;
        input_data.resize(input_file_size);

        input_file.read((char*)input_data.data(), input_file_size);

        switch(mode) 
        {
            case 1: // compress 
            {
                rle_compress_2_1(input_data, output_data);
            }
            break;
            case 2: // compress 
            {
                rle_decompress_2_1(input_data, output_data);
            }
            break;
        }
        std::ofstream output_file( output_filename, std::ios::binary | std::ios::trunc);
        output_file.write((char*)output_data.data(), output_data.size());
    }

    return 0;
}