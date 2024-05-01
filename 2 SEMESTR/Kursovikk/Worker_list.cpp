#include "Worker_list.h"

using namespace std;

bool Worker::Read(std::ifstream &input, std::ofstream &log)
{
    F_I_O.Read(input, log);

    std::string obrazovanie_string = "";
    getline(input, obrazovanie_string, '$');
    //input >> obrazovanie_string; // считываем номер профессии
    education_lvl = (Obrazovanie)std::atoi(obrazovanie_string.c_str());

    std::string num_resumes_string = "";
    getline(input, num_resumes_string, '%');
    //input >> num_resumes_string; // считываем номер профессии
    resumes.num_resumes = (int)std::atoi(num_resumes_string.c_str());

    resumes.head = new ResumeNode();
    resumes.cur = resumes.head;
    for (int i = 0; i < resumes.num_resumes; i++)
    {
        resumes.cur->value.Read(input, log);
        resumes.cur->next = new ResumeNode();
        resumes.cur = resumes.cur->next;
    }
    char toSkip;
    while (!input.eof() && input.peek() == '\n')
        input >> std::noskipws>> toSkip;
    return true;
}
bool Worker::Write(std::ofstream &output, std::ofstream &log)
{
    F_I_O.Write(output, log);

    std::string obrazovanie_string = "";
    obrazovanie_string = std::to_string(education_lvl);
    output << obrazovanie_string<< '$';

    std::string num_resumes_string = "";
    num_resumes_string = std::to_string(resumes.num_resumes);
    output << num_resumes_string<< '%';

    resumes.cur = resumes.head;
    for (int i = 0; i < resumes.num_resumes; i++)
    {
        resumes.cur->value.Write(output, log);
        resumes.cur = resumes.cur->next;
    }
    output << std::endl;
    return true;
}
void Worker::Print(std::ofstream &log)
{
    cout << "ФИО: " << F_I_O.familia << " " << F_I_O.imya << " " << F_I_O.otchestvo << endl;
    log << "ФИО: " << F_I_O.familia << " " << F_I_O.imya << " " << F_I_O.otchestvo << endl;

    cout << "Образование: " << Obraz2String(education_lvl) << endl;
    log << "Образование: " << Obraz2String(education_lvl) << endl;
    resumes.cur = resumes.head;
    for(int i = 0; i < resumes.num_resumes; i++)
    {
        resumes.cur->value.Print(log);
        resumes.cur=resumes.cur->next;
    }
}