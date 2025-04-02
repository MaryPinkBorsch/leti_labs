
#include "MEGA_compressors.h"
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
// Хафман
void HA_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile(input_filename, input_data);
    cout << "HA до сжатия " << input_data.size() << " байт" << endl;
    HA_compress(input_data, tmp_data);
    cout << "HA размер сжатого " << tmp_data.size() << " байт" << endl;
    cout << "k = " << (double)input_data.size() / tmp_data.size()<< endl;
    writefile(compressed_filename, tmp_data);
}
void HA_decompressor(std::string compressed_filename, std::string output_filename)
{

    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(compressed_filename, tmp_data);
    HA_decompress(tmp_data, output_data);
    cout << "HA после сжатия " << output_data.size() << " байт" << endl;
    writefile(output_filename, output_data);
}

// РЛЕ
void RLE_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile(input_filename, input_data);
    cout << "RLE до сжатия " << input_data.size() << " байт" << endl;
    rle_compress(input_data, tmp_data);
    cout << "RLE размер сжатого " << tmp_data.size() << " байт" << endl;
    cout << "k = " << (double)input_data.size() / tmp_data.size()<< endl;
    writefile(compressed_filename, tmp_data);
}
void RLE_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(compressed_filename, tmp_data);
    rle_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
    cout << "RLE после сжатия " << output_data.size() << " байт" << endl;
}

// БВТ + РЛЕ
void BWT_RLE_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "BWT_RLE до сжатия " << input_data.size() << " байт" << endl;
    BWT_compress(input_data, tmp_data);
    rle_compress(tmp_data, tmp_data2);
    cout << "BWT_RLE размер сжатого " << tmp_data2.size() << " байт" << endl;
    cout << "k = " << (double)input_data.size() / tmp_data2.size()<< endl;
    writefile(compressed_filename, tmp_data2);
}
void BWT_RLE_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> output_data;

    readfile(compressed_filename, input_data);
    rle_decompress(input_data, tmp_data);
    BWT_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
    cout << "BWT_RLE после сжатия " << output_data.size() << " байт" << endl;
}

// БВТ + МТФ + ХА
void BWT_MTF_HA_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data3;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "BWT_MTF_HA до сжатия " << input_data.size() << " байт" << endl;
    BWT_MTF_compress(input_data, tmp_data);
    HA_compress(tmp_data, tmp_data3);
    cout << "BWT_MTF_HA размер сжатого " << tmp_data3.size() << " байт" << endl;
    cout << "k = " << (double)input_data.size() / tmp_data3.size()<< endl;
    writefile(compressed_filename, tmp_data3);
}
void BWT_MTF_HA_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> output_data;

    readfile(compressed_filename, input_data);
    HA_decompress(input_data, tmp_data);
    BWT_MTF_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
    cout << "BWT_MTF_HA после сжатия " << output_data.size() << " байт" << endl;
}

// БВТ + МТФ + РЛЕ + ХА
void BWT_MTF_RLE_HA_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data3;
    std::vector<char> tmp_data4;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "BWT_MTF_RLE_HA до сжатия " << input_data.size() << " байт" << endl;
    BWT_MTF_compress(input_data, tmp_data);
    rle_compress(tmp_data, tmp_data3);
    HA_compress(tmp_data3, tmp_data4);
    cout << "BWT_MTF_RLE_HA размер сжатого " << tmp_data4.size() << " байт" << endl;
    cout << "k = " << (double)input_data.size() / tmp_data4.size()<< endl;
    writefile(compressed_filename, tmp_data4);
}
void BWT_MTF_RLE_HA_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(compressed_filename, input_data);
    HA_decompress(input_data, tmp_data);
    rle_decompress(tmp_data, tmp_data2);
    BWT_MTF_decompress(tmp_data2, output_data);
    writefile(output_filename, output_data);
    cout << "BWT_MTF_RLE_HA после сжатия " << output_data.size() << " байт" << endl;
}

// 78
void LZ78_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile(input_filename, input_data);
    cout << "LZ78 до сжатия " << input_data.size() << " байт" << endl;
    LZ78_compress(input_data, tmp_data);
    cout << "LZ78 размер сжатого " << tmp_data.size() << " байт" << endl;
    cout << "k = " << (double)input_data.size() / tmp_data.size()<< endl;
    writefile(compressed_filename, tmp_data);
}
void LZ78_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(compressed_filename, tmp_data);
    LZ78_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
    cout << "LZ78 после сжатия " << output_data.size() << " байт" << endl;
}

// 78 + Хаффман
void LZ78_HA_compressor(std::string input_filename, std::string compressed_filename)
{

    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "Lz78_HA до сжатия " << input_data.size() << " байт" << endl;
    LZ78_compress(input_data, tmp_data);
    HA_compress(tmp_data, tmp_data2);
    cout << "Lz78_HA размер сжатого " << tmp_data2.size() << " байт" << endl;
    cout << "k = " << (double)input_data.size() / tmp_data2.size()<< endl;
    writefile(compressed_filename, tmp_data2);
}
void LZ78_HA_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(compressed_filename, input_data);
    HA_decompress(input_data, tmp_data);
    LZ78_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
    cout << "Lz78_HA после сжатия " << output_data.size() << " байт" << endl;
}

// 77
void LZ77_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile(input_filename, input_data);
    cout << "LZ77 до сжатия " << input_data.size() << " байт" << endl;
    LZ77_compress(input_data, tmp_data);
    cout << "LZ77 размер сжатого " << tmp_data.size() << " байт" << endl;
    cout << "k = " << (double)input_data.size() / tmp_data.size()<< endl;
    writefile(compressed_filename, tmp_data);
}
void LZ77_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(compressed_filename, tmp_data);
    LZ77_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
    cout << "LZ77 после сжатия " << output_data.size() << " байт" << endl;

}

// 77 + Хаффман
void LZ77_HA_compressor(std::string input_filename, std::string compressed_filename)
{

    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> tmp_data2;
    std::vector<char> output_data;

    readfile(input_filename, input_data);
    cout << "Lz77_HA до сжатия " << input_data.size() << " байт" << endl;
    LZ77_compress(input_data, tmp_data);
    HA_compress(tmp_data, tmp_data2);
    cout << "Lz77_HA размер сжатого " << tmp_data2.size() << " байт" << endl;
    cout << "k = " << (double)input_data.size() / tmp_data2.size()<< endl;
    writefile(compressed_filename, tmp_data2);
}
void LZ77_HA_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    std::vector<char> output_data;

    readfile(compressed_filename, input_data);
    HA_decompress(input_data, tmp_data);
    LZ77_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
    cout << "Lz77_HA после сжатия " << output_data.size() << " байт" << endl;
}

//////это для тестов///////////////////////////////////////////////////////////////////////////////////

void BWT_compressor(std::string input_filename, std::string compressed_filename)
{
    std::vector<char> input_data;
    std::vector<char> tmp_data;
    readfile(input_filename, input_data);
    cout << "BWT до сжатия " << input_data.size() << " байт" << endl;
    BWT_compress(input_data, tmp_data);
    cout << "BWT размер сжатого " << tmp_data.size() << " байт" << endl;
    cout << "k = " << (double)input_data.size() / tmp_data.size()<< endl;
    writefile(compressed_filename, tmp_data);
}
void BWT_decompressor(std::string compressed_filename, std::string output_filename)
{
    std::vector<char> tmp_data;
    std::vector<char> output_data;
    readfile(compressed_filename, tmp_data);
    BWT_decompress(tmp_data, output_data);
    writefile(output_filename, output_data);
}
