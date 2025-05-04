#pragma once

#include "downsampling.h"
#include "diff_coding.h"

// таблицы с СТР. 150 с документа itf - ...

struct DC_table
{
    int Category = 0;
    int Len = 0;
    std::vector<int> Arr_Code = {};
};

struct AC_table
{
    int Run = 0;
    int Size = 0;
    int Len = 0;
    std::vector<int> Arr_Code = {};
};

// структура, хранящаяя код Хаффмана и число в бинарной форме
struct huff_pair
{
    std::vector<int> bit_arr; // само число в бинарной форме
    std::vector<int> h_code;  // массив с кодом хаффмана
    // костыли
    bool positive = true; // знак
    int AC_num = 0;       // для РЛЕ для АС
};

void table(std::vector<DC_table> &Y_table, std::vector<DC_table> &Cromatic_table, std::vector<AC_table> &y_tabl, std::vector<AC_table> &croma_tabl);
void huff_code(var_pair &input, huff_pair &output, bool flag, bool y_flag);
void huff_decode(huff_pair &input, var_pair &output, bool flag, bool y_flag);