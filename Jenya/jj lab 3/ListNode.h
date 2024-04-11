#include "StrM3.h"

struct ListNode
{
    StrM strochka;
    ListNode *next = nullptr; 
    void addMemory(ListNode*&cur);
    void addNext(ListNode*&cur);
};