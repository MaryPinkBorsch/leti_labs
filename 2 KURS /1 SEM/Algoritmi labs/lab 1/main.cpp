#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "selection_sort.h"
#include "insertion_sort.h"
#include "boggo_sort.h"
#include "merge_sort.h"
#include "q_sort.h"
#include "heap_sort.h"

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
    // проверка валидности
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
        std::vector<std::string> A = {"Ivanov", "Bvanov", "Dvanov", "Gvanov", "Avanov", "Cvanov"};
        std::vector<std::string> A_sorted = A;
        insertion_sort(A_sorted);
        check_sorting(A, A_sorted);
    }
    {
        std::vector<int> A_sorted = A;
        boggo_sort(A_sorted);
        check_sorting(A, A_sorted);
    }
    {
        std::vector<int> A_sorted = A;
        merge_sort(A_sorted);
        check_sorting(A, A_sorted);
    }
    {
        std::vector<int> A_sorted = A;
        q_sort(A_sorted);
        check_sorting(A, A_sorted);
    }
    {
        std::vector<int> A_sorted = A;
        insertion_sort_improved(A_sorted);
        check_sorting(A, A_sorted);
    }
    {
        std::vector<int> A_sorted = A;
        heap_sort(A_sorted);
        check_sorting(A, A_sorted);
    }

    static int NUM_ELEMENTS = 10000; // миллион
    std::vector<int> sorted_already;
    std::vector<int> randomized;
    std::vector<int> reverse_sorted;
    for (int i = 0; i < NUM_ELEMENTS; i++)
    {
        sorted_already.push_back(i);
        reverse_sorted.push_back(NUM_ELEMENTS - i - 1);
    }
    randomized = sorted_already;
    std::random_shuffle(randomized.begin(), randomized.end());

    // замеры времени
    // selection_sort замеры
    {
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            selection_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "selection_sort, sorted_already " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            selection_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "selection_sort, randomized " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            selection_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "selection_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
    }
    // insertion_sort замеры
    {
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            insertion_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, sorted_already " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            insertion_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, randomized " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            insertion_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
    }
    // boggo_sort замеры
    {
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            boggo_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, sorted_already " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            boggo_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, randomized " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            boggo_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
    }
    // merge_sort замеры
    {
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            merge_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, sorted_already " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            merge_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, randomized " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            merge_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
        }
    }

    return 0;
}