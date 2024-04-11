#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>


static const int N = 1000;
static const int M = 1000;

struct StrM
{
    char massiv[N + 1];
    char Marker;

    bool read_StrM(std::ifstream &input, std::ofstream &res);
    void print(std::ofstream &res);
};