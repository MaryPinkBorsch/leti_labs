#include "BST.h"
#include "RBT.h"
#include "AVL.h"
#include "common_tree.h"

using namespace std;

// ПЛАН!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// 1. Реализовать бинарное дерево поиска, красно-черное дерево и АВЛ дерево
//(структура, балансировка, операции вставки/удаления/поиска).

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
    // AVL
    {
        AVL_node *root = new AVL_node(50);
        AVLinsert(root, 25);
        AVLinsert(root, 75);
        AVLinsert(root, 100);
        AVLinsert(root, 30);
        AVLinsert(root, 1);
        inOrder_print(root);
        root = AVLdelete(root, 50);
        std::cout << std::endl;
        inOrder_print(root);
        AVL_node *prev = 0;
        AVL_node *t = AVLsearch(root, 25, prev);
        AVL_node *tmp = AVLsearch(root, 666, prev);
    }
    // BST
    {
        BST_node *root = new BST_node(50);
        BSTinsert(root, 25);
        BSTinsert(root, 75);
        BSTinsert(root, 100);
        BSTinsert(root, 30);
        BSTinsert(root, 1);
        inOrder_print(root);
        root = BSTdelete(root, 50);
        std::cout << std::endl;
        inOrder_print(root);
        std::cout << std::endl;
        BST_node *prev = 0;
        BST_node *t = BSTsearch(root, 25, prev);
        BST_node *tmp = BSTsearch(root, 666, prev);
    }

    // RBT
    {
        RBTree tree;
        tree.root = new RBT_node(50);
        tree.RBTinsert(25);
        tree.RBTinsert(75);
        tree.RBTinsert(100);
        tree.RBTinsert(30);
        tree.RBTinsert(1);

        inOrder_print(tree.root);
        tree.delete_this_Value(50);
        std::cout << std::endl;
        inOrder_print(tree.root);
        std::cout << std::endl;
        RBT_node *prev = 0;
        tree.RBTsearch(25);
        tree.RBTsearch(666);
    }
}