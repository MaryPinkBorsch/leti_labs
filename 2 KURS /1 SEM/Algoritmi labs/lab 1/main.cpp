#include <iostream>
#include <vector>
#include <algorithm>

#include "selection_sort.h"
#include "insertion_sort.h"

using namespace std;

// функция для сравнения результата сортировки с результатом работы функции из стандартной библиотеки
template <typename T>
void check_sorting(std::vector<T> &unsorted, std::vector<T> &to_check)
{
    std::vector<T> copy = unsorted;
    std::sort(copy.begin(), copy.end());
    if (copy != to_check)
    {
        std::cout << "Массив отсортирован неправильно!" << std::endl;
        abort();
    }
}

int main(int argc, char *argv[])
{
    std::vector<int> A = {58, 934, 1, 0, -54, 605, 19, 18, 25, 2005};
    {
        std::vector<int> A_sorted = A;
        selection_sort(A_sorted);
        check_sorting(A, A_sorted);
    }
    {
        std::vector<int> A_sorted = A;
        insertion_sort(A_sorted);
        check_sorting(A, A_sorted);
    }
    {
        std::vector<std::string> A = {"Ivanov","Bvanov","Dvanov","Gvanov","Avanov","Cvanov"};
        std::vector<std::string> A_sorted = A;
        insertion_sort(A_sorted);
        check_sorting(A, A_sorted);
    }

    return 0;
}