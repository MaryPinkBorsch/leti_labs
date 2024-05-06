#include "F_resume.h"

#include "Worker_list.h"

using namespace std;

bool Resume::Read(std::ifstream &input, std::ofstream &log)
{
    wanted_profession.Read(input, log);

    StrL wanted_salary_string;
    wanted_salary_string.Read(input, log);
    wanted_salary = atoi(wanted_salary_string.massiv);
    return true;
}
bool Resume::Write(std::ofstream &output, std::ofstream &log)
{
    wanted_profession.Write(output, log);

    StrL wanted_salary_string;
    snprintf(wanted_salary_string.massiv, StrL::N, "%d", wanted_salary); // запись в строку стрL числа салари в форме строки
    wanted_salary_string.Write(output, log);

    return true;
}

void Resume::Print(std::ofstream &log)
{
    cout << "Желаемая professia: ";
    log << "Желаемая professia: ";
    wanted_profession.Print(log);

    cout << "Желаемая зарплата: " << wanted_salary << endl;

    log << "Желаемая зарплата: " << wanted_salary << endl;
}