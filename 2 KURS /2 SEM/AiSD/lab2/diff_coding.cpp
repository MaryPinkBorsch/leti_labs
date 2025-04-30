#include "diff_coding.h"

void diff_code(std::vector<double> &DCs)
{
    if (DCs.empty())
        abort();
    for (int i = 1; i < DCs.size() - 1; i++)
    {
        DCs[i] = DCs[i] - DCs[i - 1];
    }
}
void diff_decode(std::vector<double> &DCs)
{
    if (DCs.empty())
        abort();
    for (int i = DCs.size() - 2; i > 0; i--)
    {
        DCs[i] = DCs[i] + DCs[i - 1];
    }
}