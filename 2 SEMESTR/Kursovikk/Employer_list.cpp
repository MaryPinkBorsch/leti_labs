#include "Employer_list.h"

using namespace std;

bool Employer::Read(std::ifstream &input, std::ofstream &log)
{
    F_I_O.Read(input, log);

    work_field.Read(input, log);

    adress.Read(input, log);

    StrL num_vacansii_string;
    num_vacansii_string.Read(input, log);
    offered_vacansii.num_vacansii = atoi(num_vacansii_string.massiv);

    if (offered_vacansii.num_vacansii > 0) 
    {
        offered_vacansii.head = new VacansiaNode();
        offered_vacansii.cur = offered_vacansii.head;
        for (int i = 0; i < offered_vacansii.num_vacansii; i++)
        {
            offered_vacansii.cur->value = new Vacansia();
            offered_vacansii.cur->value->Read(input, log); // if ?
            if (i != offered_vacansii.num_vacansii - 1) 
            {
                offered_vacansii.cur->next = new VacansiaNode();
                offered_vacansii.cur = offered_vacansii.cur->next;
            }
        }
    }
    char toSkip;
    while (!input.eof() && input.peek() == '\n')
        input >> std::noskipws >> toSkip;
    return true;
}

bool Employer::Write(std::ofstream &output, std::ofstream &log)
{
    F_I_O.Write(output, log);

    work_field.Write(output, log);

    adress.Write(output, log);

    StrL num_vacansii_string;
    snprintf(num_vacansii_string.massiv, StrL::N, "%d", offered_vacansii.num_vacansii); // запись в строку стрL числа салари в форме строки
    num_vacansii_string.len = std::strlen(num_vacansii_string.massiv);
    num_vacansii_string.Write(output, log);

    offered_vacansii.cur = offered_vacansii.head;
    for (int i = 0; i < offered_vacansii.num_vacansii; i++)
    {
        offered_vacansii.cur->value->Write(output, log);
        offered_vacansii.cur = offered_vacansii.cur->next;
    }
    output << std::endl;
    return true;
}

void Employer::Print(std::ofstream &log)
{
    cout << "ФИО: ";
    log << "ФИО: ";
    F_I_O.Print(log);

    cout << "Рабочая область: ";
    log << "Рабочая область: ";
    work_field.Print(log);

    cout << "Город: ";
    log << "Город: ";
    adress.Print(log);

    offered_vacansii.cur = offered_vacansii.head;
    for (int i = 0; i < offered_vacansii.num_vacansii; i++)
    {
        offered_vacansii.cur->value->Print(log);
        offered_vacansii.cur = offered_vacansii.cur->next;
    }
}