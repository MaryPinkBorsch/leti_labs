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

void TestBMP()
{
    std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome.bmp";
    std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome1.bmp";
    std::vector<unsigned char> input_data;
    std::vector<unsigned char> tmp_data;
    std::vector<unsigned char> output_data;
    readfile(input_filename, input_data);
    rle_compress_2_1(input_data, tmp_data);
    rle_decompress_2_1(tmp_data, output_data);
    writefile(output_filename, output_data);
}

int main(int argc, char *argv[])
{
    cout<<"2.1" << endl<< endl;
    //2.1
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
        cout << endl<< endl;
    }
    {
        vector<unsigned char> tmp = {0xCF, 0xCC, 0xCF, 0xCF, 0xCF};
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
        cout << endl<< endl;
    }
    cout<<"2.2" << endl<< endl;
    //2.2
    {
        vector<unsigned char> tmp = {0xCF, 0xCF, 0xCF, 0xCF, 0xCF};
        vector<unsigned char> tmp1;
        vector<unsigned char> tmp2;

        rle_compress_2_2(tmp, tmp1);
        cout << "исходная строка: ";
        print(tmp);
        cout << "сжатая строка: ";
        print(tmp1);

        rle_decompress_2_2(tmp1, tmp2);

        cout << "разжатая строка: ";
        print(tmp2);
        cout << endl<< endl;
    }
    {
        vector<unsigned char> tmp = {0xCF, 0xCC, 0xCF, 0xCF, 0xCF, 0xCA};
        vector<unsigned char> tmp1;
        vector<unsigned char> tmp2;

        rle_compress_2_2(tmp, tmp1);
        cout << "исходная строка: ";
        print(tmp);
        cout << "сжатая строка: ";
        print(tmp1);

        rle_decompress_2_2(tmp1, tmp2);

        cout << "разжатая строка: ";
        print(tmp2);
        cout << endl<< endl;
    }


    return 0;
    TestBMP();

    if (argc == 4)
    {
        int mode = std::atoi(argv[1]); // 1 == compress, 2 == decompress
        std::string input_filename = argv[2];
        std::string output_filename = argv[3];
        size_t input_file_size = std::filesystem::file_size(input_filename);

        std::vector<unsigned char> input_data;
        std::vector<unsigned char> output_data;

        switch (mode)
        {
        case 1: // compress
        {
            readfile(input_filename, input_data);
            rle_compress_2_1(input_data, output_data);
            writefile(output_filename, output_data);
        }
        break;
        case 2: // compress
        {
            readfile(input_filename, input_data);
            rle_decompress_2_1(input_data, output_data);
            writefile(output_filename, output_data);
        }
        break;
        }
    }

    return 0;
}