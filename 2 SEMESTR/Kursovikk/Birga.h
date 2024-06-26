#pragma once

#include "Vacansia_list.h"
#include "Worker_list.h"
#include "Employer_list.h"
#include "F_resume.h"
#include "F_dogovor.h"

struct Birga
{
    F_Vacancia vacancii;
    F_resume resumes;
    F_dogovor dogovori;

    F_Worker workers;
    F_Employer employers;

    WorkerNode *AddWorker(std::ofstream &log);
    EmployerNode *AddEmployer(std::ofstream &log);
    void PrintWorkers(std::ofstream &log);
    void PrintEmployers(std::ofstream &log);
    void PrintWorker(WorkerNode *cur, std::ofstream &log);
    void PrintEmployer(EmployerNode *cur, std::ofstream &log);
    void AddWorkerResume();
    void AddEmployerVacancy();
    void FindVacanciesForWorker_name(StrL &name, std::ofstream &log);
    void FindResumesForEmployer();
    void MakeDogovor();
    void PrintDogovors(std::ofstream &log);
    void PrintDogovor(DogovorNode* cur, std::ofstream &log);
    F_Vacancia FindVacanciiForWorker(WorkerNode *worker, std::ofstream &log);

    //void ReadNew_file_base(std::string filename22, std::ofstream &log, std::string filename1);

    void BigProcess(std::ofstream &log, std::ofstream &res);

    bool Read(std::string &filename, std::ofstream &log);
    bool Write(std::string &filename, std::ofstream &log);
};

/// ♥️ ///