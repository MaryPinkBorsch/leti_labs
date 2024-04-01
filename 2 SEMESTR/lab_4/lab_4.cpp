#include "common.h"
#include "FormularV.h"
#include "lab_4.h"

using namespace std;

bool Read_file(std::ifstream &input, std::ofstream &res, FormularV &formularVert)
{
    formularVert.head = new ListNodeV;
    formularVert.cur = formularVert.head;
    while (!input.eof())
    {
        if (formularVert.cur->ReadV(input, res))
        {
            formularVert.cur->next = new ListNodeV;
            formularVert.prev = formularVert.cur;
            formularVert.cur = formularVert.cur->next;
        }
        else
            break;
    }
    if (formularVert.cur->f_H.head == nullptr && formularVert.prev != nullptr) 
    {
        delete formularVert.cur;
        formularVert.prev->next = nullptr;
    }

    return true;
}

void PrimenitIzmenenia(std::ofstream &res, FormularV &formularVert, ListNodeVFunction toApply)
{
    formularVert.cur = formularVert.head;
    while (formularVert.cur != nullptr)
    {
        (*toApply)(*formularVert.cur);
        formularVert.cur = formularVert.cur->next;
    }
}

void print2(std::ofstream &res, FormularV &formularVert)
{
    formularVert.cur = formularVert.head;
    while (formularVert.cur != nullptr)
    {
        formularVert.cur->print1(res);
        cout << endl
             << "-------> " << endl
             << endl;
        res << endl
            << "-------> " << endl
            << endl;
        formularVert.cur = formularVert.cur->next;
    }
    cout << " NULL_V  " << endl
         << endl;
    ;
    res << " NULL_V " << endl
        << endl;
    ;
}

void ToLowerCase(ListNodeV& toProcess) 
{
    toProcess.f_H.cur = toProcess.f_H.head;
    while(toProcess.f_H.cur != nullptr) 
    {
        for (int i = 0; i < toProcess.f_H.cur->podstroka.len; i++) 
            toProcess.f_H.cur->podstroka.massiv[i] = std::tolower(toProcess.f_H.cur->podstroka.massiv[i]);
        toProcess.f_H.cur = toProcess.f_H.cur->next;        
    }
}
void ToUpperCase(ListNodeV& toProcess) 
{
    toProcess.f_H.cur = toProcess.f_H.head;
    while(toProcess.f_H.cur != nullptr) 
    {
        for (int i = 0; i < toProcess.f_H.cur->podstroka.len; i++) 
            toProcess.f_H.cur->podstroka.massiv[i] = std::toupper(toProcess.f_H.cur->podstroka.massiv[i]);
        toProcess.f_H.cur = toProcess.f_H.cur->next;        
    }
}

void BIG_process(std::ofstream &res, std::string in_filename)
{
    ifstream input;
    input.open(in_filename, std::ios_base::in);
    FormularV f1;
    int rezhim;

    if (Read_file(input, res, f1))
    {
        cout << "ИСХОДНИК: " << endl
             << endl;
        res << "ИСХОДНИК: " << endl
            << endl;
        print2(res, f1);

        ListNodeVFunction myFunction;
        cout << "Выберите режим 1 - легкий, 0 - сложный"<<endl;
        cin>>rezhim;

        switch (rezhim)
        {
        case 0:
            myFunction = &ToUpperCase;
            break;
        case 1:
            myFunction = &ToLowerCase;
            break;
        default:
            break;
        }

        PrimenitIzmenenia(res, f1, myFunction);

        print2(res, f1);

        //deleteng


        cout<< " конец обработки "<< in_filename<<endl<<endl;
                res<< " конец обработки "<< in_filename<<endl<<endl;


    }
}
