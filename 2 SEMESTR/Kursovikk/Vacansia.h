#pragma once
#include <string>

#include "F_w_field.h"
#include "F_gorod.h"
#include "F_professia.h"
#include "F_education_lvl.h"
struct Vacansia
{
    F_w_field work_field; // рабочая область
    F_professia professia;
    F_gorod adress; // gorod
    int salary;
    F_education_lvl education_lvl; // уровень образования

    // F_offer  offers;
};