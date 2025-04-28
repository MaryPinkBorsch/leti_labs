#include "diff_coding.h"

void diff_code(std::vector<double> &DCs)
{
    for (int i = 1; i < DCs.size() - 1; i++)
    {
        DCs[i] = DCs[i] - DCs[i - 1];
    }
}
void diff_decode(std::vector<double> &DCs)
{
    for (int i = 1; i < DCs.size() - 1; i++)
    {
        DCs[i] = DCs[i] + DCs[i - 1];
    }
}