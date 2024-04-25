#include "F_resume.h"

#include "Worker_list.h"

using namespace std;

bool Resume::Read(std::ifstream &input, std::ofstream &log)
{
    std::string professia_string = "";
    input >> professia_string; // считываем номер профессии
    wanted_profession = (Professia)std::atoi(professia_string.c_str());
    std::string wanted_salary_string = "";
    input >> wanted_salary_string; // считываем желаемую зарплату
    wanted_salary = (Professia)std::atoi(wanted_salary_string.c_str());

    return true;
}
bool Resume::Write(std::ofstream &output, std::ofstream &log)
{
    std::string professia_string = "";
    professia_string = std::to_string(wanted_profession);
    output << professia_string<< std::endl;
    std::string wanted_salary_string = "";
    wanted_salary_string = std::to_string(wanted_salary);
    output << wanted_salary_string<< std::endl;

    return true;
}

void Resume::Print(std::ofstream &log)
{
    cout << "Желаемая professia: " << (int)wanted_profession << endl
         << "Желаемая зарплата: " << wanted_salary << endl;

    log << "Желаемая professia: " << (int)wanted_profession << endl
        << "Желаемая зарплата: " << wanted_salary << endl;
}