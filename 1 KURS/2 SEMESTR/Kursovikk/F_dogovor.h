#pragma once

#include "Employer_list.h"
#include "Worker_list.h"
#include "Vacansia_list.h"

#include "common.h"

struct Dogovor
{
    EmployerNode *rabotodatel = nullptr; // работодатель
    WorkerNode *rabotnik = nullptr;      // работник
    // VacansiaNode * next = nullptr;
    VacansiaNode *closed_vacansiaa = nullptr; // закрытая вакансия
   // ResumeNode *closed_resumee = nullptr;

    bool Read(std::ifstream &input, std::ofstream &log);
    bool Write(std::ofstream &output, std::ofstream &log);

    void Print(std::ofstream &log);
    
};

struct DogovorNode
{
    Dogovor value;
    DogovorNode *next = nullptr;
};

struct F_dogovor
{
    DogovorNode *head = nullptr;
    DogovorNode *cur = nullptr;
    int num_dogovori = 0;
};