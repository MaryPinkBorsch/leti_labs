#pragma once

#include <iostream>
#include <algorithm>

// в БСТ левый ребенок ВСЕГДА меньше родителя а ПРАВЫЙ всегда БОЛЬШЕ!!!
// все значения уникальны
struct BST_node
{
    BST_node *L = nullptr; // левый ребенок
    BST_node *R = nullptr; // правый ребенок

    int val = 0; // значение
    // BST_node * parent = nullptr; //родитель

    // конструкторы пусть тут бубд
    BST_node() : val(0), L(nullptr), R(nullptr) {}
    BST_node(int x) : val(x), L(nullptr), R(nullptr) {}
    BST_node(int x, BST_node *L, BST_node *R) : val(x), L(L),
                                                R(R) {}
};
// struct BST // структура бинарного дерева поиска
// {
//     BST_node *root = nullptr;
// };

//(структура, балансировка, операции вставки/удаления/поиска).

// это я набросала ручками просто так
BST_node *BST_search2(int to_search, BST_node *root)
{
    if (root == nullptr)
    {
        std::cout << "ПУстое дерево! " << std::endl;
        return nullptr;
    }
    BST_node *cur = root;
    while (1)
    {
        if (cur->val == to_search)
            return cur;
        else if (cur->val > to_search)
        {
            if (cur->L == nullptr)
                return nullptr;
            cur = cur->L;
        }
        else if (cur->val < to_search)
        {
            if (cur->R == nullptr)
                return nullptr;
            cur = cur->R;
        }
        if (cur->R == nullptr && cur->L == nullptr)
        {
            std::cout << "Значение НЕ найдено!!! " << std::endl;
            return nullptr;
        }
    }
}

// это я писала для задачи на литкоде
BST_node *BSTsearch(BST_node *root, int to_search, BST_node *&prev)
{
    if (!root)
    {
        std::cout << "Пyстое дерево! " << std::endl;
        return nullptr;
    }
    BST_node *cur = root;
    prev = nullptr;

    while (1)
    {
        if (cur->val == to_search)
            return cur;
        else if (cur->val > to_search && cur->L)
        {
            prev = cur;
            cur = cur->L;
        }
        else if (cur->val < to_search && cur->R)
        {
            prev = cur;
            cur = cur->R;
        }
        else
        {
            // std::cout << "ЗНАЧЕНИЕ " << to_search << " НЕ НАЙДЕНО" << std::endl;
            return nullptr;
        }
    }
}

// удаление из БСТ
BST_node *BSTdelete(BST_node *root, int to_search)
{
    // to_search = значение ключа который надо будет удалить

    BST_node *res = root;
    BST_node *prev = nullptr;

    if (!root)
        return nullptr;

    BST_node *to_delete = BSTsearch(root, to_search, prev);

    if (to_delete)
    {
        if (!to_delete->L && !to_delete->R) // нету детей
        {
            if (prev == 0) // надо удалить корень
            {
                res = nullptr;
            }
            if (prev && prev->L == to_delete) // просто занулить
                prev->L = 0;
            else if (prev && prev->R == to_delete)
                prev->R = 0;
        }
        else
        {
            if (!to_delete->L && to_delete->R)
            {
                if (root == to_delete)
                    res = to_delete->R;
                if (prev && prev->L == to_delete) // просто заменить единственного детя
                    prev->L = to_delete->R;
                else if (prev && prev->R == to_delete)
                    prev->R = to_delete->R;
            }

            if (to_delete->L && !to_delete->R)
            {
                if (root == to_delete)
                    res = to_delete->L;
                if (prev && prev->L == to_delete) // просто заменить единственного детя
                    prev->L = to_delete->L;
                else if (prev && prev->R == to_delete)
                    prev->R = to_delete->L;
            }

            if (to_delete->L && to_delete->R)
            {
                // всегда заменяем то делит правым ребенком и левого ребенка
                // с его поддеревом отправляем в крайний левый угол(самого
                // левого подребенка) правого ребенка
                BST_node *tmp = to_delete->R->L;
                BST_node *tmp2 = to_delete->L;

                BST_node *tmp3 = tmp2; // временная переменная чтобы воткнуть

                while (tmp3->R)
                    tmp3 = tmp3->R;
                tmp3->R = tmp;

                if (to_delete == root)
                {
                    root = to_delete->R;
                    root->L = tmp2;
                    res = root;
                }
                else
                {
                    if (prev && prev->L == to_delete) // просто заменить
                    {
                        prev->L = to_delete->R;
                        prev->L->L = tmp2;
                    }
                    else if (prev && prev->R == to_delete)
                    {
                        prev->R = to_delete->R;
                        prev->R->L = tmp2;
                    }
                }
            }
        }
    }
    return res;
}

// вставка в дерево БСТ
void BSTinsert(BST_node *root, int vall)
{
    BST_node *tmp = nullptr;
    BST_node *t = BSTsearch(root, vall, tmp);
    if (t != nullptr)
    {
        std::cout << "Такое значение уже есть! Нельзя вставить!!!" << std::endl;
        return;
    }
    BST_node *cur = root;
    BST_node *to_insert = new BST_node();
    to_insert->val = vall;

    while (1)
    {

        if (cur->val > vall)
        {
            if (!cur->L)
            {
                cur->L = to_insert;
                return;
            }
            else
                cur = cur->L;
        }
        else if (cur->val < vall)
        {

            if (!cur->R)
            {
                cur->R = to_insert;
                return;
            }
            else
                cur = cur->R;
        }
    }
    return;
}
