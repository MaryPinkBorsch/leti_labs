#include "Vacansia_list.h"
#include "Employer_list.h"
#include "Worker_list.h"

using namespace std;

bool Vacansia::Read(std::ifstream &input, std::ofstream &log)
{

    // std::string work_field_string = "";
    // getline(input, work_field_string, '?');
    // work_field = (WorkField)std::atoi(work_field_string.c_str());

    std::string professia_string = "";
    getline(input, professia_string, '^');
    professia = (Professia)std::atoi(professia_string.c_str());

    // std::string gorod_string = "";
    // getline(input, gorod_string, '&');
    // adress = (Gorod)std::atoi(gorod_string.c_str());

    std::string salary_string = "";
    getline(input, salary_string, '*');
    salary = (Professia)std::atoi(salary_string.c_str());

    std::string obrazovanie_string = "";
    getline(input, obrazovanie_string, '!');
    education_lvl = (Obrazovanie)std::atoi(obrazovanie_string.c_str());

    return true;
}

bool Vacansia::Write(std::ofstream &output, std::ofstream &log)
{
    // std::string work_field_string = "";
    // work_field_string = std::to_string(work_field);
    // output << work_field_string << '?';

    std::string professia_string = "";
    professia_string = std::to_string(professia);
    output << professia_string << '^';

    // std::string gorod_string = "";
    // gorod_string = std::to_string(adress);
    // output << gorod_string << '&';

    std::string salary_string = "";
    salary_string = std::to_string(salary);
    output << salary_string << '*';

    std::string obrazovanie_string = "";
    obrazovanie_string = std::to_string(education_lvl);
    output << obrazovanie_string << '!';

    return true;
}
