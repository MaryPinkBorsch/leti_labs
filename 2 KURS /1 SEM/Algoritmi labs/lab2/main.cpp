#include "BST.h"
#include "RBT.h"
#include "AVL.h"
#include "common_tree.h"
#include <vector>
#include <cstdlib>

using namespace std;

// ПЛАН!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// 1. Реализовать бинарное дерево поиска, красно-черное дерево и АВЛ дерево
//(структура, балансировка, операции вставки/удаления/поиска).

// типо считаем что в КОРНЕ дерева высота = 0 !!!!!!!!!!!!!!!!!!!!!!!!!

// 2. Получить зависимость высоты дерева поиска от количества ключей, при условии,
// что значение ключа - случайная величина, распределенная равномерно.
// Какая асимпотика функции h(n) наблюдается у двоичного дерева поиска?

// 3. Получить зависимость АВЛ и красно-черного дерева поиска от количества ключей, при условии,
// что значения ключей монотонно возрастают.

// 3. Вывести полученные результаты на графики
// 4. Сравнить с теоретической оценкой высоты.

// 5. Реализовать обходы в глубину и обход в ширину двоичного дерева с выводом результата.

int main(int argc, char *argv[])
{
    // AVL BUG
    {
        cout << "Обработка AVL" << endl;
        AVL_node *root = new AVL_node(1);
        root = AVLinsert(root, 2);
        root = AVLinsert(root, 3);
        root = AVLinsert(root, 4);
        root = AVLinsert(root, 5);
        cout << endl;
        inOrder_print(root);
        cout << endl;
    }

    // AVL
    {
        cout << "Обработка AVL" << endl;
        AVL_node *root = new AVL_node(50);
        root = AVLinsert(root, 25);
        root = AVLinsert(root, 75);
        root = AVLinsert(root, 100);
        root = AVLinsert(root, 30);
        root = AVLinsert(root, 1);
        inOrder_print(root);
        root = AVLdelete(root, 50);
        cout << endl;
        inOrder_print(root);
        AVL_node *prev = 0;
        AVL_node *t = AVLsearch(root, 25, prev);
        AVL_node *tmp = AVLsearch(root, 666, prev);

        int max_H = 0;
        int n = 0; //  колисчество узлов в дереве
        find_max_height(root, max_H, n);
        cout << "Максимальная высота " << max_H << " при " << n << " узлов" << endl;
    }
    // BST
    {
        cout << endl
             << "Обработка BST" << endl;

        BST_node *root = new BST_node(50);
        BSTinsert(root, 25);
        BSTinsert(root, 75);
        BSTinsert(root, 100);
        BSTinsert(root, 30);
        BSTinsert(root, 1);
        inOrder_print(root);
        root = BSTdelete(root, 50);
        cout << endl;
        inOrder_print(root);
        cout << endl;
        BST_node *prev = 0;
        BST_node *t = BSTsearch(root, 25, prev);
        BST_node *tmp = BSTsearch(root, 666, prev);

        int max_H = 0;
        int n = 0; //  колисчество узлов в дереве
        find_max_height(root, max_H, n);
        cout << "Максимальная высота " << max_H << " при " << n << " узлов" << endl;
    }

    // RBT
    {
        cout << endl
             << "Обработка RBT" << endl;

        RBTree tree;
        tree.RBTinsert(50);
        tree.RBTinsert(25);
        tree.RBTinsert(75);
        tree.RBTinsert(100);
        tree.RBTinsert(30);
        tree.RBTinsert(1);

        inOrder_print(tree.root);
        cout << endl;

        levelOrder_print(tree.root);
        cout << endl;

        tree.delete_this_Value(50);
        cout << endl;
        inOrder_print(tree.root);
        cout << endl;
        levelOrder_print(tree.root);
        cout << endl;

        RBT_node *prev = 0;
        tree.RBTsearch(25);
        tree.RBTsearch(666);

        int max_H = 0;
        int n = 0; //  колисчество узлов в дереве
        find_max_height(tree.root, max_H, n);
        cout << "Максимальная высота " << max_H << " при " << n << " узлов" << endl;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // задание 2
    const int MAX_ELEM = 10000;
    {
        cout << endl
             << " Задание 2" << endl;
        BST_node *root = new BST_node;

        vector<int> keys;
        
        keys.resize(MAX_ELEM);
        for (int i = 0; i < MAX_ELEM; i++)
        {
            keys[i] = rand();
            BSTinsert(root, keys[i]);

            if (i % 10 == 0)
            {
                int max_H = 0;
                int n = 0; //  колисчество узлов в дереве
                find_max_height(root, max_H, n);
                cout << "Максимальная высота " << max_H << " при " << n << " узлов" << endl;
            }
        }
    }

    // задание 3 AVL
    {
        cout << endl
             << " Задание 3 - AVL" << endl;
        AVL_node *root = new AVL_node();

        vector<int> keys;
       
        keys.resize(MAX_ELEM);
        for (int i = 1; i < MAX_ELEM; i++)
        {
            keys[i] = i;
            root = AVLinsert(root, keys[i]);

            if (i % 10 == 0)
            {
                int max_H = 0;
                int n = 0; //  колисчество узлов в дереве
                find_max_height(root, max_H, n);
                cout << "Максимальная высота " << max_H << " при " << n << " узлов" << endl;
            }
        }
    }
// задание 3 RBT
    {
        cout << endl
             << " Задание 3 - RBT" << endl;
        RBTree tree;

        vector<int> keys;
        
        keys.resize(MAX_ELEM);
        for (int i = 0; i < MAX_ELEM; i++)
        {
            keys[i] = i;
            tree.RBTinsert(keys[i]);

            if (i % 10 == 0)
            {
                int max_H = 0;
                int n = 0; //  колисчество узлов в дереве
                find_max_height(tree.root, max_H, n);
                cout << "Максимальная высота " << max_H << " при " << n << " узлов" << endl;
            }
        }
    }
}