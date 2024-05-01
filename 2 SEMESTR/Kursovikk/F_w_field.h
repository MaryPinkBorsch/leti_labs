#pragma once
#include "common.h"
enum WorkField
{
    STROITELSTVO,
    PROGRAMMIROVANIE,
    MEDICINA,
    RAZNOE
};

inline std::string W_field2String(WorkField w_f)
{
    switch (w_f)
    {
    case STROITELSTVO:
        return "Stroitelstvo";
    case PROGRAMMIROVANIE:
        return "Programmirovanie";
    case MEDICINA:
        return "Medizina";
    case RAZNOE:
        return "Raznoe";

    default:
        return "Raznoe";
    }
}

inline WorkField String2W_field(std::string w_f)
{
    if (w_f.compare("Stroitelstvo") == 0)
        return STROITELSTVO;
    else if (w_f.compare("Programmirovanie") == 0)
        return PROGRAMMIROVANIE;
    else if (w_f.compare("Medizina") == 0)
        return MEDICINA;
    else if (w_f.compare("Raznoe") == 0)
        return RAZNOE;

    else
        return RAZNOE;
}