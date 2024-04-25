#pragma once
#include <string>

#include "F_w_field.h"
#include "F_gorod.h"
#include "F_professia.h"
#include "F_education_lvl.h"
#include "Employer_list.h"

struct Vacansia
{
    WorkField work_field; // рабочая область
    Professia professia;
    Gorod adress; // gorod
    int salary;
    Obrazovanie education_lvl; // уровень образования

    EmployerNode *Rabotodatel = nullptr;

    
    bool Read(std::ifstream & input, std::ofstream & log);
    bool Write(std::ofstream & output, std::ofstream & log); 
};

struct VacansiaNode
{
    Vacansia value;
    VacansiaNode *next = nullptr;
};

struct F_Vacancia
{
    VacansiaNode *head = nullptr;
    VacansiaNode *cur = nullptr;
};