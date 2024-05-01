#include "Employer_list.h"

using namespace std;

bool Employer::Read(std::ifstream &input, std::ofstream &log)
{
    F_I_O.Read(input, log);

    std::string work_field_string = "";
    getline(input, work_field_string, '$');
    work_field = (WorkField)std::atoi(work_field_string.c_str());

    std::string gorod_string = "";
    getline(input, gorod_string, '%');
    adress = (Gorod)std::atoi(gorod_string.c_str());

    std::string num_vacansii_string = "";
    getline(input, num_vacansii_string, '+');
    //input >> num_vacansii_string; //
    offered_vacansii.num_vacansii = (int)std::atoi(num_vacansii_string.c_str());

    offered_vacansii.head = new VacansiaNode();
    offered_vacansii.cur = offered_vacansii.head;
    for (int i = 0; i < offered_vacansii.num_vacansii; i++)
    {
        offered_vacansii.cur->value.Read(input, log);
        offered_vacansii.cur->next = new VacansiaNode();
        offered_vacansii.cur = offered_vacansii.cur->next;
    }
    char toSkip;
    while (!input.eof() && input.peek() == '\n')
        input >> std::noskipws >> toSkip;
    return true;
}

bool Employer::Write(std::ofstream &output, std::ofstream &log)
{
    F_I_O.Write(output, log);

    std::string work_field_string = "";
    work_field_string = std::to_string(work_field);
    output << work_field_string<< '$';

    std::string gorod_string = "";
    gorod_string = std::to_string(adress);
    output << gorod_string<< '%';

    std::string num_vacansii_string = "";
    num_vacansii_string = std::to_string(offered_vacansii.num_vacansii);
    output << num_vacansii_string<< '+';

    offered_vacansii.cur = offered_vacansii.head;
    for (int i = 0; i < offered_vacansii.num_vacansii; i++)
    {
        offered_vacansii.cur->value.Write(output, log);
        offered_vacansii.cur = offered_vacansii.cur->next;
    }
    output << std::endl;
    return true;
}

void Employer::Print(std::ofstream &log)
{
    cout << "ФИО: " << F_I_O.familia << " " << F_I_O.imya << " " << F_I_O.otchestvo << endl;
    log << "ФИО: " << F_I_O.familia << " " << F_I_O.imya << " " << F_I_O.otchestvo << endl;

    cout << "Рабочая область: " << (int)work_field << endl;
    log << "Рабочая область: " << (int)work_field << endl;
    offered_vacansii.cur = offered_vacansii.head;
    for(int i = 0; i < offered_vacansii.num_vacansii; i++)
    {
        offered_vacansii.cur->value.Print(log);
        offered_vacansii.cur=offered_vacansii.cur->next;
    }
}