#include "ListNode.h"

using namespace std;

void ListNode::addMemory(ListNode*&cur)
{
    if(cur==0)
        cur = new ListNode;
    if(cur == nullptr)
    {
        cout << " ОШИБКА, ПАМЯТЬ КОНЧИЛАСЬ!!! " << endl;
        abort();
    }
}

void ListNode::addNext(ListNode*&cur)
{
    cur->next = new ListNode;
    if(cur->next == nullptr)
    {
        cout << " ОШИБКА, ПАМЯТЬ КОНЧИЛАСЬ!!! " << endl;
        abort();
    }
}
