#pragma once
#include <string>

#include "F_fio.h"
#include "F_w_field.h"
#include "F_gorod.h"
#include "F_professia.h"

struct Employer_list
{
    F_fio F_I_O;
    F_w_field work_field; // рабочая область
    F_gorod adress;       // gorod
    int number;           // telephone

    Employer_list *next = 0;

    // F_professia wanted_professii; //
};