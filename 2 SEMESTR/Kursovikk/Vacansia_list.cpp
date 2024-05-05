#include "Vacansia_list.h"
#include "Employer_list.h"
#include "Worker_list.h"
#include "common.h"
using namespace std;

bool Vacansia::Read(std::ifstream &input, std::ofstream &log)
{
    professia.Read(input, log);

    StrL salary_str;
    salary = atoi(salary_str.massiv);

    education_lvl.Read(input, log);

    return true;
}

bool Vacansia::Write(std::ofstream &output, std::ofstream &log)
{

    professia.Write(output, log);

    StrL salary_str;
    snprintf(salary_str.massiv, StrL::N,"%d",salary); // запись в строку стрл числа салари в форме строки

    education_lvl.Write(output, log);

    return true;
}

void Vacansia::Print(std::ofstream &log)
{
    // cout << "Предлагаемая professia: " << Prof2String(professia) << endl
    //      << "Предлагаемая зарплата: " << salary << endl
    //      << "Необходимый уровень образования: " << Obraz2String(education_lvl) << endl;

    // log << "Предлагаемая professia: " << Prof2String(professia) << endl
    //     << "Предлагаемая зарплата: " << salary << endl
    //     << "Необходимый уровень образования: " << Obraz2String(education_lvl) << endl;
}
