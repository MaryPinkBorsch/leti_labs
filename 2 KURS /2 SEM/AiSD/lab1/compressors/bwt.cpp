#include "bwt.h"

#include "serialization.h"

#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int G_BWT_BLOCK_SIZE = 64;

std::pair<std::vector<char>, std::vector<int>> transform_chunk(const std::vector<char> &chunk)
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

    return {encoded_chunk, {original_index}};
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
    std::sort(table.begin(), table.end());

    std::vector<char> result(len);

    int current_row = original_index;

    for (size_t i = 0; i < len; ++i)
    {
        result[i] = table[current_row].first;
        current_row = table[current_row].second;
    }

    return result;
}

std::pair<std::vector<char>, std::vector<int>> bwt(const std::vector<char> &data, size_t chunk_size = G_BWT_BLOCK_SIZE)
{
    std::vector<char> transformed_data;
    std::vector<int> indices;

    for (size_t start = 0; start < data.size(); start += chunk_size)
    {
        size_t end = std::min(start + chunk_size, data.size());
        std::vector<char> chunk(data.begin() + start, data.begin() + end);

        auto [encoded_chunk, index] = transform_chunk(chunk); // Получаем индекс и закодированный блок
        transformed_data.insert(transformed_data.end(), encoded_chunk.begin(), encoded_chunk.end());
        indices.push_back(index[0]); // Сохраняем индекс
    }

    return {transformed_data, indices};
}

std::vector<char> bwt_decode(const std::vector<char> &encoded_data, const std::vector<int> &indices, size_t chunk_size)
{
    std::vector<char> restored_data;
    size_t position = 0;
    for (size_t indd = 0; position < encoded_data.size(); ++indd)
    {
        size_t end = position + chunk_size;
        if (end > encoded_data.size())
            end = encoded_data.size();

        std::vector<char> chunk(encoded_data.begin() + position, encoded_data.begin() + end);
        int original_index = indices[indd]; // Получаем индекс исходной строки

        auto restored_chunk = reverse_transform_chunk(original_index, chunk);
        restored_data.insert(restored_data.end(), restored_chunk.begin(), restored_chunk.end());

        position += chunk_size;
    }

    return restored_data;
}

void BWT_compress_debug(std::vector<char> &input, std::vector<char> &output)
{
    for (int i = 0; i < input.size(); i += G_BWT_BLOCK_SIZE)
    {
        std::vector<char> block;
        std::vector<char> compressed_block;

        std::vector<char>::iterator last_item = (i + G_BWT_BLOCK_SIZE < input.size()) ? (input.begin() + i + G_BWT_BLOCK_SIZE) : input.end();
        block.insert(block.begin(), input.begin() + i, last_item);
        auto tmp = bwt(block, G_BWT_BLOCK_SIZE);
        serialize(output, tmp.first);
        serialize(output, tmp.second);

        std::vector<char> check_block = bwt_decode(tmp.first, tmp.second, G_BWT_BLOCK_SIZE);
        if (check_block != block)
            abort();
    }
}

void BWT_compress(std::vector<char> &input, std::vector<char> &output)
{
    for (int i = 0; i < input.size(); i += G_BWT_BLOCK_SIZE)
    {
        std::vector<char> block;
        std::vector<char> compressed_block;

        std::vector<char>::iterator last_item = (i + G_BWT_BLOCK_SIZE < input.size()) ? (input.begin() + i + G_BWT_BLOCK_SIZE) : input.end();
        block.insert(block.begin(), input.begin() + i, last_item);
        auto tmp = bwt(block, G_BWT_BLOCK_SIZE);
        serialize(output, tmp.first);
        serialize(output, tmp.second);
    }
}

void BWT_decompress(std::vector<char> &input, std::vector<char> &output)
{
    size_t offset = 0;
    while (offset < input.size())
    {
        std::vector<char> compressed_block;
        std::vector<int> indicies;
        deserialize(input, compressed_block, offset);
        deserialize(input, indicies, offset);
        std::vector<char> decompressed_block = bwt_decode(compressed_block, indicies, G_BWT_BLOCK_SIZE);
        output.insert(output.end(), decompressed_block.begin(), decompressed_block.end());
    }
}
