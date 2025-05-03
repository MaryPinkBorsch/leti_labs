#pragma once

#include "downsampling.h"

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