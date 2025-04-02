#include "bwt.h"
#include "mtf.h"

#include "serialization.h"

#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int G_BWT_BLOCK_SIZE = 64;

std::pair<std::vector<char>, int> transform_chunk(const std::vector<char> &chunk)
{
    size_t len = chunk.size();
    std::vector<std::string> rotations(len);

    // Создаем все ротации
    for (size_t i = 0; i < len; ++i)
    {
        rotations[i] = std::string(chunk.begin() + i, chunk.end()) + std::string(chunk.begin(), chunk.begin() + i);
    }

    // Сортируем ротации
    std::sort(rotations.begin(), rotations.end());

    // Получаем индекс оригинальной строки и закодированный блок
    int original_index = std::find(rotations.begin(), rotations.end(), std::string(chunk.begin(), chunk.end())) - rotations.begin();
    std::vector<char> encoded_chunk(len);

    for (size_t i = 0; i < len; ++i)
    {
        encoded_chunk[i] = rotations[i].back(); // Последний символ каждой ротации
    }

    return {encoded_chunk, original_index};
}

std::vector<char> reverse_transform_chunk(int original_index, const std::vector<char> &encoded_chunk)
{
    size_t len = encoded_chunk.size();

    // Создаем таблицу пар (символ, индекс)
    std::vector<std::pair<char, int>> table(len);

    for (int idx = 0; idx < len; ++idx)
    {
        table[idx] = {encoded_chunk[idx], idx};
    }

    // Сортируем таблицу по символам
    std::stable_sort(table.begin(), table.end(), [&](const std::pair<char, int> & lhs, const std::pair<char, int> & rhs ){ return (unsigned char)lhs.first < (unsigned char)rhs.first;});

    std::vector<char> result(len);

    int current_row = original_index;

    for (size_t i = 0; i < len; ++i)
    {
        result[i] = table[current_row].first;
        current_row = table[current_row].second;
    }

    return result;
}

std::pair<std::vector<char>, int> bwt(const std::vector<char> &data)
{
    std::vector<char> transformed_data;
    std::vector<int> indices;

    auto [encoded_chunk, index] = transform_chunk(data); // Получаем индекс и закодированный блок
    transformed_data.insert(transformed_data.end(), encoded_chunk.begin(), encoded_chunk.end());

    return {transformed_data, index};
}

std::vector<char> bwt_decode(const std::vector<char> &encoded_data, int & index)
{
    return reverse_transform_chunk(index, encoded_data);
}

void BWT_compress(std::vector<char> &input, std::vector<char> &output, bool mtf)
{
    for (int i = 0; i < input.size(); i += G_BWT_BLOCK_SIZE)
    {
        std::vector<char> block;
        std::vector<char>::iterator last_item = (i + G_BWT_BLOCK_SIZE < input.size()) ? (input.begin() + i + G_BWT_BLOCK_SIZE) : input.end();
        block.insert(block.begin(), input.begin() + i, last_item);
        auto tmp = bwt(block);
        if (mtf) 
        {
            std::vector<char> tmp2;
            MTF_compress(tmp.first, tmp2);
            serialize(output, tmp2);
            serialize(output, tmp.second);
        }
        else 
        {
            serialize(output, tmp.first);
            serialize(output, tmp.second);
        }
    }
}

void BWT_decompress(std::vector<char> &input, std::vector<char> &output, bool mtf)
{
    size_t offset = 0;
    while (offset < input.size())
    {
        if (mtf) 
        {
            std::vector<char> compressed_mtf_block;
            deserialize(input, compressed_mtf_block, offset);
            int index;
            deserialize(input, index, offset);
            std::vector<char> compressed_block;
            MTF_decompress(compressed_mtf_block, compressed_block);
            std::vector<char> decompressed_block = bwt_decode(compressed_block, index);
            output.insert(output.end(), decompressed_block.begin(), decompressed_block.end());
        }
        else 
        {
            std::vector<char> compressed_block;
            int index;
            deserialize(input, compressed_block, offset);
            deserialize(input, index, offset);
            std::vector<char> decompressed_block = bwt_decode(compressed_block, index);
            output.insert(output.end(), decompressed_block.begin(), decompressed_block.end());
        }
    }
}

void BWT_MTF_compress(std::vector<char> &input, std::vector<char> &output) 
{
    return BWT_compress(input, output, true);
}
void BWT_MTF_decompress(std::vector<char> &input, std::vector<char> &output) 
{
    return BWT_decompress(input, output, true);
}

