#pragma once

#include "FormularH.h"

struct ListNodeV
{
    FormularH f_H;
    ListNodeV * next = nullptr;

    bool ReadV(std::ifstream & input, std::ofstream & res);

};
