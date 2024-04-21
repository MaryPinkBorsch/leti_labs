#pragma once
#include <string>
// #include "F_w_field.h"
// #include "F_gorod.h"

#include "F_fio.h"
#include "F_professia.h"
#include "F_education_lvl.h"
#include "F_resume.h"

struct Worker
{
    F_fio F_I_O;
    Obrazovanie education_lvl;   // уровень образования
    // int opit; // лет опыта 
    F_resume resumes; // все резюме работника
};

struct WorkerNode
{
    Worker value;
    WorkerNode *next = 0;
};

struct F_Worker 
{
    WorkerNode * head = nullptr;
    WorkerNode * cur = nullptr;
};