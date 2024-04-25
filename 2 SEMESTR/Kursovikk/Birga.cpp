#include "Birga.h"

using namespace std;

void Birga::AddWorker() {}
void Birga::PrintWorkers() {}
void Birga::PrintWorker(WorkerNode *cur) 
{
    
}

void Birga::AddEmployer() {}
void Birga::PrintEmployers() {}
void Birga::PrintEmployer() {}
void Birga::AddWorkerResume() {}
void Birga::AddEmployerVacancy() {}
void Birga::FindVacanciesForWorker() {}
void Birga::FindResumesForEmployer() {}
void Birga::MakeDogovor() {}
void Birga::PrintDogovors() {}
void Birga::PrintDogovor() {}

bool Birga::Read(std::string &filename, std::ofstream &log)
{
    std::ifstream input(filename, ios::in);

    std::string num_workers_string = "";
    input >> num_workers_string; // 
    workers.num_workers = (int)std::atoi(num_workers_string.c_str());

    workers.head = new WorkerNode();
    workers.cur = workers.head;
    for (int i = 0; i < workers.num_workers; i++)
    {
        workers.cur->value.Read(input, log);
        workers.cur->next = new WorkerNode();
        workers.cur = workers.cur->next;
    }

    return false;
}
bool Birga::Write(std::string &filename, std::ofstream &log)
{
    std::ofstream output(filename, ios::out | ios::trunc);

    std::string num_workers_string = "";
    num_workers_string = std::to_string(workers.num_workers);
    output << num_workers_string;

    workers.cur = workers.head;
    for (int i = 0; i < workers.num_workers; i++)
    {
        workers.cur->value.Write(output, log);
        workers.cur = workers.cur->next;
    }
    return false;
}
