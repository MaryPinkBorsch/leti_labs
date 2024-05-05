#pragma once

#include "StrL.h"

#include "common.h"
struct EmployerNode;

struct Vacansia
{
   // WorkField work_field; // рабочая область
    StrL professia;
   // Gorod adress; // gorod
    int salary;
    StrL education_lvl; // уровень образования

    EmployerNode *Rabotodatel = nullptr; //!!!!
    //F_fio Employerr;
    
    bool Read(std::ifstream & input, std::ofstream & log);
    bool Write(std::ofstream & output, std::ofstream & log);
    void Print(std::ofstream &log); 
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

    int num_vacansii = 0;
};