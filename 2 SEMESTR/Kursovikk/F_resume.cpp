#include "F_resume.h"

#include "Worker_list.h"

using namespace std;

bool Resume::Read(std::ifstream &input, std::ofstream &log)
{
    // std::string professia_string = "";
    // getline(input, professia_string, '&');
    // // input >> professia_string; // считываем номер профессии
    // wanted_profession = (Professia)std::atoi(professia_string.c_str());

    // std::string wanted_salary_string = "";
    // // input >> wanted_salary_string; // считываем желаемую зарплату
    // getline(input, wanted_salary_string, '*');

    // wanted_salary = std::atoi(wanted_salary_string.c_str()); 

    return true;
}
bool Resume::Write(std::ofstream &output, std::ofstream &log)
{
    // std::string professia_string = "";
    // professia_string = std::to_string(wanted_profession);
    // output << professia_string << '&';
    // std::string wanted_salary_string = "";
    // wanted_salary_string = std::to_string(wanted_salary);
    // output << wanted_salary_string << '*';

    return true;
}

void Resume::Print(std::ofstream &log)
{
    // cout << "Желаемая professia: " << Prof2String(wanted_profession) << endl
    //      << "Желаемая зарплата: " << wanted_salary << endl;

    // log << "Желаемая professia: " << Prof2String(wanted_profession) << endl
    //     << "Желаемая зарплата: " << wanted_salary << endl;
}