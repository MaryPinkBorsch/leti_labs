#pragma once

#include "downsampling.h"
struct var_pair
{
    int num = 0;           // число битов
    std::vector<int> arr;  // само число в бинарной форме
    bool positive = false; // костыль для определения знака
};

// разностное кодирование
void diff_code(std::vector<double> &DCs);
void diff_decode(std::vector<double> &DCs);

// переменное кодирование
// типо надо получить на вход число и записать его как пару: (а, б)
// где а = число битов
//  б = само число в бинарном виде
void var_code(std::vector<double> &input, std::vector<var_pair> &output);
void var_decode(std::vector<var_pair> &input, std::vector<double> &output);