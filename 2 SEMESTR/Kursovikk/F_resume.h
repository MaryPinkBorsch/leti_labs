#pragma once
#include "Worker_list.h"

struct Resume
{
    WorkerNode * soiskatel = 0;
    Professia wanted_profession; // желаемая professia
    int wanted_salary;

    
    bool Read(std::ifstream & input, std::ofstream & log);
    bool Write(std::ofstream & output, std::ofstream & log); 
};

struct ResumeNode
{
    Resume value;
    ResumeNode * next=0;
};

struct F_resume
{
    ResumeNode  * head =0;
    ResumeNode * cur =0;
};