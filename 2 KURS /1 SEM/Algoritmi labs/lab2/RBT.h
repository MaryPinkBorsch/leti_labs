#pragma once
#include <iostream>
#include <algorithm>

// RBT = красно черные деревья
//  1) у КРАСНЫХ узлов могут быть ТОЛЬКО ЧЕРНЫЕ дети
//  2) все листья = черные (пустые ноды или указатели на НУЛЛ)
//  3) корень у рбт ВСЕГДА ЧЕРНЫЙ!!!
//  4) количество черных узлов из ноды до любого листа должно быть одинаковым

// ПЛАН ВСТАВКИ
// 1. обычная бст вставка
// 2. поправить цвета/ ошибки возникшие после вставки (проверить что у красного только черные дети)

// ПРИ ВСТАВКЕ КРАСНОГО УЗЛА!!!!!
// Случай 1: Дядя красный
//          Перекрас родителя и дядю в черный, а деда и бабку в красный
//          Затем идти вверх по дереву, чтобы проверить наличие дальнейших ошибок

// Случай 2: Дядя  черный

//      2.1 Узел является правым ребенком ---> выполнить LEFT ROTATION родительского узла
//      2.2 Узел является левым ребенком ---> выполнить RIGHT ROTTATION на деде и перекрасить соответствующим образом

struct RBT
{

    // RBT_node
    struct RBT_node
    {
        int val;          // значение
        RBT_node *L;      // левый деть
        RBT_node *R;      // правый деть
        char colour;      // цвет (или Р или Б)
        RBT_node *parent; // указатель на родителя
        // конструктор
        RBT_node(int val) : val(val), L(nullptr), R(nullptr), colour('R'), parent(nullptr) {}
    };

    // корень РБТ
    RBT_node *root;

    bool LL_rot; // флаг для обычного левого поворота (ЛЛ ротейшн)
    bool RR_rot; // флаг для обычного правого поворота (ПП ротейшн)
    bool LR_rot; // флаг для поворта сначало ВЛЕВО потом ВПРАВО (ЛП ротейшн) (БОЛЬШОЙ ЛЕВЫЙ ПОВОРОТ)
    bool RL_rot; // флаг для поворта сначало ВПРАВО потом ВЛЕВО (ПЛ ротейшн) (БОЛЬШОЙ ПРАВЫЙ ПОВОРОТ)

    // ЛЕВЫЙ ПОВОРОТ
    RBT_node *L_Rotation(RBT_node *node)
    {
        RBT_node *x = node->R; // праввый ребенок полученой ноды запоминается в Х
        RBT_node *y = x->L;    //  левый ребенок Х
        x->L = node;           // записываем нод в левое поддерево Х
        node->R = y;           // бывший левый ребенок Х становится правым ребенком ноды
        node->parent = x;      // переставляем указатель родителя ноды на Х
        if (y != nullptr)
            y->parent = node; // записываем что у  У родитель это нод (если сам У был)
        return x;             // возвращаем новый корень текущего поддерева
    }

    // ПРАВЫЙ ПОВОРОТ
    RBT_node *R_Rotation(RBT_node *node)
    {
        RBT_node *x = node->L; // берем левого ребенка ноды и запишем его в Х
        RBT_node *y = x->R;    // У это правый ребенок Х
        x->R = node;           // правым ребенком Х становится нод
        node->L = y;           // левым ребенком ноды становится У
        node->parent = x;      // указатель родителя ноды ставим на Х
        if (y != nullptr)
            y->parent = node; // если У не пустой, ставим что его родитель это нода
        return x;
    }

    // вставка РБТ узла
    RBT_node *insert_node(RBT_node *root, int val)
    {
        bool flag = false; // специальный флаг чтоб следить за красными нодами (что у них дети только черные)

        // если корень пуст
        if (root == nullptr)
            return new RBT_node(val);

        // если значение меньше значения корня
        else if (val < root->val)
        {
            root->L = insert_node(root->L, val);
            root->L->parent = root;
            if (root != this->root) // произошла смена корня ?
            {
                // конфликт ---> 2 красных подряд
                if (root->colour == 'R' && root->L->colour == 'R')
                    flag = true;
            }
        }
        else // если значение больше значения корня
        {
            root->R = insert_node(root->R, val);
            root->R->parent = root;
            if (root != this->root) // произошла смена корня ?
            {
                // конфликт ---> 2 красных подряд
                if (root->colour == 'R' && root->R->colour == 'R')
                    flag = true;
            }
        }

        // Левый поворот
        if (LL_rot)
        {
            root = L_Rotation(root);
            root->colour = 'B'; // перекрас корня в черный!
            root->L->colour = 'R';
            LL_rot = false;
        }
        // правый поворот
        else if (RR_rot)
        {
            root = R_Rotation(root);
            root->colour = 'B'; // перекрас корня в черный
            root->R->colour = 'R';
            RR_rot = false;
        }
        // большой правый поворот
        else if (RL_rot)
        {
            root->R = R_Rotation(root->R);
            root->R->parent = root;
            root = L_Rotation(root);
            root->colour = 'B'; // перекрас корня в черный
            root->L->colour = 'R';
            RL_rot = false;
        }
        // большой левый поворот
        else if (LR_rot)
        {
            root->L = L_Rotation(root->L);
            root->L->parent = root;
            root = R_Rotation(root);
            root->colour = 'B'; // перекрас корня в черный
            root->R->colour = 'R';
            LR_rot = false;
        }

        // если 2 КРАСНЫХ нода подряд ---> надо это исправить
        // конфликт корней типо
        if (flag)
        {
            if (root->parent->R == root) // ???
            {
                if (root->parent->L == nullptr || root->parent->L->colour == 'B')
                {
                    if (root->L != nullptr && root->L->colour == 'R')
                        RL_rot = true;
                    else if (root->R != nullptr && root->R->colour == 'R')
                        LL_rot = true;
                }
                else
                {
                    root->parent->L->colour = 'B';
                    root->colour = 'B';
                    if (root->parent != this->root)
                        root->parent->colour = 'R';
                }
            }
            else
            {
                if (root->parent->R == nullptr || root->parent->R->colour == 'B')
                {
                    if (root->L != nullptr && root->L->colour == 'R')
                        RR_rot = true;
                    else if (root->R != nullptr && root->R->colour == 'R')
                        LR_rot = true;
                }
                else
                {
                    root->parent->R->colour = 'B';
                    root->colour = 'B';
                    if (root->parent != this->root)
                        root->parent->colour = 'R';
                }
            }
            flag = false;
        }
        return root;
    }

    // Helper function to perform Inorder Traversal
    void inorderTraversal_node(RBT_node *node)
    {
        if (node != nullptr)
        {
            inorderTraversal_node(node->L);
            std::cout << node->val << " ";
            inorderTraversal_node(node->R);
        }
    }

    // Helper function to print the tree
    void print_node(RBT_node *root, int otstup)
    {
        if (root != nullptr)
        {
            otstup += 10;
            print_node(root->R, otstup);
            std::cout << std::endl;
            for (int i = 10; i < otstup; i++)
                std::cout << " ";
            std::cout << root->val << std::endl;
            print_node(root->L, otstup);
        }
    }

    RBT() : root(nullptr), LL_rot(false), RR_rot(false), LR_rot(false), RL_rot(false) {}

    // вставка в РБТ дерево
    void RBTinsert(int val)
    {
        if (root == nullptr)
        {
            root = new RBT_node(val);
            root->colour = 'B';
        }
        else
            root = insert_node(root, val);
    }

    // инордер прозход по дереву
    void inorderTraversal()
    {
        inorderTraversal_node(root);
    }

    // принт
    void print()
    {
        print_node(root, 0);
    }
};
