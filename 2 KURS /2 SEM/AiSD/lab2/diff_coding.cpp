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
// flag = 0 ---> обработка ДС
// flag = 1 ---> обработка АС
void var_code(std::vector<double> &input, std::vector<var_pair> &output, bool flag)
{
    // ОБРАБОТКА ДС
    // типо надо получить на вход число и записать его как пару:
    // где num = число битов
    //  arr = само число в бинарном виде
    if (flag == 0)
    {
        output.resize(input.size());
        for (int i = 0; i < input.size(); i++)
        {
            int t = 0;
            int num = input[i]; // надо ОБРАБЛТКУ ОТРИЦАТЕЛЬНЫХ ичсел
            if (num < 0)
            {
                output[i].positive = false;
                num *= -1;
            }
            else
                output[i].positive = true;
            while (num)
            {
                t = num % 2;
                num = num / 2;

                output[i].arr.push_back(t);
            }
            reverse(output[i].arr.begin(), output[i].arr.end()); // это чтоб оно было в правильном порядке
            output[i].num_bits = output[i].arr.size();
        }
    }
    // обработка АС (тут надо в итоге иметь тройку (а,б,в))
    // а = число повторений АС_num
    // б = сколько бит num
    // в = массив с бинарнйо формой arr
    else
    {
        int k = 0;
        output.resize(input.size() / 2);
        for (int i = 0; i < input.size(); i += 2)
        {
            if (k > output.size())
                abort;
            int t = 0;
            output[k].AC_num = input[i]; // записали количество повторений АС
            int num = input[i + 1];      // т.к  input[i] = это количесвто повторений РЛЕ
            if (num < 0)
            {
                output[k].positive = false;
                num *= -1;
            }
            else
                output[k].positive = true;

            if (num == 0)
            {
                output[k].num_bits = 1;
                output[k].arr = {0};
            }
            while (num)
            {
                t = num % 2;
                num = num / 2;

                output[k].arr.push_back(t);
            }
            reverse(output[k].arr.begin(), output[k].arr.end()); // это чтоб оно было в правильном порядке
            output[k].num_bits = output[k].arr.size();
            k++;
        }
    }
}
void var_decode(std::vector<var_pair> &input, std::vector<double> &output, bool flag)
{
    if (flag == 0)
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
            if (input[i].positive == false)
                tmp *= -1;
            output[i] = tmp;
        }
    }
    // обработка АС
    else
    {
        output.resize(input.size() * 2);
        int k = 0;
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
            if (input[i].positive == false)
                tmp *= -1;

            if (k > output.size())
                abort;
            output[k] = input[i].AC_num;
            output[k + 1] = tmp;
            k += 2;
        }
    }
}