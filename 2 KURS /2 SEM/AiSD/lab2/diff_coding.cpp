#include "diff_coding.h"
using namespace std;
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

// переменное кодирование
void var_code(std::vector<double> &input, std::vector<var_pair> &output)
{
    // типо надо получить на вход число и записать его как пару: (а, б)
    // где а = число битов
    //  б = само число в бинарном виде
    output.resize(input.size());
    for (int i = 0; i < input.size(); i++)
    {
        int t = 0;
        int num = input[i];
        while (num)
        {
            t = num % 2;
            num = num / 2;

            output[i].a++;
            output[i].b.push_back(t);
        }
        reverse(output[i].b.begin(), output[i].b.end()); // это чтоб оно было в правильном порядке
    }
}
void var_decode(std::vector<var_pair> &input, std::vector<double> &output)
{
    ;
}