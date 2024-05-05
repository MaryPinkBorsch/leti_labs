#pragma once

#include "StrL.h"
#include "Vacansia_list.h"

#include "common.h"

struct Employer
{
    StrL F_I_O;
    StrL work_field; // рабочая область
    StrL adress;     // gorod

    F_Vacancia offered_vacansii; // формуляр на список предлагаемых вакансий

    bool Read(std::ifstream &input, std::ofstream &log);
    bool Write(std::ofstream &output, std::ofstream &log);
    void Print(std::ofstream &log);
};

struct EmployerNode
{
    Employer value;
    EmployerNode *next = nullptr;
};

struct F_Employer
{
    EmployerNode *head = nullptr;
    EmployerNode *cur = nullptr;

    int num_employers = 0;
};