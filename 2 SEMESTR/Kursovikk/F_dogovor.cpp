#include "F_dogovor.h"

using namespace std;

bool Dogovor::Read(std::ifstream &input, std::ofstream &log)
{
    return false;
}
bool Dogovor::Write(std::ofstream &output, std::ofstream &log)
{
    return false;
}

void Dogovor::Print(std::ofstream &log)
{
    cout << "ДОГОВОР:" << endl;
    log << "ДОГОВОР:" << endl;

    cout << "Работодатель:" << endl;
    log << "Работодатель:" << endl;
    rabotodatel->value.Print(log);

    cout << "Соискатель:" << endl;
    log << "Соискатель:" << endl;
    rabotnik->value.Print(log);

    cout << "Закрытая вакансия:" << endl;
    log << "Закрытая вакансия:" << endl;
    closed_vacansiaa->value->Print(log);

    cout << "Резюме работника:" << endl;
    log << "Резюме работника:" << endl;
    closed_resumee->value.Print(log);
}