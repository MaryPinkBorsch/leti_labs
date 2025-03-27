
#include "MEGA_compressors.h"
using namespace std;
// Хафман
void HA_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile(input_filename, input_data);
    cout << "HA ИЗначальный размер " << input_data.size() << " байт" << endl;
    HA_compress(input_data, tmp_data);
    cout << "HA размер сжатого " << tmp_data.size() << " байт" << endl;
    writefile(compressed_filename, tmp_data);
}
void HA_decompressor(std::string compressed_filename, std::string output_filename)
{

    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(compressed_filename, tmp_data);
    HA_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
}

// РЛЕ
void RLE_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile(input_filename, input_data);
    cout << "RLE ИЗначальный размер " << input_data.size() << " байт" << endl;
    rle_compress(input_data, tmp_data);
    cout << "RLE размер сжатого " << tmp_data.size() << " байт" << endl;
    writefile(compressed_filename, tmp_data);
}
void RLE_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(compressed_filename, tmp_data);
    rle_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
}

// БВТ + РЛЕ
void BWT_RLE_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "BWT_RLE ИЗначальный размер " << input_data.size() << " байт" << endl;
    BWT_compress(input_data, tmp_data);
    rle_compress(tmp_data, tmp_data2);
    cout << "BWT_RLE размер сжатого " << tmp_data2.size() << " байт" << endl;
    writefile(compressed_filename, tmp_data2);
}
void BWT_RLE_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(compressed_filename, input_data);
    rle_decompress(tmp_data2, tmp_data);
    BWT_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
}

// БВТ + МТФ + ХА
void BWT_MTF_HA_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> tmp_data3;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "BWT_MTF_HA ИЗначальный размер " << input_data.size() << " байт" << endl;
    BWT_compress(input_data, tmp_data);
    MTF_compress(tmp_data, tmp_data2);
    HA_compress(tmp_data2, tmp_data3);
    cout << "BWT_MTF_HA размер сжатого " << tmp_data3.size() << " байт" << endl;
    writefile(compressed_filename, tmp_data3);
}
void BWT_MTF_HA_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(compressed_filename, input_data);
    HA_decompress(input_data, tmp_data);
    MTF_decompress(tmp_data, tmp_data2);
    BWT_decompress(tmp_data2, output_data);
    writefile(output_filename, output_data);
}

// БВТ + МТФ + РЛЕ + ХА
void BWT_MTF_RLE_HA_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> tmp_data3;
    std::vector<char> tmp_data4;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "BWT_MTF_RLE_HA ИЗначальный размер " << input_data.size() << " байт" << endl;
    BWT_compress(input_data, tmp_data);
    MTF_compress(tmp_data, tmp_data2);
    rle_compress(tmp_data2, tmp_data3);
    HA_compress(tmp_data3, tmp_data4);
    cout << "BWT_MTF_RLE_HA размер сжатого " << tmp_data4.size() << " байт" << endl;
    writefile(compressed_filename, tmp_data4);
}
void BWT_MTF_RLE_HA_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> tmp_data3;
    std::vector<char> output_data;

    readfile(compressed_filename, input_data);
    HA_decompress(input_data, tmp_data);
    rle_decompress(tmp_data, tmp_data2);
    MTF_decompress(tmp_data2, tmp_data3);
    BWT_decompress(tmp_data3, output_data);
    writefile(output_filename, output_data);
}

// 78
void LZ78_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile(input_filename, input_data);
    cout << "LZ78 ИЗначальный размер " << input_data.size() << " байт" << endl;
    LZ78_compress(input_data, tmp_data);
    cout << "LZ78 размер сжатого " << tmp_data.size() << " байт" << endl;
    writefile(compressed_filename, tmp_data);
}
void LZ78_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(compressed_filename, tmp_data);
    LZ78_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
}

// 78 + Хаффман
void LZ78_HA_compressor(std::string input_filename, std::string compressed_filename)
{

    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "Lz78_HA ИЗначальный размер " << input_data.size() << " байт" << endl;
    LZ78_compress(input_data, tmp_data);
    HA_compress(tmp_data, tmp_data2);
    cout << "Lz78_HA размер сжатого " << tmp_data2.size() << " байт" << endl;
    writefile(compressed_filename, tmp_data2);
}
void LZ78_HA_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(compressed_filename, input_data);
    HA_decompress(tmp_data2, tmp_data);
    LZ78_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
    // cout << "Lz78_HA после Сжатие gray  размер: " << output_data.size() << endl;
}

// 77
void LZ77_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile(input_filename, input_data);
    cout << "LZ77 ИЗначальный размер " << input_data.size() << " байт" << endl;
    LZ77_compress(input_data, tmp_data);
    cout << "LZ77 размер сжатого " << tmp_data.size() << " байт" << endl;
    writefile(compressed_filename, tmp_data);
}
void LZ77_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(compressed_filename, tmp_data);
    LZ77_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
}

// 77 + Хаффман
void LZ77_HA_compressor(std::string input_filename, std::string compressed_filename)
{

    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "Lz77_HA ИЗначальный размер " << input_data.size() << " байт" << endl;
    LZ77_compress(input_data, tmp_data);
    HA_compress(tmp_data, tmp_data2);
    cout << "Lz77_HA размер сжатого " << tmp_data2.size() << " байт" << endl;
    writefile(compressed_filename, tmp_data2);
}
void LZ77_HA_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(compressed_filename, input_data);
    HA_decompress(tmp_data2, tmp_data);
    LZ77_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
    cout << "Lz77_HA после Сжатие gray  размер: " << output_data.size() << endl;
}
