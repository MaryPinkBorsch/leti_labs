#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <algorithm>

#include "rle.h"
#include "huffman.h"
#include "lz77.h"
#include "lz78.h"
#include "bwt.h"
#include "mtf.h"

#include "MEGA_compressors.h"
using namespace std;
template <typename T>
void print(const vector<T> &tmp)
{
    for (int i = 0; i < tmp.size(); i++)
    {
        printf("%.2X, ", tmp[i]);
    }
    cout << endl;
}

void TestBMP_chb()
{
    // std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome.bmp";
    // std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome1.bmp";
    // std::vector<unsigned char> input_data;
    // std::vector<unsigned char> tmp_data;
    // std::vector<unsigned char> output_data;
    // readfile(input_filename, input_data);
    // cout << "ИЗначальный размер " << input_data.size() << " байт" << endl;
    // rle_compress_2_1(input_data, tmp_data);
    // cout << "Сжатие чб 2.1 размер: " << tmp_data.size() << endl;
    // rle_decompress_2_1(tmp_data, output_data);
    // writefile(output_filename, output_data);
}

void TestBMP2_chb()
{
    std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome.bmp";
    std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome2_2.bmp";
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(input_filename, input_data);
    cout << "ИЗначальный размер " << input_data.size() << " байт" << endl;
    rle_compress(input_data, tmp_data);
    cout << "Сжатие чб 2.2 размер: " << tmp_data.size() << endl;
    rle_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
}

void TestBMP_gray()
{
    // std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
    // std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale1.bmp";
    // std::vector< char> input_data;
    // std::vector< char> tmp_data;
    // std::vector< char> output_data;
    // readfile(input_filename, input_data);
    // cout << "ИЗначальный размер " << input_data.size() << " байт" << endl;
    // rle_compress_2_1(input_data, tmp_data);
    // cout << "Сжатие gray 2.1 размер: " << tmp_data.size() << endl;
    // rle_decompress_2_1(tmp_data, output_data);
    // writefile(output_filename, output_data);
}

void TestBMP2_gray()
{
    std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
    std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale2_2.bmp";
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(input_filename, input_data);
    cout << "ИЗначальный размер " << input_data.size() << " байт" << endl;
    rle_compress(input_data, tmp_data);
    cout << "Сжатие gray 2.2 размер: " << tmp_data.size() << endl;
    rle_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
}

void TestBMP_gray_HA()
{
    std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
    std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale1_HA.bmp";
    std::vector<char> input_data;
    std::vector<char> compressed;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "ИЗначальный размер " << input_data.size() << " байт" << endl;
    HA_compress(input_data, compressed);
    cout << "Сжатие gray HUFFMAN размер: " << compressed.size() << " байт" << endl;
    HA_decompress(compressed, output_data);
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
    }
    {
        std::string input_str = "ilovepinkilovepinnkeeilovepinklolkekcheburek";
        std::vector<char> input;
        input.insert(input.end(), input_str.begin(), input_str.end());
        std::vector<char> compressed;
        std::vector<char> output;
        HA_compress(input, compressed);
        HA_decompress(compressed, output);
        if (input != output)
            abort();
    }
    cout << endl;
}

void TestLZ_78()
{
    {
        std::string str_input = "bananananananananananaaaaaaaaaaaaaaaaaaaaaaaaaaaaxyz";
        std::vector<char> input;
        std::vector<char> output;
        std::vector<char> tmp;
        input.insert(input.begin(), str_input.begin(), str_input.end());
        cout << "lz78 исходная строка: ";
        for (auto &it : input)
            cout << it;

        cout << endl
             << "lz78 размер до сжатия: " << input.size() << endl;
        LZ78_compress(input, tmp);
        cout << "lz78 размер сжатого: " << tmp.size() << endl;
        LZ78_decompress(tmp, output);

        cout << "lz78 сжатая строка: ";
        for (auto &it : output)
            cout << it;

        cout << endl
             << "lz78 размер после сжатия: " << output.size() << endl;
    }

    cout << endl;

    {
        std::string str_input = "abacabacabadaca";
        std::vector<char> input;
        std::vector<char> output;
        std::vector<char> tmp;
        input.insert(input.begin(), str_input.begin(), str_input.end());
        cout << "lz78 исходная строка: \t";
        for (auto &it : input)
            cout << it;

        cout << endl
             << "lz78 размер до сжатия: " << input.size() << endl;
        LZ78_compress(input, tmp);
        cout << "lz78 размер сжатого: " << tmp.size() << endl;
        LZ78_decompress(tmp, output);

        cout << "lz78 сжатая строка: \t";
        for (auto &it : output)
            cout << it;

        cout << endl
             << "lz78 размер после сжатия: " << output.size() << endl;
    }

    cout << endl;
}

void TestLZ_77()
{
    {
        std::string str_input = "bananananananananananaaaaaaaaaaaaaaaaaaaaaaaaaaaaxyz";
        std::vector<char> input;
        std::vector<char> output;
        std::vector<char> tmp;
        input.insert(input.begin(), str_input.begin(), str_input.end());
        cout << "lz77 исходная строка: ";
        for (auto &it : input)
            cout << it;

        cout << endl
             << "lz77 размер до сжатия: " << input.size() << endl;
        LZ77_compress(input, tmp);
        cout << "lz77 размер сжатого: " << tmp.size() * sizeof(LZ77_Node) << endl;
        LZ77_decompress(tmp, output);

        cout << "lz77 сжатая строка: ";
        for (auto &it : output)
            cout << it;

        cout << endl
             << "lz77 размер после сжатия: " << output.size() << endl;
    }

    cout << endl;

    {
        std::string str_input = "abacabacabadaca";
        std::vector<char> input;
        std::vector<char> output;
        std::vector<char> tmp;
        input.insert(input.begin(), str_input.begin(), str_input.end());
        cout << "lz77 исходная строка: \t";
        for (auto &it : input)
            cout << it;

        cout << endl
             << "lz77 размер до сжатия: " << input.size() << endl;
        LZ77_compress(input, tmp);
        cout << "lz77 размер сжатого: " << tmp.size() * sizeof(LZ77_Node) << endl;
        LZ77_decompress(tmp, output);

        cout << "lz77 сжатая строка: \t";
        for (auto &it : output)
            cout << it;

        cout << endl
             << "lz77 размер после сжатия: " << output.size() << endl;
    }

    cout << endl;
}

void TestBWT()
{
    {
        std::string str_input = "banana";
        std::vector<char> input;
        std::vector<char> output;
        std::vector<char> tmp;

        input.insert(input.begin(), str_input.begin(), str_input.end());
        cout << "BWT исходная строка: ";
        for (auto &it : input)
            cout << it;

        cout << endl
             << "BWT размер до сжатия: " << input.size() << endl;
        BWT_compress(input, tmp);

        cout << "BWT размер сжатого: " << tmp.size() << endl
             << "bwt преобразование: ";
        for (auto &it : tmp)
            cout << it;

        BWT_decompress(tmp, output);
        cout << endl;

        cout << "BWT разжатая строка: ";
        for (auto &it : output)
            cout << it;

        cout << endl
             << "BWT размер после сжатия: " << output.size() << endl;
    }

    cout << endl;
    {
        std::string str_input = "abracadabra";
        std::vector<char> input;
        std::vector<char> output;
        std::vector<char> tmp;

        input.insert(input.begin(), str_input.begin(), str_input.end());
        cout << "BWT исходная строка: ";
        for (auto &it : input)
            cout << it;

        cout << endl
             << "BWT размер до сжатия: " << input.size() << endl;
        BWT_compress(input, tmp);

        cout << "BWT размер сжатого: " << tmp.size() << endl
             << "bwt преобразование: ";
        for (auto &it : tmp)
            cout << it;

        BWT_decompress(tmp, output);
        cout << endl;

        cout << "BWT разжатая строка: ";
        for (auto &it : output)
            cout << it;

        cout << endl
             << "BWT размер после сжатия: " << output.size() << endl;
    }

    cout << endl;
}

void TestMTF()
{
    {
        std::string str_input = "banana";
        std::vector<char> input;
        std::vector<char> output;
        std::vector<char> tmp;

        input.insert(input.begin(), str_input.begin(), str_input.end());
        cout << "MTF исходная строка: ";
        for (auto &it : input)
            cout << it;

        cout << endl
             << "MTF размер до сжатия: " << input.size() << endl;
        MTF_compress(input, tmp);

        cout << "MTF размер сжатого: " << tmp.size() * sizeof(int) << endl
             << "MTF преобразование: ";
        for (auto &it : tmp)
            cout << it;

        MTF_decompress(tmp, output);
        cout << endl;

        cout << "MTF разжатая строка: ";
        for (auto &it : output)
            cout << it;

        cout << endl
             << "MTF размер после сжатия: " << output.size() << endl;
    }

    cout << endl;
    {
        std::string str_input = "abracadabra";
        std::vector<char> input;
        std::vector<char> output;
        std::vector<char> tmp;

        input.insert(input.begin(), str_input.begin(), str_input.end());
        cout << "MTF исходная строка: ";
        for (auto &it : input)
            cout << it;

        cout << endl
             << "MTF размер до сжатия: " << input.size() << endl;
        MTF_compress(input, tmp);

        cout << "MTF размер сжатого: " << tmp.size() * sizeof(int) << endl
             << "MTF преобразование: ";
        for (auto &it : tmp)
            cout << it;

        MTF_decompress(tmp, output);
        cout << endl;

        cout << "MTF разжатая строка: ";
        for (auto &it : output)
            cout << it;

        cout << endl
             << "MTF размер после сжатия: " << output.size() << endl;
    }
}

// первый тест матрешка
void TestBMP_gray_Lz78_HA()
{
    std::string input_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
    std::string output_filename = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscaleLZ78_HA.bmp";
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;
    readfile(input_filename, input_data);
    cout << "Lz78_HA ИЗначальный размер " << input_data.size() << " байт" << endl;
    LZ78_compress(input_data, tmp_data);
    HA_compress(tmp_data, tmp_data2);
    cout << "Lz78_HA размер сжатого " << tmp_data2.size() << " байт" << endl;

    HA_decompress(tmp_data2, tmp_data);
    LZ78_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
    cout << "Lz78_HA после Сжатие gray  размер: " << output_data.size() << endl;

    if (input_data.size() != output_data.size())
    {
        cout << "ОШИБКА TestBMP_gray_Lz78_HA!!!!" << endl;
    }
}

int main(int argc, char *argv[])
{
    TestLZ_78();
    TestBMP_gray_Lz78_HA();
    return 0;
    TestHuffman();

    return 0;
    LZ77_Node node_ololo;
    node_ololo.length = 666;
    node_ololo.offset = 777;
    node_ololo.next = 'y';
    LZ77_Node node_omg;
    node_omg.length = 1;
    node_omg.offset = 2;
    node_omg.next = '3';
    std::vector<LZ77_Node> nodes_wtf = {node_ololo, node_omg};
    std::vector<char> buffer;
    serialize(buffer, nodes_wtf);
    size_t offset = 0;
    std::vector<LZ77_Node> nodes_tmp;
    deserialize(buffer, nodes_tmp, offset);
    return 0;

    // TestMTF();

    // TestBWT();
    // TestLZ_77();

    // TestLZ_78();

    // TestHuffman();
    // TestBMP_gray_HA();

    // cout << "2.1" << endl
    //      << endl;
    // // 2.1
    // {
    //     vector<unsigned char> tmp = {0xCF, 0xCF, 0xCF, 0xCF, 0xCF};
    //     vector<unsigned char> tmp1;
    //     vector<unsigned char> tmp2;

    //     rle_compress_2_1(tmp, tmp1);
    //     cout << "исходная строка: ";
    //     print(tmp);
    //     cout << "сжатая строка: ";
    //     print(tmp1);

    //     rle_decompress_2_1(tmp1, tmp2);

    //     cout << "разжатая строка: ";
    //     print(tmp2);
    //     cout << endl
    //          << endl;
    // }
    // {
    //     vector<unsigned char> tmp = {0xCF, 0xCC, 0xCF, 0xCF, 0xCF};
    //     vector<unsigned char> tmp1;
    //     vector<unsigned char> tmp2;

    //     rle_compress_2_1(tmp, tmp1);
    //     cout << "исходная строка: ";
    //     print(tmp);
    //     cout << "сжатая строка: ";
    //     print(tmp1);

    //     rle_decompress_2_1(tmp1, tmp2);

    //     cout << "разжатая строка: ";
    //     print(tmp2);
    //     cout << endl
    //          << endl;
    // }
    cout << "2.2" << endl
         << endl;
    // 2.2
    {
        vector<char> tmp = {(char)0xCF, (char)0xCF, (char)0xCF, (char)0xCF, (char)0xCF};
        vector<char> tmp1;
        vector<char> tmp2;

        rle_compress(tmp, tmp1);
        cout << "исходная строка: ";
        print(tmp);
        cout << "сжатая строка: ";
        print(tmp1);

        rle_decompress(tmp1, tmp2);

        cout << "разжатая строка: ";
        print(tmp2);
        cout << endl
             << endl;
    }
    {
        vector<char> tmp = {(char)0xCF, (char)0xCC, (char)0xCF, (char)0xCF, (char)0xCF, (char)0xCA};
        vector<char> tmp1;
        vector<char> tmp2;

        rle_compress(tmp, tmp1);
        cout << "исходная строка: ";
        print(tmp);
        cout << "сжатая строка: ";
        print(tmp1);

        rle_decompress(tmp1, tmp2);

        cout << "разжатая строка: ";
        print(tmp2);
        cout << endl
             << endl;
    }
    return 0;

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
    // if (argc == 4)
    // {
    //     int mode = std::atoi(argv[1]); // 1 == compress, 2 == decompress
    //     std::string input_filename = argv[2];
    //     std::string output_filename = argv[3];
    //     size_t input_file_size = std::filesystem::file_size(input_filename);

    //     std::vector<unsigned char> input_data;
    //     std::vector<unsigned char> output_data;

    //     switch (mode)
    //     {
    //     case 1: // compress
    //     {
    //         readfile(input_filename, input_data);
    //         rle_compress_2_1(input_data, output_data);
    //         writefile(output_filename, output_data);
    //     }
    //     break;
    //     case 2: // compress
    //     {
    //         readfile(input_filename, input_data);
    //         rle_decompress_2_1(input_data, output_data);
    //         writefile(output_filename, output_data);
    //     }
    //     break;
    //     }
    // }

    return 0;
}