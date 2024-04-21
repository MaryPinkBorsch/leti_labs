#pragma once

#include "Employer_list.h"
#include "Worker_list.h"
#include "Vacansia_list.h"

struct Dogovor 
{
    EmployerNode * rabotodatel = nullptr; // работодатель
    WorkerNode * rabotnik = nullptr; // работник
    VacansiaNode * next = nullptr;// закрытая вакансия

    bool Read(std::string & filename);
    bool Write(std::string & filename);
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