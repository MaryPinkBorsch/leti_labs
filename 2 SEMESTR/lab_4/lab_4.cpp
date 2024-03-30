#include "common.h"
#include "FormularV.h"

using namespace std;

bool Read_file(std::ifstream &input, std::ofstream &res, FormularV &formularVert)
{
    formularVert.cur = formularVert.head;
    while (!input.eof())
    {
        formularVert.cur = new ListNodeV; // addMemo
        if(formularVert.cur->ReadV(input, res))
        {
            formularVert.cur = formularVert.cur->next;
        }
        else
            break;
    }
}
