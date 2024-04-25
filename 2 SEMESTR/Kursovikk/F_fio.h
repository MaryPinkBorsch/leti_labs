#pragma once
#include <string>

struct F_fio
{
    std::string familia;
    std::string imya;
    std::string otchestvo;
    bool Read(std::ifstream &input, std::ofstream &log);
    bool Write(std::ofstream &output, std::ofstream &log);
};