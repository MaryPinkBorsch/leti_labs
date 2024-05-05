#include "Worker_list.h"

using namespace std;

bool Worker::Read(std::ifstream &input, std::ofstream &log)
{
    F_I_O.Read(input, log);
        
    education_lvl.Read(input, log);

    // std::string num_resumes_string = "";
    // getline(input, num_resumes_string, '%');
    // // input >> num_resumes_string; // считываем номер профессии
    // resumes.num_resumes = (int)std::atoi(num_resumes_string.c_str());

    // resumes.head = new ResumeNode();
    // resumes.cur = resumes.head;
    // for (int i = 0; i < resumes.num_resumes; i++)
    // {
    //     resumes.cur->value.Read(input, log);
    //     resumes.cur->next = new ResumeNode();
    //     resumes.cur = resumes.cur->next;
    // }
    // char toSkip;
    // while (!input.eof() && input.peek() == '\n')
    //     input >> std::noskipws >> toSkip;
    return true;
}
bool Worker::Write(std::ofstream &output, std::ofstream &log)
{
    F_I_O.Write(output, log);

    // std::string obrazovanie_string = "";
    // obrazovanie_string = std::to_string(education_lvl);
    // output << obrazovanie_string << '$';

    // std::string num_resumes_string = "";
    // num_resumes_string = std::to_string(resumes.num_resumes);
    // output << num_resumes_string << '%';

    // resumes.cur = resumes.head;
    // for (int i = 0; i < resumes.num_resumes; i++)
    // {
    //     resumes.cur->value.Write(output, log);
    //     resumes.cur = resumes.cur->next;
    // }
    // output << std::endl;
    return true;
}
void Worker::Print(std::ofstream &log)
{
    cout << "ФИО: ";
    log << "ФИО: ";
    F_I_O.Print(log);

    cout << "Образование: " ;
    log << "Образование: ";
    education_lvl.Print(log);
    resumes.cur = resumes.head;
    for (int i = 0; i < resumes.num_resumes; i++)
    {
        resumes.cur->value.Print(log);
        resumes.cur = resumes.cur->next;
    }
}