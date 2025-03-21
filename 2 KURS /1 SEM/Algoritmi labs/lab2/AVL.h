#pragma once
#include <iostream>
#include <algorithm>

// AVL tree
// баланс, должен быть +1/-1/0 (иначе дерево разбалансированно)
// высота, для  расчета баланча balance = Hleft - Hright
struct AVL_node
{

    int val;
    AVL_node *L; // левый ребенок
    AVL_node *R; // праввый ребенок
    int height;

    // конструктор
    AVL_node(int k = 0)
    {
        val = k;
        L = nullptr;
        R = nullptr;
        height = 1;
    }
};

// функция для получения высоты
int height(AVL_node *N)
{
    if (N == nullptr)
        return 0;
    return N->height;
}

// поиск (аналог БСТ)
AVL_node *AVLsearch(AVL_node *root, int to_search, AVL_node *&prev)
{
    if (!root)
    {
        std::cout << "Пyстое дерево! " << std::endl;
        return nullptr;
    }
    AVL_node *cur = root;
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
            // std::cout << "ЗНАЧЕНИЕ НЕ НАЙДЕНО" << std::endl;
            return nullptr;
        }
    }
}

// ПРАВЫЙ ПОВОРОТ для поддерева с корнем У
AVL_node *RRotate(AVL_node *y)
{
    AVL_node *x = y->L;  // левый ребенок У
    AVL_node *T2 = x->R; // правый ребенок Х запоминаем

    // поворот (У становится ПравРебенком Х, а Лребенком у  У становится  правое поддерево Х)
    x->R = y;
    y->L = T2;

    // перепись высот (максимальная из 2х возможных +1 )
    y->height = 1 + std::max(height(y->L), height(y->R));
    x->height = 1 + std::max(height(x->L), height(x->R));

    // возвращается новый корень
    return x;
}

// ЛЕВЫЙ ПОВОРОТ для поддерева с корнем Х
AVL_node *LRotate(AVL_node *x)
{
    AVL_node *y = x->R;  // правый ребенок Х
    AVL_node *T2 = y->L; // левый ребенок У, запоминаем

    // сам поворот
    y->L = x;  // Х становится левым ребенком У
    x->R = T2; // правым ребенком Х становится бывший Лребенок У

    // переписываем высоты
    x->height = 1 + std::max(height(x->L), height(x->R));
    y->height = 1 + std::max(height(y->L), height(y->R));

    // возвращаем корень новый
    return y;
}

// функция для получения баланса
int getBalance(AVL_node *N)
{
    if (N == nullptr)
        return 0;
    return height(N->L) - height(N->R);
}

AVL_node *AVLinsert(AVL_node *node, int val)
{
    // 1) обычная вставка в БСТ
    if (node == nullptr)
        return new AVL_node(val);

    if (val < node->val)
        node->L = AVLinsert(node->L, val);
    else if (val > node->val)
        node->R = AVLinsert(node->R, val);
    else // значения должны быть уникальны
    {
        std::cout << "НЕЛЬЗЯ вставить, уже есть такое значение в дереве!" << std::endl;
        return node;
    }

    // 2. переписываем высоту
    node->height = 1 + std::max(height(node->L), height(node->R));

    // 3. Проверяем баланс после вставки на разбалансировку
    int balance = getBalance(node);

    // 4 случая разбалансировки:

    // а) Left Left Case
    if (balance > 1 && val < node->L->val)
        return RRotate(node); // праввый поворот

    // б) Right Right Case
    if (balance < -1 && val > node->R->val)
        return LRotate(node); // делавем левый поворот

    // в) Left Right Case
    if (balance > 1 && val > node->L->val)
    {
        node->L = LRotate(node->L); // большой левый поворот (?)
        return RRotate(node);
    }

    // г) Right Left Case
    if (balance < -1 && val < node->R->val)
    {
        node->R = RRotate(node->R); // большой правый поворот(?)
        return LRotate(node);
    }

    // возвращаем указатель на ноду
    return node;
}

// функция чтоб найти самый левый лист
AVL_node *minValueAVLNode(AVL_node *node)
{
    AVL_node *cur = node;

    // ищем
    while (cur->L != nullptr)
        cur = cur->L;

    return cur;
}

// удаление из АВЛ, удалаяет ноду со значением вал и возвращает корень  измененного поддерева!!!
AVL_node *AVLdelete(AVL_node *root, int val)
{
    // проверка на пустоту
    if (root == nullptr)
        return root;

    // если значение меньше значения корня, тогда удаляем из левого поддерева
    if (val < root->val)
        root->L = AVLdelete(root->L, val);

    // если значение больше значения корня, тогда удаляем из правого поддерева
    else if (val > root->val)
        root->R = AVLdelete(root->R, val);

    // если надо удалить текущий корень
    else
    {
        // ШАГ 1

        // СЛУЧАЙ а) у ноды 1 ребенок ИЛИ нет детей
        if ((root->L == nullptr) || (root->R == nullptr))
        {
            AVL_node *temp = root->L ? root->L : root->R; // запоминаем ребенка который есть

            // а если детей нет:
            if (temp == nullptr)
            {
                temp = root;
                root = nullptr; // затираем текущий корень
            }
            else
                *root = *temp; // копируем непустого ребенка в корень

            free(temp); // затираем тмп
        }
        else
        {
            // СЛУЧАЙ б) если 2 ребенка:

            AVL_node *temp = minValueAVLNode(root->R);
            // нужен самый левый лист из правого поддерева текущего корня

            // этот лист встает на место корня
            root->val = temp->val;

            // удаляем ребенка
            root->R = AVLdelete(root->R, temp->val);
        }
    }

    // если дерево состояло только из корня:
    if (root == nullptr)
        return root;

    // надо переписать высоту текущего нода
    root->height = 1 + std::max(height(root->L), height(root->R));

    // проверяем баланс (вдруг дерево разбалансировало)
    int balance = getBalance(root);

    // 4 СЛУЧАЯ РАЗБАЛАНСИРОВКИ

    // а) Left Left
    if (balance > 1 && getBalance(root->L) >= 0)
        return RRotate(root); // делаем правый поворот

    // б) Left Right
    if (balance > 1 && getBalance(root->L) < 0)
    {
        root->L = LRotate(root->L); // вроде это большой левый
        return RRotate(root);
    }

    // в) Right Right
    if (balance < -1 && getBalance(root->R) <= 0)
        return LRotate(root); // левый поворот

    // г) Right Left Case
    if (balance < -1 && getBalance(root->R) > 0)
    {
        root->R = RRotate(root->R); // большой правый поворот
        return LRotate(root);
    }

    return root;
}

