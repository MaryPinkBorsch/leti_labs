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
    // типо надо получить на вход число и записать его как пару:
    // где num = число битов
    //  arr = само число в бинарном виде
    output.resize(input.size());
    for (int i = 0; i < input.size(); i++)
    {
        int t = 0;
        int num = input[i]; // надо ОБРАБЛТКУ ОТРИЦАТЕЛЬНЫХ ичсел
        while (num)
        {
            t = num % 2;
            num = num / 2;

            output[i].arr.push_back(t);
        }
        reverse(output[i].arr.begin(), output[i].arr.end()); // это чтоб оно было в правильном порядке
        output[i].num = output[i].arr.size();
    }
}
void var_decode(std::vector<var_pair> &input, std::vector<double> &output)
{
    output.resize(input.size());
    for (int i = 0; i < input.size(); i++)
    {
        double tmp = 0;
        int p = input[i].arr.size() - 1; // счетчик степени
        for (int j = 0; j < input[i].arr.size(); j++)
        {
            if (input[i].arr[j])
                tmp += pow(2, p);
            p--;
        }
        output[i] = tmp;
    }
}