#pragma once
#include <iostream>
// проход по деверу
template <typename T>
void preOrder_print(T *root)
{
    if (root != nullptr)
    {
        std::cout << root->val << " ";
        preOrder_print(root->L);
        preOrder_print(root->R);
    }
}

template <typename T>
void inOrder_print(T *root)
{
    if (root != nullptr)
    {

        inOrder_print(root->L);
        std::cout << root->val << " ";
        inOrder_print(root->R);
    }
}