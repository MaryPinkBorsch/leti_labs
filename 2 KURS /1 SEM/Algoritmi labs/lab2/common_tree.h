#pragma once
#include <iostream>
#include <queue>
#include <algorithm>
// проход по дереву В ГЛУБИНУ
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
// тоже проход по дереву в глубину, написаннный на шаблоне
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

// вывод поуровнево для Х
// ОБХОД дерева В ШИРИНУ на шаблонах
template <typename T>
void levelOrder_print(T *x)
{
    if (x == nullptr)
        return;

    // используем очередь
    std::queue<T *> q;
    T *curr;

    q.push(x);

    while (!q.empty())
    {
        curr = q.front();
        q.pop();

        std::cout << curr->val << " ";

        // кладем детей в очередь
        if (curr->L != nullptr)
            q.push(curr->L);
        if (curr->R != nullptr)
            q.push(curr->R);
    }
}

template <typename T>
void find_max_height(T *x, int &max_h, int &num_nodes, int cur_h = 0)
{
    // типо считаем что в КОРНЕ высота = 0 !!!!!!!!!!!!!!!!!!!!!!!!!

    if (x != nullptr)
    {
        num_nodes++;
        if (cur_h > max_h)
            max_h = cur_h;

        find_max_height(x->L, max_h, num_nodes, cur_h + 1);
        find_max_height(x->R, max_h, num_nodes, cur_h + 1);
    }
}