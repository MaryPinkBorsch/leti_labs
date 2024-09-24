#pragma once
#include "common.h"
// #include "F_w_field.h"
// #include "F_gorod.h"

#include "StrL.h"
#include "F_resume.h"

struct Worker
{
    StrL F_I_O;
    StrL education_lvl; // уровень образования

    F_resume resumes; // все резюме работника

    bool Read(std::ifstream &input, std::ofstream &log);
    bool Write(std::ofstream &output, std::ofstream &log);
    void Print(std::ofstream &log);
};

struct WorkerNode
{
    Worker value;
    WorkerNode *next = 0;
};

struct F_Worker
{
    WorkerNode *head = nullptr;
    WorkerNode *cur = nullptr;
    int num_workers = 0;
};