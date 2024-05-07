#include "Vacansia_list.h"
#include "Employer_list.h"
#include "Worker_list.h"
#include "common.h"
using namespace std;

bool Vacansia::Read(std::ifstream &input, std::ofstream &log)
{
    professia.Read(input, log);

    StrL salary_str;
    salary_str.Read(input, log);
    salary = atoi(salary_str.massiv);

    education_lvl.Read(input, log);

    StrL closed_str;
    closed_str.Read(input, log);
    closed = atoi(closed_str.massiv);

    return true;
}

bool Vacansia::Write(std::ofstream &output, std::ofstream &log)
{

    professia.Write(output, log);

    StrL salary_str;
    snprintf(salary_str.massiv, StrL::N, "%d", salary); // запись в строку стрL числа салари в форме строки
    salary_str.len = std::strlen(salary_str.massiv);
    salary_str.Write(output, log);

    education_lvl.Write(output, log);

    StrL closed_str;
    snprintf(closed_str.massiv, StrL::N, "%d", closed ? 1 : 0); // запись в строку стрL числа в форме строки
    closed_str.len = std::strlen(closed_str.massiv);
    closed_str.Write(output, log);

    return true;
}

void Vacansia::Print(std::ofstream &log)
{
    cout << "Предлагаемая professia: ";
    log << "Предлагаемая professia: ";
    professia.Print(log);

    cout << "Предлагаемая зарплата: " << salary << endl;
    log << "Предлагаемая зарплата: " << salary << endl;

    cout << "Необходимый уровень образования:";
    log << "Необходимый уровень образования:";
    education_lvl.Print(log);

    if (closed == true)
    {
        cout << "Статус вакансии: закрыта" << endl;
        log << "Статус вакансии: закрыта" << endl;
    }
    else
    {
        cout << "Статус вакансии: открыта" << endl;
        log << "Статус вакансии: открыта" << endl;
    }
}
