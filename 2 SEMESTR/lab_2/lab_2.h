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
    
    bool read_StrM(std::ifstream & input, std::ofstream &res);
    void print1(std::ofstream &res);
};

struct Predlojenie_idx
{
    int stroka_idx_start = -1; // номер строки в котором находится начало предложения
    int stroka_idx_end = -1; // номер строки в котором находится конец предложения
    int stroka_smeschenie_start = -1; // номер первого символа в предложения строке
    int stroka_smeschenie_end = -1; // номер последнего символа в предложения строке

    int num_znaki = 0;
};

struct Text
{
    // строки из которых состоит текст. предложение может быть в нескольих строках подряд
    StrM stroki[M];
    Predlojenie_idx indexi_predlojenii[M];

    int num_stroki;
    int num_predlojenia;

    bool read_file(std::string filename, std::ofstream &res);
    void print2(std::ofstream &res);
    void process_znaki(std::ofstream &res);
    bool IsRazdelitel(char c);
    bool IsZnak(char c);
};

