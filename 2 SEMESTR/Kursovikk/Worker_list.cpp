#include "Worker_list.h"

using namespace std;

bool Worker::Read(std::ifstream &input, std::ofstream &log)
{
    F_I_O.Read(input, log);

    education_lvl.Read(input, log);

    StrL num_resumes_str;
    num_resumes_str.Read(input, log);
    resumes.num_resumes = atoi(num_resumes_str.massiv);

    resumes.head = new ResumeNode();
    resumes.cur = resumes.head;
    for (int i = 0; i < resumes.num_resumes; i++)
    {
        resumes.cur->value.Read(input, log); // add if  ?
        resumes.cur->next = new ResumeNode();
        resumes.cur = resumes.cur->next;
    }
    char toSkip;
    while (!input.eof() && input.peek() == '\n')
        input >> std::noskipws >> toSkip;
    return true;
}
bool Worker::Write(std::ofstream &output, std::ofstream &log)
{
    F_I_O.Write(output, log);

    education_lvl.Write(output, log);

    StrL num_resumes_string;
    snprintf(num_resumes_string.massiv, StrL::N, "%d", resumes.num_resumes); // запись в строку стрL числа салари в форме строки
    num_resumes_string.Write(output, log);

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
    cout << "ФИО: ";
    log << "ФИО: ";
    F_I_O.Print(log);

    cout << "Образование: ";
    log << "Образование: ";
    education_lvl.Print(log);
    resumes.cur = resumes.head;
    for (int i = 0; i < resumes.num_resumes; i++)
    {
        resumes.cur->value.Print(log);
        resumes.cur = resumes.cur->next;
    }
}