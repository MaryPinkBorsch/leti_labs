#include "Birga.h"

using namespace std;

WorkerNode *Birga::AddWorker(std::ofstream &log)
{
    WorkerNode *newWorker = new WorkerNode();

    cout << "Введите Имя" << endl;
    cin >> newWorker->value.F_I_O.imya;
    cout << "Введите Отчество" << endl;
    cin >> newWorker->value.F_I_O.otchestvo;
    cout << "Введите фамилиё" << endl;
    cin >> newWorker->value.F_I_O.familia;

    string obrazovanie_string = "";
    cout << "Введите уровень образования цифрой" << endl;
    cin >> obrazovanie_string; // считываем номер профессии
    newWorker->value.education_lvl = (Obrazovanie)atoi(obrazovanie_string.c_str());

    int counter = 0;
    cout << "Добавить резюме? Y/N" << endl;
    char answer = 'f';
    cin >> answer;
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
        std::cout << "Введите профессию цифрой" << std::endl;
        std::cin >> (int &)newWorker->value.resumes.cur->value.wanted_profession;
        std::cout << "Введите желаемую зарплату" << std::endl;
        std::cin >> newWorker->value.resumes.cur->value.wanted_salary;
        ++counter;
        cout << "Добавить еще резюме? Y/N" << endl;
        cin >> answer;
    }
    newWorker->value.resumes.num_resumes = counter;

    std::cout << "Найти подходящие вакансии? Y/N" << std::endl;
    std::cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        F_Vacancia suitable_vac = FindVacanciiForWorker(newWorker);
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
                log << "Работодатель: " ;
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
    // if (yes)
    // {
    //     F_Vacancia suitable_vac = FindResumesForEmployer(newWorker);
    //     пройти по формуляру и вызвать Print(log)
    // }

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

    std::cout << "Введите Имя" << std::endl;
    std::cin >> newEmployer->value.F_I_O.imya;
    std::cout << "Введите Отчество" << std::endl;
    std::cin >> newEmployer->value.F_I_O.otchestvo;
    std::cout << "Введите фамилиё" << std::endl;
    std::cin >> newEmployer->value.F_I_O.familia;

    std::string W_field_string = "";
    std::cout << "Введите рабочую область цифрой" << std::endl;
    std::cin >> W_field_string; // считываем номер профессии
    newEmployer->value.work_field = (WorkField)std::atoi(W_field_string.c_str());

    std::string adress_string = "";
    std::cout << "Введите город цифрой" << std::endl;
    std::cin >> adress_string; // считываем номер профессии
    newEmployer->value.adress = (Gorod)std::atoi(adress_string.c_str());

    int counter = 0;
    std::cout << "Добавить новую предлагаемую вакансию? Y/N" << std::endl;
    char answer = 'f';
    std::cin >> answer;
    newEmployer->value.offered_vacansii.head = nullptr;
    while (answer == 'Y' || answer == 'y')
    {
        if (newEmployer->value.offered_vacansii.head == nullptr)
        {
            newEmployer->value.offered_vacansii.head = new VacansiaNode();
            newEmployer->value.offered_vacansii.cur = newEmployer->value.offered_vacansii.head;
            //  !!!
        }
        else
        {
            newEmployer->value.offered_vacansii.cur->next = new VacansiaNode();
            newEmployer->value.offered_vacansii.cur = newEmployer->value.offered_vacansii.cur->next;
            // newEmployer->value.offered_vacansii.cur->value.Employerr = newEmployer->value.F_I_O;
            // !!!
        }
        std::cout << "Введите профессию цифрой" << std::endl;
        std::cin >> (int &)newEmployer->value.offered_vacansii.cur->value.professia;

        std::cout << "Введите предлагаемую зарплату" << std::endl;
        std::cin >> newEmployer->value.offered_vacansii.cur->value.salary;

        std::cout << "Введите требуемый уровень образования цифрой" << std::endl;
        std::cin >> (int &)newEmployer->value.offered_vacansii.cur->value.education_lvl;

        ++counter;

        std::cout << "Добавить еще новую предлагаемую вакансию? Y/N" << std::endl;
        std::cin >> answer;
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

    std::string num_workers_string = "";
    //  input >> num_workers_string;
    getline(input, num_workers_string, '?');
    workers.num_workers = (int)std::atoi(num_workers_string.c_str());
    if (workers.num_workers == 0)
        return true;
    workers.head = new WorkerNode();
    workers.cur = workers.head;
    for (int i = 0; i < workers.num_workers; i++)
    {
        workers.cur->value.Read(input, log);
        if (i != workers.num_workers - 1)
        {
            workers.cur->next = new WorkerNode();
            workers.cur = workers.cur->next;
        }
    }

    ///////////////////////////////////////////////////// Employers

    std::string num_employers_string = "";
    //  input >> num_workers_string;
    getline(input, num_employers_string, '?');
    employers.num_employers = (int)std::atoi(num_employers_string.c_str());
    if (employers.num_employers == 0)
        return true;
    employers.head = new EmployerNode();
    employers.cur = employers.head;
    for (int i = 0; i < employers.num_employers; i++)
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

    std::string num_workers_string = "";
    num_workers_string = std::to_string(workers.num_workers);
    output << num_workers_string << "?";

    workers.cur = workers.head;
    for (int i = 0; i < workers.num_workers; i++)
    {
        workers.cur->value.Write(output, log);
        workers.cur = workers.cur->next;
    }
    // return true;

    /////////////////////////////////////////////////// employers

    std::string num_employers_string = "";
    num_employers_string = std::to_string(employers.num_employers);
    output << num_employers_string << "?";

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
            ReadNew_file_base("new_base1.txt", log, "baza_dannih.txt");
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

void Birga::ReadNew_file_base(std::string filename22, std::ofstream &log, std::string filename1)
{
    // // ifstream input2(filename, ios::in);
    // Read(filename22, log);
    // // Write(filename22, log);
    // PrintEmployers(log);

    // ofstream output(filename1, ios::out | ios::app);

    // // std::string num_workers_string = "";
    // // num_workers_string = std::to_string(workers.num_workers);
    // // output << num_workers_string << "?";

    // // // workers.cur = workers.head;
    // for (int i = 0; i < workers.num_workers; i++)
    // {
    //     workers.cur->value.Write(output, log);
    //     workers.cur = workers.cur->next;
    // }
    // // return true;

    // /////////////////////////////////////////////////// employers

    // // std::string num_employers_string = "";
    // // num_employers_string = std::to_string(employers.num_employers);
    // // output << num_employers_string << "?";

    // // employers.cur = employers.head;
    // for (int i = 0; i < employers.num_employers; i++)
    // {
    //     employers.cur->value.Write(output, log);
    //     employers.cur = employers.cur->next;
    // }
    // // return true;
}
