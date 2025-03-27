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

    cout << endl;
    {
        std::string str_input = "abracadabrafffffaaaaaaaaaaaaaaaaaaaaaaa";
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

int run_compressors(std::string filename)
{
    // BWT
    {
        if (filename.find(".txt") != std::string::npos)
        {
            std::string input_f = filename;
            std::string output_f = filename + ".BWT.processed";
            std::string compressed_f = filename + ".BWT.compressed";
            BWT_compressor(input_f, compressed_f);
            BWT_decompressor(compressed_f, output_f);
            cout << endl;
        }
    }

    // HA
    {
        std::string input_f = filename;
        std::string output_f = filename + ".HA.processed";
        std::string compressed_f = filename + ".HA.compressed";
        HA_compressor(input_f, compressed_f);
        HA_decompressor(compressed_f, output_f);
        cout << endl;
    }

    // RLE
    {
        std::string input_f = filename;
        std::string output_f = filename + ".RLE.processed";
        std::string compressed_f = filename + ".RLE.compressed";
        RLE_compressor(input_f, compressed_f);
        RLE_decompressor(compressed_f, output_f);
        cout << endl;
    }

    // LZ78
    {
        std::string input_f = filename;
        std::string output_f = filename + ".LZ78.processed";
        std::string compressed_f = filename + ".LZ78.compressed";
        LZ78_compressor(input_f, compressed_f);
        LZ78_decompressor(compressed_f, output_f);
        cout << endl;
    }

    // LZ78 + HA
    {
        std::string input_f = filename;
        std::string output_f = filename + ".LZ78_HA.processed";
        std::string compressed_f = filename + ".LZ78_HA.compressed";
        LZ78_HA_compressor(input_f, compressed_f);
        LZ78_HA_decompressor(compressed_f, output_f);
        cout << endl;
    }

    // LZ77
    {
        std::string input_f = filename;
        std::string output_f = filename + ".LZ77.processed";
        std::string compressed_f = filename + ".LZ77.compressed";
        LZ77_compressor(input_f, compressed_f);
        LZ77_decompressor(compressed_f, output_f);
        cout << endl;
    }

    // LZ77 + HA
    {
        std::string input_f = filename;
        std::string output_f = filename + ".LZ77_HA.processed";
        std::string compressed_f = filename + ".LZ77_HA.compressed";
        LZ77_HA_compressor(input_f, compressed_f);
        LZ77_HA_decompressor(compressed_f, output_f);
        cout << endl;
    }

    // BWT + RLE
    {
        if (filename.find(".txt") != std::string::npos)
        {
            std::string input_f = filename;
            std::string output_f = filename + ".BWT_RLE.processed";
            std::string compressed_f = filename + ".BWT_RLE.compressed";
            BWT_RLE_compressor(input_f, compressed_f);
            BWT_RLE_decompressor(compressed_f, output_f);
            cout << endl;
        }
    }

    // BWT + MTF + HA
    {
        if (filename.find(".txt") != std::string::npos)
        {
            std::string input_f = filename;
            std::string output_f = filename + ".BWT_MTF_HA.processed";
            std::string compressed_f = filename + ".BWT_MTF_HA.compressed";
            BWT_MTF_HA_compressor(input_f, compressed_f);
            BWT_MTF_HA_decompressor(compressed_f, output_f);
            cout << endl;
        }
    }

    // BWT + MTF+ RLE + HA
    {
        if (filename.find(".txt") != std::string::npos)
        {
            std::string input_f = filename;
            std::string output_f = filename + ".BWT_MTF_RLE_HA.processed";
            std::string compressed_f = filename + ".BWT_MTF_RLE_HA.compressed";
            BWT_MTF_RLE_HA_compressor(input_f, compressed_f);
            BWT_MTF_RLE_HA_decompressor(compressed_f, output_f);
            cout << endl;
        }
    }
    return 0;
}
int main(int argc, char *argv[])
{
    std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
    run_compressors(input_f);
    input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/enwik7.txt";
    run_compressors(input_f);

    return 0;
};
