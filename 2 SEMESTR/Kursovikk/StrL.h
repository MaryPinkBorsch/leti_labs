#pragma once

#include "common.h"

struct StrL
{
    static const int N = 15;
    char massiv[N];
    int len = 0;
    StrL *next = nullptr;
    static const char Marker = '#'; // маркер конца считывания

    void Read(std::ifstream &input, std::ofstream &log);
    void Write(std::ofstream &output, std::ofstream &log);
    void Print(std::ofstream &log);
    void Clear(std::ofstream &log);
};
