#pragma once
#include <string>
// #include "F_w_field.h"
// #include "F_gorod.h"

#include "F_fio.h"
#include "F_professia.h"
#include "F_education_lvl.h"
struct Worker
{
    F_fio F_I_O;
    F_professia wanted_profession; // желаемая professia
    F_education_lvl education_lvl; // уровень образования
    int wanted_salary;             // желаемая z/p
};