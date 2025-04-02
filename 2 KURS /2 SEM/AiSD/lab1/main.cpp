#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "test_entropy.h"
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

int run_compressors(std::string filename, bool f_bmp)
{
    // // BWT
    // {
    //     // if (filename.find(".txt") != std::string::npos)
    //     {
    //         std::string input_f = filename;
    //         std::string output_f = filename + ".BWT.processed";
    //         std::string compressed_f = filename + ".BWT.compressed";
    //         if (f_bmp)
    //             output_f += ".bmp";
    //         BWT_compressor(input_f, compressed_f);
    //         BWT_decompressor(compressed_f, output_f);

    //         cout << endl;
    //     }
    // }

    // HA
    {
        std::string input_f = filename;
        std::string output_f = filename + ".HA.processed";
        std::string compressed_f = filename + ".HA.compressed";
        if (f_bmp)
            output_f += ".bmp";
        HA_compressor(input_f, compressed_f);
        HA_decompressor(compressed_f, output_f);

        cout << endl;
    }

    // RLE
    {
        std::string input_f = filename;
        std::string output_f = filename + ".RLE.processed";
        std::string compressed_f = filename + ".RLE.compressed";
        if (f_bmp)
            output_f += ".bmp";
        RLE_compressor(input_f, compressed_f);
        RLE_decompressor(compressed_f, output_f);

        cout << endl;
    }

    // LZ78
    {
        std::string input_f = filename;
        std::string output_f = filename + ".LZ78.processed";
        std::string compressed_f = filename + ".LZ78.compressed";
        if (f_bmp)
            output_f += ".bmp";
        LZ78_compressor(input_f, compressed_f);
        LZ78_decompressor(compressed_f, output_f);

        cout << endl;
    }

    // LZ78 + HA
    {
        std::string input_f = filename;
        std::string output_f = filename + ".LZ78_HA.processed";
        std::string compressed_f = filename + ".LZ78_HA.compressed";
        if (f_bmp)
            output_f += ".bmp";
        LZ78_HA_compressor(input_f, compressed_f);
        LZ78_HA_decompressor(compressed_f, output_f);

        cout << endl;
    }

    // LZ77
    {
        std::string input_f = filename;
        std::string output_f = filename + ".LZ77.processed";
        std::string compressed_f = filename + ".LZ77.compressed";
        if (f_bmp)
            output_f += ".bmp";
        LZ77_compressor(input_f, compressed_f);
        LZ77_decompressor(compressed_f, output_f);

        cout << endl;
    }

    // LZ77 + HA
    {
        std::string input_f = filename;
        std::string output_f = filename + ".LZ77_HA.processed";
        std::string compressed_f = filename + ".LZ77_HA.compressed";
        if (f_bmp)
            output_f += ".bmp";
        LZ77_HA_compressor(input_f, compressed_f);
        LZ77_HA_decompressor(compressed_f, output_f);

        cout << endl;
    }

    // BWT + RLE
    {
        {
            std::string input_f = filename;
            std::string output_f = filename + ".BWT_RLE.processed";
            std::string compressed_f = filename + ".BWT_RLE.compressed";
            if (f_bmp)
                output_f += ".bmp";
            BWT_RLE_compressor(input_f, compressed_f);
            BWT_RLE_decompressor(compressed_f, output_f);

            cout << endl;
        }
    }

    // BWT + MTF + HA
    {
        {
            std::string input_f = filename;
            std::string output_f = filename + ".BWT_MTF_HA.processed";
            std::string compressed_f = filename + ".BWT_MTF_HA.compressed";
            if (f_bmp)
                output_f += ".bmp";
            BWT_MTF_HA_compressor(input_f, compressed_f);
            BWT_MTF_HA_decompressor(compressed_f, output_f);

            cout << endl;
        }
    }

    // BWT + MTF+ RLE + HA
    {
        {
            std::string input_f = filename;
            std::string output_f = filename + ".BWT_MTF_RLE_HA.processed";
            std::string compressed_f = filename + ".BWT_MTF_RLE_HA.compressed";
            if (f_bmp)
                output_f += ".bmp";
            BWT_MTF_RLE_HA_compressor(input_f, compressed_f);
            BWT_MTF_RLE_HA_decompressor(compressed_f, output_f);
             
            cout << endl;
        }
    }
    return 0;
}

////////////////////////////////////
void entropy_test()
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/enwik7.txt", input_data);
    // string s = "";
    // input_data.insert(input_data.end(), s.begin(), s.end());
    cout << "BWT ИЗначальный размер " << input_data.size() << " байт" << endl;
    BWT_compress(input_data, tmp_data);

    string tmp;
    for (auto &it : tmp_data)
        tmp.push_back(it);
    unordered_map<char, double> probs;
    prob_estimate(tmp, probs);

    for (auto &it : probs)
    {
        cout << "Symbol " << it.first << " probability =  " << it.second << endl;
    }
    double HH;
    entropy(tmp, 1, HH);
    cout << "ENTROPY = " << HH << "  block size = " << G_BWT_BLOCK_SIZE << endl;

    cout << "BWT размер сжатого " << tmp_data.size() << " байт" << endl;
    writefile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/enwik7_test.txt", tmp_data);
}

void prob_estimate(string &s, unordered_map<char, double> &probs)
{
    if (!probs.empty())
        probs.clear();
    int len = s.size();
    if (len == 0)
        return;
    unordered_map<char, double> map;
    for (int i = 0; i < len; i++)
    {
        map[s[i]]++;
    }
    for (auto &it : map)
    {
        double tmp = it.second / len;
        probs[it.first] = tmp;
    }
}

void entropy(string &s, int code_len, double &entr)
{
    unordered_map<char, double> probs;
    prob_estimate(s, probs);
    if (probs.empty())
    {
        cout << "ПУстые вероятности!" << endl;
        return;
    }
    double H;
    for (auto &it : probs)
    {
        H += (code_len * it.second * log2(it.second));
        // cout << H << endl;
    }
    H *= -1;
    cout << "res H = " << H << endl;
    entr = H;
    // return H;
}

void LZ77_buff_test()
{
    {
        string s = "bananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkekbananapinklolkek";
        vector<char> input_data;
        vector<char> res;
        input_data.insert(input_data.end(), s.begin(), s.end());
        LZ77_compress(input_data, res);
        cout << "input size " << input_data.size() << endl
             << " compressed size " << res.size() << endl;
        double k = (double)input_data.size() / res.size();
        cout << "buff size = " << G_LZ77_WINDOW_SIZE << "   k = " << k << endl;
    }
}

int main(int argc, char *argv[])
{
    // {
    //     std::vector<char> input;
    //     std::vector<char> output;
    //     std::vector<char> tmp;

    //     readfile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp",input);

    //     cout << "BWT размер до сжатия: " << input.size() << endl;
    //     BWT_MTF_compress(input, tmp);

    //     cout << "BWT размер сжатого: " << tmp.size() << endl;

    //     BWT_MTF_decompress(tmp, output);

    //     cout << endl
    //             << "BWT размер после сжатия: " << output.size() << endl;

    //     writefile("/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscaleBWTtest.bmp",output);
    // }

    ////////////////////////////////////////////////////////
    cout << endl
         << endl
         << "IMAGE grayscale test" << endl;
    std::string input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie grayscale.bmp";
    run_compressors(input_f, 1);

    cout << endl
         << endl
         << "IMAGE monochrom test" << endl;
    input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie monochrome.bmp";
    run_compressors(input_f, 1);

    cout << endl
         << endl
         << "IMAGE color test" << endl;
    input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/barbie color.bmp";
    run_compressors(input_f, 1);

    cout << endl
         << endl
         << "ENWIK test" << endl;
    input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/enwik7.txt";
    run_compressors(input_f, 0);

    cout << endl
         << endl
         << "TEXT RUS test" << endl;
    input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/voina_i_mir.txt";
    run_compressors(input_f, 0);

    cout << endl
         << endl
         << "EXE FILE test" << endl;
    input_f = "/home/kalujny/work/leti_labs/2 KURS /2 SEM/AiSD/lab1/build/HEROES2W.EXE";
    run_compressors(input_f, 0);

    return 0;
    LZ77_buff_test();
    return 0;
    entropy_test();

    return 0;
}