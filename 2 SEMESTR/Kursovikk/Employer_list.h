#pragma once
#include <string>

#include "F_fio.h"
#include "F_w_field.h"
#include "F_gorod.h"
#include "F_professia.h"
#include "Vacansia_list.h"

struct Employer
{
    F_fio F_I_O;
    WorkField work_field; // рабочая область
    Gorod adress;       // gorod
    int number;           // telephone

    VacansiaNode * offered_vacansii=0;

   
    bool Read(std::ifstream & input, std::ofstream & log);
    bool Write(std::ofstream & output, std::ofstream & log); 
};

struct EmployerNode 
{
    Employer value;
    EmployerNode * next = nullptr;
};

struct F_Employer 
{
    EmployerNode * head = nullptr;
    EmployerNode * cur = nullptr;
};