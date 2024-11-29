#pragma once

#include <iostream>
#include <queue>
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

enum COLOR
{
    RED,
    BLACK
};

struct RBTnode
{

    int val;
    COLOR color;

    RBTnode *L;      // левый ребнок
    RBTnode *R;      // правый ребнок
    RBTnode *parent; // родитель

    // конструктор
    RBTnode(int val) : val(val)
    {
        parent = L = R = nullptr;

        // пусть по умолчанию будет красный
        color = RED;
    }

    // функция возвращает указатель на дядю
    RBTnode *uncle()
    {
        // если нет родителя то нет и дяди
        if (parent == nullptr or parent->parent == nullptr)
            return nullptr;

        if (parent->isOnLeft())
            // дядя справа
            return parent->parent->R;
        else
            // дядя слева
            return parent->parent->L;
    }

    // проверка является ли нода левым ребенком
    bool isOnLeft() { return this == parent->L; }

    // возвращается указатель на брата
    RBTnode *sibling()
    {
        // если нет родителя
        if (parent == nullptr)
            return nullptr;

        if (isOnLeft())
            return parent->R;

        return parent->L;
    }

    // сдвигает заданную ноду на нужное место и спускает текущую вниз (текущая становиться ребенком тмп)
    void moveDown(RBTnode *tmp)
    {
        if (parent != nullptr)
        {
            if (isOnLeft())
            {
                parent->L = tmp;
            }
            else
            {
                parent->R = tmp;
            }
        }
        tmp->parent = parent;
        parent = tmp;
    }

    bool hasRedChild()
    {
        return (L != nullptr and L->color == RED) or (R != nullptr and R->color == RED);
    }
};

struct RBTree
{
    RBTnode *root;

    // ЛЕВЫЙ ПОВОРОТ
    void leftRotate(RBTnode *x)
    {
        // тмп запоминаем првого ребенка Х
        RBTnode *tmp = x->R;

        // обновляем корень, если Х был корнем
        if (x == root)
            root = tmp;
        x->moveDown(tmp); // Х спускается вниз

        // правым ребенком Х становится левый ребенок тмп
        x->R = tmp->L;

        // левым ребенком тмп становится  Х (ставим указатель родительский указатель на Х)
        if (tmp->L != nullptr)
            tmp->L->parent = x;

        // левым ребенком тмп становится  Х  ?????
        tmp->L = x;
    }

    // ПРАВЫЙ ПОВОРОТ
    void rightRotate(RBTnode *x)
    {
        // запоминаем левого ребенка Х
        RBTnode *tmp = x->L;

        // обновляем корень если Х это корень
        if (x == root)
            root = tmp;

        x->moveDown(tmp); // Х становаится ребенком тмп (тмп выше чем Х)

        // правывй ребенок тмп становится левым ребенком Х
        x->L = tmp->R;
        // если он не пуст, выставляем указатель на родителя
        if (tmp->R != nullptr)
            tmp->R->parent = x;

        // Х становаится праввым ребенком тмп
        tmp->R = x;
    }

    // функция для обмена цветами
    void swapColors(RBTnode *x1, RBTnode *x2)
    {
        COLOR tmp;
        tmp = x1->color;
        x1->color = x2->color;
        x2->color = tmp;
    }

    // функция для обмена значениями
    void swapValues(RBTnode *u, RBTnode *v)
    {
        int tmp;
        tmp = u->val;
        u->val = v->val;
        v->val = tmp;
    }

    // если возникла ошибка 2х идущих подряд КРАСНЫХ узлов
    void fixRedRed(RBTnode *x)
    {
        // если Х это корень, прочсто перекрасим его и выходим
        if (x == root)
        {
            x->color = BLACK;
            return;
        }

        // инициализируем указатели на родителя деда и дядю
        RBTnode *parent = x->parent, *grandparent = parent->parent, *uncle = x->uncle();

        // если родитель КРАСНЫЙ
        if (parent->color != BLACK)
        {
            // есть КРАСНЫЙ ДЯДЯ
            if (uncle != nullptr && uncle->color == RED)
            {
                // перекрашиваем и рекурсивно идем на верх проверять дальше
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                fixRedRed(grandparent);
            }
            else
            {
                // используем повороты
                if (parent->isOnLeft())
                {
                    // Х слева как и родитель
                    if (x->isOnLeft())
                    {
                        // меняем цвета родителя и деда
                        swapColors(parent, grandparent);
                    }
                    else
                    {
                        leftRotate(parent); // ЛЕВЫЙ поворот
                        swapColors(x, grandparent);
                    }
                    // делаем ПРАВЫЙ поворот
                    rightRotate(grandparent);
                }
                else
                {
                    if (x->isOnLeft())
                    {
                        // делаем ПРАВЫЙ поворот
                        rightRotate(parent);
                        swapColors(x, grandparent);
                    }
                    else
                    {
                        swapColors(parent, grandparent);
                    }

                    // ЛЕВЫЙ поворот
                    leftRotate(grandparent);
                }
            }
        }
    }

    // найти узел без левого ребенка в поддереве Х
    RBTnode *successor(RBTnode *x)
    {
        RBTnode *tmp = x;

        while (tmp->L != nullptr)
            tmp = tmp->L;

        return tmp;
    }

    // функция поиска узла, который сменит удаляемый узел Х
    RBTnode *BSTreplace(RBTnode *x)
    {
        // если есть 2 ребенка, ищем самого левого в правом поддереве Х
        if (x->L != nullptr and x->R != nullptr)
            return successor(x->R);

        // детей нет
        if (x->L == nullptr and x->R == nullptr)
            return nullptr;

        // есть 1 деть
        if (x->L != nullptr)
            return x->L;
        else
            return x->R;
    }

    // УДАЛЕНИЕ ИЗ РБТ
    void deleteNode(RBTnode *to_delete)
    {
        RBTnode *u = BSTreplace(to_delete); // нода ,которая должна сменить удаляемый узел

        // если оба узла черные, выставляем флаг
        bool both_black = ((u == nullptr or u->color == BLACK) and (to_delete->color == BLACK));
        RBTnode *parent = to_delete->parent;
        // удаление ЛИСТА
        if (u == nullptr)
        {
            // to_delete это лист
            if (to_delete == root)
            {
                // to_delete это корень, зануляем корень
                root = nullptr;
            }
            else
            {
                if (both_black)
                {
                    // u and to_delete both black
                    // to_delete is leaf, fix double black at to_delete

                    // ????? вызываем для удаляемой ноды
                    fixDoubleBlack(to_delete);
                }
                else
                {
                    // один из нод красный
                    if (to_delete->sibling() != nullptr)
                        // если брат не пустой, делаем его красным
                        to_delete->sibling()->color = RED;
                }

                // удаляем  ноду из дерева
                if (to_delete->isOnLeft())
                {
                    parent->L = nullptr;
                }
                else
                {
                    parent->R = nullptr;
                }
            }
            delete to_delete;
            return;
        }

        if (to_delete->L == nullptr or to_delete->R == nullptr)
        {
            // у to_delete есть ОДИН РЕБЕНОК!
            if (to_delete == root)
            {
                // u становится новым корнем
                to_delete->val = u->val;
                to_delete->L = to_delete->R = nullptr;
                delete u;
            }
            else
            {
                // u встает на место удаляемой ноды
                if (to_delete->isOnLeft())
                {
                    parent->L = u;
                }
                else
                {
                    parent->R = u;
                }
                delete to_delete;
                u->parent = parent;
                if (both_black)
                {
                    // вызываем функцию для поправки свойств РБТ
                    fixDoubleBlack(u);
                }
                else
                {
                    // перекрашиваем u в черный, если или он или удаляемый нод красный
                    u->color = BLACK;
                }
            }
            return;
        }

        // ???????????? swap values with successor and recurse
        // если 2 РЕБЕНКА, то меняем их значения и рекурсивно удаляем u
        swapValues(u, to_delete);
        deleteNode(u);
    }

    // поправка если не выполняется свойство РБТ про черную высоту
    void fixDoubleBlack(RBTnode *x)
    {
        if (x == root)
            // если дошли до корня, выходим
            return;

        RBTnode *sibling = x->sibling(), *parent = x->parent;
        if (sibling == nullptr)
        {
            // если нет брата, идем выше и рекурсивно вызываем опять функцию
            fixDoubleBlack(parent);
        }
        else
        {
            if (sibling->color == RED)
            {
                // есть красный брат
                parent->color = RED;
                sibling->color = BLACK;
                if (sibling->isOnLeft())
                {
                    // делаем правый поворот
                    rightRotate(parent);
                }
                else
                {
                    // делаем левый поворот
                    leftRotate(parent);
                }
                fixDoubleBlack(x);
            }
            else
            {
                // есть черный брат
                if (sibling->hasRedChild())
                {
                    // как минимум 1 красный ребенок есть
                    if (sibling->L != nullptr and sibling->L->color == RED)
                    {
                        if (sibling->isOnLeft())
                        {
                            // перекрас + правый поворот
                            sibling->L->color = sibling->color;
                            sibling->color = parent->color;
                            rightRotate(parent);
                        }
                        else
                        {
                            // перекрас + большой правый поворот
                            sibling->L->color = parent->color;
                            rightRotate(sibling);
                            leftRotate(parent);
                        }
                    }
                    else
                    {
                        if (sibling->isOnLeft())
                        {
                            // большой левый поворот
                            sibling->R->color = parent->color;
                            leftRotate(sibling);
                            rightRotate(parent);
                        }
                        else
                        {
                            // левый поворот
                            sibling->R->color = sibling->color;
                            sibling->color = parent->color;
                            leftRotate(parent);
                        }
                    }
                    parent->color = BLACK;
                }
                else
                {
                    // 2 черных ребенка
                    sibling->color = RED;
                    if (parent->color == BLACK)
                        fixDoubleBlack(parent);
                    else
                        parent->color = BLACK;
                }
            }
        }
    }

    // вывод поуровнево для Х
    void levelOrder(RBTnode *x)
    {
        if (x == nullptr)
            return;

        // используем очередь
        std::queue<RBTnode *> q;
        RBTnode *curr;

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

    // рекурсивный inorder вывод
    void inorder(RBTnode *x)
    {
        if (x == nullptr)
            return;
        inorder(x->L);
        std::cout << x->val << " ";
        inorder(x->R);
    }

    // конструктор
    RBTree() { root = nullptr; }

    RBTnode *getRoot() { return root; }

    // ПОИСК
    // возвращается найденный элемент
    RBTnode *search(int n)
    {
        RBTnode *tmp = root;
        while (tmp != nullptr)
        {
            if (n < tmp->val)
            {
                if (tmp->L == nullptr)
                    break;
                else
                    tmp = tmp->L;
            }
            else if (n == tmp->val)
            {
                break;
            }
            else
            {
                if (tmp->R == nullptr)
                    break;
                else
                    tmp = tmp->R;
            }
        }
        if (tmp->val != n) // будет возвращен последний элемент (задействовано в вставке)
        {
            std::cout << "ЗНАЧЕНИЕ НЕ НАЙДЕНО!!!" << std::endl;
        }
        return tmp;
    }

    // ВСТАВКА
    void insert(int n)
    {
        RBTnode *newNode = new RBTnode(n);
        if (root == nullptr)
        {
            // корень пуст
            newNode->color = BLACK;
            root = newNode;
        }
        else
        {
            RBTnode *tmp = search(n);
            // поиск находит место для вставки + проверяет, есть ли такое значение в дереве

            if (tmp->val == n)
            {
                // если это значение уже есть, выходим
                return;
            }

            // ставим родительский указатель
            newNode->parent = tmp;

            if (n < tmp->val)
                tmp->L = newNode;
            else
                tmp->R = newNode;

            // проверяем на наличие 2х КРАСНЫХузлов подряд и исправляем, если надо
            fixRedRed(newNode);
        }
    }

    // удаление по заданному значению
    void delete_this_Value(int n)
    {
        if (root == nullptr)
            return;

        RBTnode *tmp = search(n), *u;

        if (tmp->val != n)
        {
            std::cout << "НЕТУ ДАННОГО ЗНАЧЕНИЯ В ДЕРЕВЕ, само значение = " << n << std::endl;
            return;
        }

        deleteNode(tmp);
    }

    // вывод inorder
    void printInOrder()
    {
        std::cout << "Inorder вывод: " << std::endl;
        if (root == nullptr)
            std::cout << "ДЕРЕВО ПУСТОЕ" << std::endl;
        else
            inorder(root);
        std::cout << std::endl;
    }

    // вывод дерева по уровням
    void printLevelOrder()
    {
        std::cout << "Level order вывод: " << std::endl;
        if (root == nullptr)
            std::cout << "ДЕРЕВО ПУСТОЕ" << std::endl;
        else
            levelOrder(root);
        std::cout << std::endl;
    }
};