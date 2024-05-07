#include "Birga.h"

using namespace std;

WorkerNode *Birga::AddWorker(std::ofstream &log)
{
    WorkerNode *newWorker = new WorkerNode();

    cout << "Введите FIO" << endl;
    newWorker->value.F_I_O.Vvod(log);

    cout << "Введите уровень образования (srednee / vishee) " << endl;
    newWorker->value.education_lvl.Vvod(log);

    int counter = 0;
    cout << "Добавить резюме? Y/N" << endl;
    std::string answer;
    std::getline(std::cin, answer);
    newWorker->value.resumes.head = nullptr;
    while (answer[0] == 'Y' || answer[0] == 'y')
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

        std::cout << "Введите профессию (doktor / tester)" << std::endl;
        StrL wanted_profession_s;
        wanted_profession_s.Vvod(log);
        newWorker->value.resumes.cur->value.wanted_profession = wanted_profession_s;

        std::cout << "Введите желаемую зарплату" << std::endl;
        StrL wanted_salary_string;
        wanted_salary_string.Vvod(log);
        newWorker->value.resumes.cur->value.wanted_salary = atoi(wanted_salary_string.massiv);

        ++counter;
        cout << "Добавить еще резюме? Y/N" << endl;
        std::getline(std::cin, answer);
    }
    newWorker->value.resumes.num_resumes = counter;

    std::cout << "Найти подходящие вакансии? Y/N" << std::endl;
    std::getline(std::cin, answer);

    if (answer[0] == 'y' || answer[0] == 'Y')
    {
        F_Vacancia suitable_vac = FindVacanciiForWorker(newWorker, log);
        if (suitable_vac.head == 0)
        {
            cout << " Не найдено подходящих вакансий для данного работника" << endl;

            log << " Не найдено подходящих вакансий для данного работника" << endl;
        }
        else
        {
            cout << "Подходящие предложения по вакансиям: " << endl;
            log << "ПОДХОДЯЩИЕ предложения по вакансиям: " << endl;
            suitable_vac.cur = suitable_vac.head;
            while (suitable_vac.cur != nullptr)
            {
                cout << "Работодатель: ";
                log << "Работодатель: ";
                suitable_vac.cur->value.Rabotodatel->value.F_I_O.Print(log);
                //    log << "Работодатель: " << suitable_vac.cur->value.Rabotodatel->value.F_I_O.familia <<
                //   " " << suitable_vac.cur->value.Rabotodatel->value.F_I_O.imya <<
                //    " " << suitable_vac.cur->value.Rabotodatel->value.F_I_O.otchestvo << endl;

                // !!!

                suitable_vac.cur->value.Print(log);
                suitable_vac.cur = suitable_vac.cur->next;
            }
        }
    }

    return newWorker;
    return 0;
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

EmployerNode *Birga::AddEmployer(std::ofstream &log)
{
    EmployerNode *newEmployer = new EmployerNode();

    cout << "Введите FIO" << endl;
    newEmployer->value.F_I_O.Vvod(log);

    std::cout << "Введите рабочую область (med / it)" << std::endl;
    newEmployer->value.work_field.Vvod(log);

    std::cout << "Введите город (moskva / spb)" << std::endl;
    newEmployer->value.adress.Vvod(log);

    int counter = 0;
    std::cout << "Добавить новую предлагаемую вакансию? Y/N" << std::endl;
    std::string answer;
    std::getline(std::cin, answer);
    newEmployer->value.offered_vacansii.head = nullptr;
    while (answer[0] == 'Y' || answer[0] == 'y')
    {
        if (newEmployer->value.offered_vacansii.head == nullptr)
        {
            newEmployer->value.offered_vacansii.head = new VacansiaNode();
            newEmployer->value.offered_vacansii.cur = newEmployer->value.offered_vacansii.head;
        }
        else
        {
            newEmployer->value.offered_vacansii.cur->next = new VacansiaNode();
            newEmployer->value.offered_vacansii.cur = newEmployer->value.offered_vacansii.cur->next;
        }
        StrL prof;
        std::cout << "Введите профессию (doktor / tester)" << std::endl;
        newEmployer->value.offered_vacansii.cur->value.professia.Vvod(log);

        std::cout << "Введите предлагаемую зарплату" << std::endl;
        //std::cin >> newEmployer->value.offered_vacansii.cur->value.salary;
        StrL wanted_salary_string;
        wanted_salary_string.Vvod(log);
        newEmployer->value.offered_vacansii.cur->value.salary = atoi(wanted_salary_string.massiv);

        std::cout << "Введите требуемый уровень образования (srednee / vishee)" << std::endl;
        newEmployer->value.offered_vacansii.cur->value.education_lvl.Vvod(log);

        ++counter;

        std::cout << "Добавить еще новую предлагаемую вакансию? Y/N" << std::endl;
        std::getline(std::cin, answer);
    }
    newEmployer->value.offered_vacansii.num_vacansii = counter;

    return newEmployer;
    return 0;
}

void Birga::PrintEmployers(std::ofstream &log)
{
    employers.cur = employers.head;
    for (int i = 0; i < employers.num_employers; i++)
    {
        employers.cur->value.Print(log);

        employers.cur = employers.cur->next;
    }
}

void Birga::PrintEmployer(EmployerNode *cur, std::ofstream &log)
{
    cur->value.Print(log);
}

void Birga::AddWorkerResume() {}
void Birga::AddEmployerVacancy() {}
void Birga::FindVacanciesForWorker() {}
void Birga::FindResumesForEmployer() {}
void Birga::MakeDogovor() {}
void Birga::PrintDogovors() {}
void Birga::PrintDogovor() {}

bool Birga::Read(std::string &filename, std::ofstream &log)
{
    ifstream input(filename, ios::in);
    int worker_start = workers.num_workers;

    if (workers.head == nullptr) // если считываем в пустую  базу
    {
        StrL num_workers_string;
        if (num_workers_string.Read(input, log)) 
        {
            workers.head = new WorkerNode();
            workers.cur = workers.head;

            workers.num_workers = atoi(num_workers_string.massiv);

            if (workers.num_workers == 0)
                return true;
        }
    }
    else
    {

        StrL num_workers_string;
        if (num_workers_string.Read(input, log)) 
        {
            workers.cur = workers.head;
            while (workers.cur->next != nullptr)
                workers.cur = workers.cur->next;

            workers.cur->next = new WorkerNode();
            workers.cur = workers.cur->next;

            workers.num_workers += atoi(num_workers_string.massiv);

            if (workers.num_workers == worker_start)
                return true;
        }
    }

    for (int i = worker_start; i < workers.num_workers; i++)
    {
        workers.cur->value.Read(input, log);
        if (i != workers.num_workers - 1)
        {
            workers.cur->next = new WorkerNode();
            workers.cur = workers.cur->next;
        }
    }

    ///////////////////////////////////////////////////// Employers

    int employer_start = employers.num_employers;

    if (employers.head == nullptr)
    {
        StrL num_employers_string;
        if(num_employers_string.Read(input, log)) 
        {
            employers.head = new EmployerNode();
            employers.cur = employers.head;

            employers.num_employers = atoi(num_employers_string.massiv);

            if (employers.num_employers == 0)
                return true;
        }
    }
    else
    {
        StrL num_employers_string;
        if (num_employers_string.Read(input, log)) 
        {
            employers.cur = employers.head;
            while (employers.cur->next != nullptr)
                employers.cur = employers.cur->next;

            employers.cur->next = new EmployerNode();
            employers.cur = employers.cur->next;

            employers.num_employers += atoi(num_employers_string.massiv);
            if (employers.num_employers == employer_start)
                return true;
        }
    }

    for (int i = employer_start; i < employers.num_employers; i++)
    {
        employers.cur->value.Read(input, log);
        if (i != employers.num_employers - 1)
        {
            employers.cur->next = new EmployerNode();
            employers.cur = employers.cur->next;
        }
    }
    // нам нужно починить указатели в вакансиях работодателя чтобы те верно указывали на их работодателя
    employers.cur = employers.head;
    for (int i = 0; i < employers.num_employers; i++)
    {
        employers.cur->value.offered_vacansii.cur = employers.cur->value.offered_vacansii.head;
        for (int j = 0; j < employers.cur->value.offered_vacansii.num_vacansii; j++)
        {
            employers.cur->value.offered_vacansii.cur->value.Rabotodatel = employers.cur; // восстановили указатель
            employers.cur->value.offered_vacansii.cur = employers.cur->value.offered_vacansii.cur->next;
        }
        employers.cur = employers.cur->next;
    }

    return true;
}

bool Birga::Write(std::string &filename, std::ofstream &log)
{
    ofstream output(filename, ios::out | ios::trunc);

    StrL num_workers_string;
    snprintf(num_workers_string.massiv, StrL::N, "%d", workers.num_workers); // запись в строку стрL числа салари в форме строки
    num_workers_string.len = std::strlen(num_workers_string.massiv);
    num_workers_string.Write(output, log);

    workers.cur = workers.head;
    for (int i = 0; i < workers.num_workers; i++)
    {
        workers.cur->value.Write(output, log);
        workers.cur = workers.cur->next;
    }
    // return true;

    /////////////////////////////////////////////////// employers

    StrL num_employers_string;
    snprintf(num_employers_string.massiv, StrL::N, "%d", employers.num_employers); // запись в строку стрL числа салари в форме строки
    num_employers_string.len = std::strlen(num_employers_string.massiv);
    num_employers_string.Write(output, log);

    employers.cur = employers.head;
    for (int i = 0; i < employers.num_employers; i++)
    {
        employers.cur->value.Write(output, log);
        employers.cur = employers.cur->next;
    }
    return true;
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
        std::cout << "4) добавить работодателя" << std::endl;
        std::cout << "5) распечатать работодателей" << std::endl;
        std::cout << "6) Считать с нового файла" << std::endl;

        int action = 1;
        std::string action_str;
        std::getline(std::cin, action_str);
        action = atoi(action_str.c_str());

        switch (action)
        {
        case 1:
        {
            exit = true;
        }
        break;
        case 2:
        {
            WorkerNode *newWorker = AddWorker(log);
            if (workers.head == nullptr)
                workers.head = newWorker;
            else
            {
                workers.cur = workers.head;
                while (workers.cur->next != nullptr)
                    workers.cur = workers.cur->next;

                workers.cur->next = newWorker;
            }
            workers.num_workers++;
        }
        break;
        case 3:
        {
            PrintWorkers(log);
        }
        break;
        case 4:
        {
            EmployerNode *newEmployer = AddEmployer(log);
            if (employers.head == nullptr)
                employers.head = newEmployer;
            else
            {
                employers.cur = employers.head;
                while (employers.cur->next != nullptr)
                    employers.cur = employers.cur->next;

                employers.cur->next = newEmployer;
            }
            employers.num_employers++;
        }
        break;

        case 5:
        {
            PrintEmployers(log);
        }
        break;

        case 6:
        {
            cout << "ВВЕДИТЕ НОМЕР ФАЙЛА" << endl;
            // ReadNew_file_base("new_base1.txt", log, "baza_dannih.txt");
            string f2 = "baza2.txt";
            Read(f2, log);
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

F_Vacancia Birga::FindVacanciiForWorker(WorkerNode *worker, std::ofstream &log)
{
    F_Vacancia res;
    employers.cur = employers.head;
    for (int i = 0; i < employers.num_employers; i++)
    {
        employers.cur->value.offered_vacansii.cur = employers.cur->value.offered_vacansii.head;
        for (int j = 0; j < employers.cur->value.offered_vacansii.num_vacansii; j++)
        {
            employers.cur->value.offered_vacansii.cur->value.professia;

            worker->value.resumes.cur = worker->value.resumes.head;
            for (int k = 0; k < worker->value.resumes.num_resumes; k++)
            {
                if (employers.cur->value.offered_vacansii.cur->value.professia.Equal(worker->value.resumes.cur->value.wanted_profession, log) && employers.cur->value.offered_vacansii.cur->value.salary >= worker->value.resumes.cur->value.wanted_salary && employers.cur->value.offered_vacansii.cur->value.education_lvl.Equal(worker->value.education_lvl, log))
                {
                    if (res.head == nullptr)
                    {
                        res.head = new VacansiaNode();
                        res.cur = res.head;
                    }
                    else
                    {
                        res.cur->next = new VacansiaNode();
                        res.cur = res.cur->next;
                    }
                    res.cur->value = employers.cur->value.offered_vacansii.cur->value;
                }

                worker->value.resumes.cur = worker->value.resumes.cur->next;
            }

            employers.cur->value.offered_vacansii.cur = employers.cur->value.offered_vacansii.cur->next;
        }
        employers.cur = employers.cur->next;
    }
    return res;
}
