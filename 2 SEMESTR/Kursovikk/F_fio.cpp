#include "F_fio.h"

using namespace std;

bool F_fio::Read(std::ifstream &input, std::ofstream &log)
{
    input >> familia;
    input >> imya;
    input >> otchestvo;
    return true;
}
bool F_fio::Write(std::ofstream &output, std::ofstream &log)
{
    output << familia;
    output << imya;
    output << otchestvo;
    return true;
}