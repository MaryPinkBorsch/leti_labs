#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <algorithm>

#include "rle.h"
#include "huffman.h"

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

void TestBMP_chb()
{
    std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome.bmp";
    std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome1.bmp";
    std::vector<unsigned char> input_data;
    std::vector<unsigned char> tmp_data;
    std::vector<unsigned char> output_data;
    readfile(input_filename, input_data);
    rle_compress_2_1(input_data, tmp_data);
    cout << "Сжатие чб 2.1 размер: " << tmp_data.size() << endl;
    rle_decompress_2_1(tmp_data, output_data);
    writefile(output_filename, output_data);
}

void TestBMP2_chb()
{
    std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome.bmp";
    std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome2_2.bmp";
    std::vector<unsigned char> input_data;
    std::vector<unsigned char> tmp_data;
    std::vector<unsigned char> output_data;
    readfile(input_filename, input_data);
    rle_compress_2_2(input_data, tmp_data);
    cout << "Сжатие чб 2.2 размер: " << tmp_data.size() << endl;
    rle_decompress_2_2(tmp_data, output_data);
    writefile(output_filename, output_data);
}

void TestBMP_gray()
{
    std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
    std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale1.bmp";
    std::vector<unsigned char> input_data;
    std::vector<unsigned char> tmp_data;
    std::vector<unsigned char> output_data;
    readfile(input_filename, input_data);
    rle_compress_2_1(input_data, tmp_data);
    cout << "Сжатие gray 2.1 размер: " << tmp_data.size() << endl;
    rle_decompress_2_1(tmp_data, output_data);
    writefile(output_filename, output_data);
}

void TestBMP2_gray()
{
    std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
    std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale2_2.bmp";
    std::vector<unsigned char> input_data;
    std::vector<unsigned char> tmp_data;
    std::vector<unsigned char> output_data;
    readfile(input_filename, input_data);
    rle_compress_2_2(input_data, tmp_data);
    cout << "Сжатие gray 2.2 размер: " << tmp_data.size() << endl;
    rle_decompress_2_2(tmp_data, output_data);
    writefile(output_filename, output_data);
}

void TestHuffman()
{
    {
        std::vector<char> input = {'b', 'a', 'n', 'a', 'n', 'a'};
        std::vector<HuffmanCode> table;
        HAT_node *root;
        HA_make_table(input, table, root);
        HA_print_table(table);
    }
    cout << endl;
    {
        std::string str_input = "bananananananananananaaaaaaaaaaaaaaaaaaaaaaaaaaaaxyz";
        std::vector<char> input;
        input.insert(input.begin(), str_input.begin(), str_input.end());
        std::vector<HuffmanCode> table;
        HAT_node *root;
        HA_make_table(input, table, root);
        HA_print_table(table);
    }
    cout << endl;
    {
        std::string str_input = "ilovepinklolkekcheburek";
        std::vector<char> input;
        input.insert(input.begin(), str_input.begin(), str_input.end());
        std::vector<HuffmanCode> table;
        HAT_node *root;
        HA_make_table(input, table, root);
        HA_print_table(table);

        Bitmap tmp_bitmap;
        tmp_bitmap.storage = {0};
        std::unordered_map<char, HuffmanCode*> table_idx;
        for (auto & it : table) 
        {
            table_idx[it.value] = &it;
        }

        tmp_bitmap.add_code(*table_idx['i']);
        tmp_bitmap.add_code(*table_idx['l']);
        tmp_bitmap.add_code(*table_idx['o']);
        tmp_bitmap.add_code(*table_idx['v']);
        tmp_bitmap.add_code(*table_idx['e']);
        tmp_bitmap.add_code(*table_idx['p']);
        tmp_bitmap.add_code(*table_idx['i']);
        tmp_bitmap.add_code(*table_idx['n']);
        tmp_bitmap.add_code(*table_idx['k']);

        int read_idx = 0;
        char val = ' ';
        std::string uncompressed = "";
        tmp_bitmap.get_next_symbol(read_idx,table,root,val);
        uncompressed += val;
        tmp_bitmap.get_next_symbol(read_idx,table,root,val);
        uncompressed += val;
        tmp_bitmap.get_next_symbol(read_idx,table,root,val);
        uncompressed += val;
        tmp_bitmap.get_next_symbol(read_idx,table,root,val);
        uncompressed += val;
        tmp_bitmap.get_next_symbol(read_idx,table,root,val);
        uncompressed += val;
        tmp_bitmap.get_next_symbol(read_idx,table,root,val);
        uncompressed += val;
        tmp_bitmap.get_next_symbol(read_idx,table,root,val);
        uncompressed += val;
        tmp_bitmap.get_next_symbol(read_idx,table,root,val);
        uncompressed += val;
        tmp_bitmap.get_next_symbol(read_idx,table,root,val);
        uncompressed += val;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    TestHuffman();

    return 0;
    cout << "2.1" << endl
         << endl;
    // 2.1
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
        cout << endl
             << endl;
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
        cout << endl
             << endl;
    }
    cout << "2.2" << endl
         << endl;
    // 2.2
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
        cout << endl
             << endl;
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
        cout << endl
             << endl;
    }

    // 2.3
    cout << "2.3" << endl;
    {
        vector<unsigned char> tmp = {0xCF, 0xCE, 0xCF, 0xCE, 0xCF, 0xCE};
        vector<unsigned char> tmp1;
        vector<unsigned char> tmp2;

        rle_compress_2_3<16>(tmp, tmp1);
        cout << "исходная строка: ";
        print(tmp);
        cout << "сжатая строка: ";
        print(tmp1);

        rle_decompress_2_3<16>(tmp1, tmp2);

        cout << "разжатая строка: ";
        print(tmp2);
        cout << endl
             << endl;
    }
    {
        vector<unsigned char> tmp = {0xCF, 0xCE, 0xCF, 0xCF, 0xCE, 0xCF};
        vector<unsigned char> tmp1;
        vector<unsigned char> tmp2;

        rle_compress_2_3<24>(tmp, tmp1);
        cout << "исходная строка: ";
        print(tmp);
        cout << "сжатая строка: ";
        print(tmp1);

        rle_decompress_2_3<24>(tmp1, tmp2);

        cout << "разжатая строка: ";
        print(tmp2);
        cout << endl
             << endl;
    }

    return 0;

    TestBMP_chb();
    TestBMP2_chb();
    TestBMP_gray();
    TestBMP2_gray();
    return 0;
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