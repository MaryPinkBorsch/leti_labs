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

int main(int argc, char *argv[])
{

    // HA
    {
        std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
        std::string output_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_HA.bmp";
        std::string compressed_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_HA_compr";
        HA_compressor(input_f, compressed_f);
        HA_decompressor(compressed_f, output_f);
        cout <<endl;
    }

    // RLE
    {
        std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
        std::string output_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_RLE.bmp";
        std::string compressed_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_RLE_compr";
        RLE_compressor(input_f, compressed_f);
        RLE_decompressor(compressed_f, output_f);
        cout <<endl;
    }

    

    // LZ78
    {
        std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
        std::string output_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_LZ78.bmp";
        std::string compressed_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_LZ78_compr";
        LZ78_compressor(input_f, compressed_f);
        LZ78_decompressor(compressed_f, output_f);
        cout <<endl;
    }

    // LZ78 + HA
    {
        std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
        std::string output_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_LZ78_HA.bmp";
        std::string compressed_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_LZ78_HA_compr";
        LZ78_HA_compressor(input_f, compressed_f);
        LZ78_HA_decompressor(compressed_f, output_f);
        cout <<endl;
    }

    // LZ77
    {
        std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
        std::string output_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_LZ77.bmp";
        std::string compressed_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_LZ77_compr";
        LZ77_compressor(input_f, compressed_f);
        LZ77_decompressor(compressed_f, output_f);
        cout <<endl;
    }

    // LZ77 + HA
    {
        std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
        std::string output_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_LZ77_HA.bmp";
        std::string compressed_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_LZ77_HA_compr";
        LZ77_HA_compressor(input_f, compressed_f);
        LZ77_HA_decompressor(compressed_f, output_f);
        cout <<endl;
    }

    // BWT + RLE
    {
        std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
        std::string output_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_BWT_RLE.bmp";
        std::string compressed_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_BWT_RLE_compr";
        BWT_RLE_compressor(input_f, compressed_f);
        BWT_RLE_decompressor(compressed_f, output_f);
        cout <<endl;
    }
    
    // BWT + MTF + HA
    {
        std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
        std::string output_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_BWT_MTF_HA.bmp";
        std::string compressed_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_BWT_MTF_HA_compr";
        BWT_MTF_HA_compressor(input_f, compressed_f);
        BWT_MTF_HA_decompressor(compressed_f, output_f);
        cout <<endl;
    }

    // BWT + MTF+ RLE + HA
    {
        std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
        std::string output_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_BWT_MTF_RLE_HA.bmp";
        std::string compressed_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/test_BWT_MTF_RLE_HA_compr";
        BWT_MTF_RLE_HA_compressor(input_f, compressed_f);
        BWT_MTF_RLE_HA_decompressor(compressed_f, output_f);
        cout <<endl;
    }
    return 0;
}