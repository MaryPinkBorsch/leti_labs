#include "Birga.h"

using namespace std;

WorkerNode* Birga::AddWorker(std::ofstream &log) 
{
    WorkerNode* newWorker = new WorkerNode();

    std::cout << "Введите Имя";
    std::cin >> newWorker->value.F_I_O.imya;
    std::cout << "Введите Отчество";
    std::cin >> newWorker->value.F_I_O.otchestvo;
    std::cout << "Введите фамилиё";
    std::cin >> newWorker->value.F_I_O.familia;

    std::string obrazovanie_string = "";
    std::cout << "Введите уровень образования цифрой";
    std::cin >> obrazovanie_string; // считываем номер профессии
    newWorker->value.education_lvl = (Obrazovanie)std::atoi(obrazovanie_string.c_str());

    int counter = 0;
    std::cout << "Добавить резюме? Y/N";
    char answer = 'f';
    std::cin >> answer;
    newWorker->value.resumes.head = nullptr;
    while (answer == 'Y' || answer == 'y') 
    {
        if (newWorker->value.resumes.head == nullptr)
        {
            newWorker->value.resumes.head = new ResumeNode();
            newWorker->value.resumes.cur = newWorker->value.resumes.head;
        }
        else 
        {
            newWorker->value.resumes.cur->next = new ResumeNode();
            newWorker->value.resumes.cur = newWorker->value.resumes.cur->next;
        }
        std::cout << "Введите профессию цифрой";
        std::cin >> (int&)newWorker->value.resumes.cur->value.wanted_profession;
        std::cout << "Введите желаемую зарплату";
        std::cin >> newWorker->value.resumes.cur->value.wanted_salary;
        ++counter;
        std::cout << "Добавить еще резюме? Y/N";
        std::cin >> answer;
    }
    newWorker->value.resumes.num_resumes = counter;

    return newWorker;
}

void Birga::PrintWorker(WorkerNode *cur, std::ofstream &log)
{
    cur->value.Print(log);
}
void Birga::PrintWorkers(std::ofstream &log)
{
    workers.cur = workers.head;
    for (int i = 0; i < workers.num_workers; i++)
    {
        workers.cur->value.Print(log);
        workers.cur = workers.cur->next;
    }
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

void Birga::BigProcess(std::ofstream &log)
{
    bool exit = false;
    while (!exit) 
    {
        std::cout << "Меню:" << std::endl;
        std::cout << "1) выход" << std::endl;
        std::cout << "2) добавить работника" << std::endl;
        std::cout << "3) распечатать работников" << std::endl;

        char action = 1;
        std::cin >> action;
        switch (action) 
        {
            case 1:
            {
                exit = true;
            }
            break;
            case 2:
            {
                AddWorker(log);
            }
            break;
            case 3:
            {
                PrintWorkers(log);
            }
            break;
            default:
            {
                exit = true;
            }
            break;
        }
    }
}
