#pragma once

#include "common.h"

struct StrL
{
    static const int N = 25;
    char massiv[N] = {0};
    int len = 0;
    StrL *next = nullptr;
    static const char Marker = '#'; // маркер конца считывания

    bool Read(std::ifstream &input, std::ofstream &log);
    bool Write(std::ofstream &output, std::ofstream &log);
    void Print(std::ofstream &log);
    void Vvod(std::ofstream &log);
    void Clear(std::ofstream &log);
    bool Equal(StrL& another, std::ofstream &log);
};
