#include "common.h"
#include "FormularV.h"
#include "lab_4.h"

using namespace std;

bool Read_file(std::ifstream &input, std::ofstream &res, FormularV &formularVert)
{
    if(input.eof())
    {
        cout<< "SPISOK PUST" <<endl;
        return false;
    }

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
        (*toApply)(*formularVert.cur, res);
        formularVert.cur = formularVert.cur->next;
    }
}

void PrimenitIzmenenia2(std::ofstream &res, FormularV &formularVert, ListNodeVFunction2 toApply)
{
    formularVert.cur = formularVert.head;

    (*toApply)(formularVert, res);
}

void print2(std::ofstream &res, FormularV &formularVert)
{
    formularVert.cur = formularVert.head;
    while (formularVert.cur != nullptr)
    {
        formularVert.cur->print1(res);
        cout << endl
             << "---> " << endl
             << endl;
        res << endl
            << "---> " << endl
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

void ToLowerCase(ListNodeV &toProcess, std::ofstream &res)
{
    toProcess.f_H.cur = toProcess.f_H.head;
    while (toProcess.f_H.cur != nullptr)
    {
        for (int i = 0; i < toProcess.f_H.cur->podstroka.len; i++)
            toProcess.f_H.cur->podstroka.massiv[i] = std::tolower(toProcess.f_H.cur->podstroka.massiv[i]);
        toProcess.f_H.cur = toProcess.f_H.cur->next;
    }
}
void ToUpperCase(ListNodeV &toProcess, std::ofstream &res)
{
    toProcess.f_H.cur = toProcess.f_H.head;
    while (toProcess.f_H.cur != nullptr)
    {
        for (int i = 0; i < toProcess.f_H.cur->podstroka.len; i++)
            toProcess.f_H.cur->podstroka.massiv[i] = std::toupper(toProcess.f_H.cur->podstroka.massiv[i]);
        toProcess.f_H.cur = toProcess.f_H.cur->next;
    }
}

void Delete_Znaki(ListNodeV &toProcess, std::ofstream &res)
{
    // ListNode *cur = head;
    toProcess.f_H.cur = toProcess.f_H.head;
    if (toProcess.f_H.head == nullptr)
    {
        res << "Список пуст!! " << endl;
        // если пустой список сразу возвращаемся
        return;
    }
    toProcess.f_H.prev = nullptr;
    while (toProcess.f_H.cur != nullptr)
    {
        if (toProcess.f_H.cur->podstroka.search1(res) == true)
        {
            FormularH f_tmp;
            f_tmp.cur = toProcess.f_H.cur->next;
            if (toProcess.f_H.prev)
                toProcess.f_H.prev->next = f_tmp.cur;
            delete toProcess.f_H.cur;
            if (toProcess.f_H.cur == toProcess.f_H.head) // если только что удалили голову - переставим ее вперед
            {
                toProcess.f_H.head = f_tmp.cur;
                res << "голова удалена" << endl;
            }
            toProcess.f_H.cur = f_tmp.cur;
        }
        else
        {
            toProcess.f_H.prev = toProcess.f_H.cur;
            toProcess.f_H.cur = toProcess.f_H.cur->next;
        }
    }
}

void Delete_Znaki2(FormularV &toProcess_F, std::ofstream &res)
{
    // ListNode *cur = head;
    toProcess_F.cur = toProcess_F.head;
    bool del = false;

    if (toProcess_F.cur == nullptr)
    {
        cout << " пустой верт. список! " << endl;
        return;
    }
    toProcess_F.prev = nullptr;

    while (toProcess_F.cur != nullptr)
    {
        bool del = false;
        toProcess_F.cur->f_H.cur = toProcess_F.cur->f_H.head;

        while (toProcess_F.cur->f_H.cur != nullptr)
        {
            if (toProcess_F.cur->f_H.cur->podstroka.search1(res) == true)
            {
                del = true;
                break;
            }

            toProcess_F.cur->f_H.cur = toProcess_F.cur->f_H.cur->next;
        }
        if (del == true)
        {

            FormularV f_tmp;
            f_tmp.cur = toProcess_F.cur->next;
            if (toProcess_F.prev)
                toProcess_F.prev->next = f_tmp.cur;
            delete toProcess_F.cur;
            if (toProcess_F.cur == toProcess_F.head) // если только что удалили голову - переставим ее вперед
            {
                toProcess_F.head = f_tmp.cur;
                res << "голова удалена" << endl;
            }
            toProcess_F.cur = f_tmp.cur;
        }
        else
        {
            toProcess_F.prev = toProcess_F.cur;
            toProcess_F.cur = toProcess_F.cur->next;
        }

        // toProcess_F.cur = toProcess_F.cur->next;
    }
}

void BIG_process(std::ofstream &res, std::string in_filename)
{
    ifstream input;
    input.open(in_filename, std::ios_base::in);
    FormularV f1;
    int rezhim;
    cout << "Началась обработка " << in_filename << endl
         << endl;
    res << "Началась обработка " << in_filename << endl
        << endl;
    if (Read_file(input, res, f1))
    {
        cout << "ИСХОДНИК: " << endl
             << endl;
        res << "ИСХОДНИК: " << endl
            << endl;
        print2(res, f1);

        ListNodeVFunction myFunction; // для обработки горизонтальных элементов

        ListNodeVFunction2 myFunction2; // для обработки вертикальных элементов
        cout << "Выберите режим обработки: " << endl
             << " 1 - смена всего шрифта на прописной," << endl
             << " 0 - смена всего шрифта на заглавный, "
             << endl
             << " 2 - удаление горизонтального элемента, содержащего знаки препинания," << endl
             << " 3 - удаление вертик. элемента, содержащего знаки препинания" << endl;
       res << "Выберите режим обработки: " << endl
             << " 1 - смена всего шрифта на прописной," << endl
             << " 0 - смена всего шрифта на заглавный, "
             << endl
             << " 2 - удаление горизонтального элемента, содержащего знаки препинания," << endl
             << " 3 - удаление вертик. элемента, содержащего знаки препинания" << endl;
        cin >> rezhim;

        cout << " Выбран режим: " << rezhim << endl;
        res << " Выбран режим: " << rezhim << endl;

        switch (rezhim)
        {
        case 0:
            myFunction = &ToUpperCase;
            break;
        case 1:
            myFunction = &ToLowerCase;
            break;
        case 2:
            myFunction = &Delete_Znaki;
            break;
        case 3:
            myFunction2 = &Delete_Znaki2;
            break;
        default:
            break;
        }

        if (rezhim == 0 || rezhim == 1 || rezhim == 2)
            PrimenitIzmenenia(res, f1, myFunction);
        else if (rezhim == 3)
            PrimenitIzmenenia2(res, f1, myFunction2);
        else
        {
            cout << "ВЫБРАННЫЙ РЕЖИМ НЕ СУЩЕСТВУЕТ (купите очки)" << endl;
            res << "ВЫБРАННЫЙ РЕЖИМ НЕ СУЩЕСТВУЕТ (купите очки)" << endl;
            return;
        }

        cout<< endl << "RESULT: " << endl
             << endl;
        res<< endl << "RESULT: " << endl
            << endl;

        print2(res, f1);

        // deleteng

        cout << " конец обработки " << in_filename << endl
             << endl;
        res << " конец обработки " << in_filename << endl
            << endl<< endl<< endl;
    }
}
