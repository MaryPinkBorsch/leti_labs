#pragma once

#include "Employer_list.h"
#include "Worker_list.h"
#include "Vacansia_list.h"

#include "common.h"

struct Dogovor 
{
    EmployerNode * rabotodatel = nullptr; // работодатель
    WorkerNode * rabotnik = nullptr; // работник
    VacansiaNode * next = nullptr;// закрытая вакансия

  
    bool Read(std::ifstream & input, std::ofstream & log);
    bool Write(std::ofstream & output, std::ofstream & log); 
};

struct DogovorNode 
{
    Dogovor value;
    DogovorNode * next = nullptr;
};

struct F_dogovor
{ 
    DogovorNode * head = nullptr;
    DogovorNode * cur = nullptr;
};