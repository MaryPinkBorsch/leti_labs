#pragma once

#include "common.h"

struct WorkerNode;

struct Resume
{
    WorkerNode *soiskatel = 0;

    StrL wanted_profession; // желаемая professia
    int wanted_salary;

    bool Read(std::ifstream &input, std::ofstream &log);
    bool Write(std::ofstream &output, std::ofstream &log);

    void Print(std::ofstream &log);
};

struct ResumeNode
{
    Resume value;
    ResumeNode *next = 0;
};

struct F_resume
{
    ResumeNode *head = 0;
    ResumeNode *cur = 0;
    int num_resumes = 0;
};