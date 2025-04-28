#pragma once
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <stdio.h>
// воспользуюсь структурой чтоб удобно работать с пикселями

struct Pixel
{
    double Y = 0;
    double Cb = 0;
    double Cr = 0;
};

struct Block
{
    std::vector<std::vector<Pixel>> matrix_data;
};

// типо мне надо получить данные ширина/высота, + вектор массив из писелей,
// получается будет 2мерный массив  (ВЫСОТА строк диной ДЛИНА) каждый элемент которого хранить У Сб и Ср
//  моя задача (по заданию в 2 раза) в Н раз сжать изображение
//  типо берем блоки Н х Н и внутри их делаем одоного цвета (значение цветогого канала)

// вернуть полагаю можно или 2мерный массив или снова вектор массив
//  как и приходящий после преобразования УсБсР

void vector_2matrix(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &out_data, std::vector<double> in_data);

// дата это наш матрица пикселей, Н это то во сколько надо сжать наше изображение
// нужно проверить что и ширина и высота кратны Н (иначе нормально не засемплить)
void redownsampling(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int H);
void downsampling(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int H);

void blocking(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int N, std::vector<Block> &blocks);