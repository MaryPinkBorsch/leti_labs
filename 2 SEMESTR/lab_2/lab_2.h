#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

static const int N = 100;
static const int M = 100;

struct StrM
{
    char massiv[N+1];
    char Marker;
    
    bool read_StrM(std::string filename, std::ofstream &res);
};

struct Text
{
    // строки из которых состоит текст. предложение может быть в нескольих строках подряд
    StrM stroki[M+1];

    bool read_file(std::string filename, std::ofstream &res);
};

struct Predlojenie_idx
{
    int stroka_idx_start; // номер строки в котором находится начало предложения
    int stroka_idx_end; // номер строки в котором находится конец предложения
    int stroka_smeschenie_start; // номер первого символа в предложения строке
    int stroka_smeschenie_end; // номер последнего символа в предложения строке

    int num_znaki;
};

