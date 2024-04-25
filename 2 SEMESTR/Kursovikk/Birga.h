#pragma once
#include <string>

#include "Vacansia_list.h"
#include "Worker_list.h"
#include "Employer_list.h"
#include "F_resume.h"
#include "F_dogovor.h"

struct Birga
{
    F_Vacancia  vacancii;
    F_resume    resumes;
    F_dogovor   dogovori;
    F_Worker    workers;
    F_Employer  employers;

    WorkerNode* AddWorker(std::ofstream &log);
    void AddEmployer();
    void PrintWorkers(std::ofstream &log);
    void PrintEmployers();
    void PrintWorker(WorkerNode *cur, std::ofstream &log);
    void PrintEmployer();
    void AddWorkerResume();
    void AddEmployerVacancy();
    void FindVacanciesForWorker();
    void FindResumesForEmployer();
    void MakeDogovor();
    void PrintDogovors();
    void PrintDogovor();

    void BigProcess(std::ofstream &log);
    
    bool Read(std::string &filename, std::ofstream &log);
    bool Write(std::string &filename, std::ofstream &log);
};