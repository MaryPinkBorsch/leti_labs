#pragma once

#include "rle.h"
#include "huffman.h"
#include "lz77.h"
#include "lz78.h"
#include "bwt.h"
#include "mtf.h"

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

// Хаффман
void HA_compressor(std::string input_filename, std::string compressed_filename);
void HA_decompressor(std::string compressed_filename, std::string output_filename);

// РЛЕ
void RLE_compressor(std::string input_filename, std::string compressed_filename);
void RLE_decompressor(std::string compressed_filename, std::string output_filename);

// БВТ + РЛЕ
void BWT_RLE_compressor(std::string input_filename, std::string compressed_filename);
void BWT_RLE_decompressor(std::string compressed_filename, std::string output_filename);

// БВТ + МТФ + ХА
void BWT_MTF_HA_compressor(std::string input_filename, std::string compressed_filename);
void BWT_MTF_HA_decompressor(std::string compressed_filename, std::string output_filename);

// БВТ + МТФ + РЛЕ + ХА
void BWT_MTF_RLE_HA_compressor(std::string input_filename, std::string compressed_filename);
void BWT_MTF_RLE_HA_decompressor(std::string compressed_filename, std::string output_filename);

// ЛЗ 78
void LZ78_compressor(std::string input_filename, std::string compressed_filename);
void LZ78_decompressor(std::string compressed_filename, std::string output_filename);

// ЛЗ 78 + Хаффман
void LZ78_HA_compressor(std::string input_filename, std::string compressed_filename);
void LZ78_HA_decompressor(std::string compressed_filename, std::string output_filename);

// ЛЗ 77
void LZ77_compressor(std::string input_filename, std::string compressed_filename);
void LZ77_decompressor(std::string compressed_filename, std::string output_filename);

// ЛЗ 77 + Хаффман
void LZ77_HA_compressor(std::string input_filename, std::string compressed_filename);
void LZ77_HA_decompressor(std::string compressed_filename, std::string output_filename);
