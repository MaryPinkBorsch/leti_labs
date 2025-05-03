#pragma once

#include "downsampling.h"
struct var_pair
{
    int num_bits = 0;      // число битов
    std::vector<int> arr;  // само число в бинарной форме
    bool positive = false; // костыль для определения знака

    int AC_num = 0; // костыль для АС, это количество повторений АС РЛЕ символа
};

// разностное кодирование
void diff_code(std::vector<double> &DCs);
void diff_decode(std::vector<double> &DCs);

// переменное кодирование
void var_code(std::vector<double> &input, std::vector<var_pair> &output, bool flag);
void var_decode(std::vector<var_pair> &input, std::vector<double> &output, bool flag);