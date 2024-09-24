#include "StrM3.h"

struct ListNode
{
    StrM strochka;
    ListNode *next = nullptr; // указатель на следующий элемент списка

    void addMemo(ListNode*&cur);
    void addNext(ListNode*&cur);
    // void infoInput(ListNode*&cur);
};