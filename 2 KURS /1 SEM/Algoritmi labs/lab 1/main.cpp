#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <chrono>

#include "selection_sort.h"
#include "insertion_sort.h"
#include "boggo_sort.h"
#include "merge_sort.h"
#include "q_sort.h"
#include "heap_sort.h"
#include "bubble_sort.h"
#include "shell_sort.h"

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

int tests() 
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
    {
        std::vector<int> A_sorted = A;
        std::vector<int> gaps = Shell_gaps(A.size());
        shell_sort(A_sorted, gaps);
        check_sorting(A, A_sorted);
    }
    {
        std::vector<int> A_sorted = A;
        std::vector<int> gaps = Hibbard_gaps(A.size());
        shell_sort(A_sorted, gaps);
        check_sorting(A, A_sorted);
    }
    {
        std::vector<int> A_sorted = A;
        std::vector<int> gaps = Pratt_gaps(A.size());
        shell_sort(A_sorted, gaps);
        check_sorting(A, A_sorted);
    }

    // НАЧАЛО ОБРАБОТКИ!!!!

    static int NUM_ELEMENTS = 100000; // от 1000 до 100.000 с шагом 10 К
    std::vector<int> sorted_already;
    std::vector<int> randomized;
    std::vector<int> reverse_sorted;
    std::vector<int> almost_sorted;
    for (int i = 0; i < NUM_ELEMENTS; i++)
    {
        sorted_already.push_back(i);
        reverse_sorted.push_back(NUM_ELEMENTS - i - 1);
    }
    randomized = sorted_already;
    almost_sorted = sorted_already;
    for (int i = 0; i < NUM_ELEMENTS / 20; ++i)
    {
        // чтобы получить 10% / 90% отсортированную последовательность
        // мы берем отсортированную и меняем 10% случайно взятых элементов местами
        std::swap(almost_sorted[rand() % almost_sorted.size()], almost_sorted[rand() % almost_sorted.size()]);
    }
    std::random_shuffle(randomized.begin(), randomized.end());

    // замеры времени
    cout << "Размер входных данных: " << NUM_ELEMENTS << endl
         << endl;
    // selection_sort замеры
    {
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            selection_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "selection_sort, sorted_already " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            selection_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "selection_sort, randomized " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = almost_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            selection_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "selection_sort, almost_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            selection_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "selection_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl
                      << std::endl;
        }
    }
    // insertion_sort замеры
    {
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            insertion_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, sorted_already " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            insertion_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, randomized " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = almost_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            insertion_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, almost_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            insertion_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "insertion_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl
                      << std::endl;
        }
    }

    // boggo_sort замеры  \\ ВРОДЕ НЕ НАДО ДЛЯ ЛАБЫ
    // {
    //     {
    //         std::vector<int> copy = sorted_already;
    //         std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    //         boggo_sort(copy);
    //         std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    //         std::cout << "boggo_sort, sorted_already " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "ms" << std::endl;
    //     }
    //     {
    //         std::vector<int> copy = randomized;
    //         std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    //         boggo_sort(copy);
    //         std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    //         std::cout << "boggo_sort, randomized " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "ms" << std::endl;
    //     }
    //     {
    //         std::vector<int> copy = reverse_sorted;
    //         std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    //         boggo_sort(copy);
    //         std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    //         std::cout << "boggo_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "ms" << std::endl;
    //     }
    // }

    // merge_sort замеры
    {
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            merge_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "merge_sort, sorted_already " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            merge_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "merge_sort, randomized " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = almost_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            merge_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "merge_sort, almost_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            merge_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "merge_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl
                      << std::endl;
        }
    }

    // heap_sort замеры
    {
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            heap_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "heap_sort, sorted_already " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            heap_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "heap_sort, randomized " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = almost_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            heap_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "heap_sort, almost_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            heap_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "heap_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl
                      << std::endl;
        }
    }

    // q_sort замеры
    {
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            q_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "q_sort, sorted_already " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            q_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "q_sort, randomized " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = almost_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            q_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "q_sort, almost_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            q_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "q_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl
                      << std::endl;
        }
    }
    // bubble_sort замеры
    {
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            bubble_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "bubble_sort, sorted_already " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            bubble_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "bubble_sort, randomized " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = almost_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            bubble_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "bubble_sort, almost_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            bubble_sort(copy);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "bubble_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl
                      << std::endl;
        }
    }

    // замеры времени
    // shell_sort замеры
    {
        // SHELL
        std::vector<int> gaps = Shell_gaps(NUM_ELEMENTS);
        cout << "SHELL GAPS: " << endl;
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, sorted_already " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, randomized " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = almost_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, almost_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl
                      << std::endl;
        }

        // HIBBARD
        cout << "HIBBARD GAPS: " << endl;
        gaps = Hibbard_gaps(NUM_ELEMENTS);
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, sorted_already " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, randomized " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = almost_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, almost_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl
                      << std::endl;
        }

        // PRATT
        cout << "PRATT GAPS: " << endl;
        gaps = Pratt_gaps(NUM_ELEMENTS);
        {
            std::vector<int> copy = sorted_already;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, sorted_already " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = randomized;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, randomized " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = almost_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, almost_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl;
        }
        {
            std::vector<int> copy = reverse_sorted;
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            shell_sort(copy, gaps);
            std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
            std::cout << "shell_sort, reverse_sorted " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << "ms" << std::endl
                      << std::endl;
        }
    }
    return 0;
}

enum SortAlgorithm : int
{
    BUBBLE_SORT = 0,
    INSERTION_SORT,
    HEAP_SORT,
    QUICK_SORT,
    MERGE_SORT,
    SELECTION_SORT,
    SHELL_SORT,
    SHELL_SORT_HIBBARD,
    SHELL_SORT_PRATT,
    SORTING_ALGORITHM_MAX
};

enum DataMode : int
{
    SORTED,
    ALMOST_SORTED,
    REVERSE_SORTED,
    RANDOMIZED,
    DATA_MODE_MAX
};

std::unordered_map<int, std::string> SortAlgorithmNames;
std::unordered_map<int, std::string> DataModeNames;

int main(int argc, char *argv[])
{
    SortAlgorithmNames[BUBBLE_SORT] = "Bubble";
    SortAlgorithmNames[INSERTION_SORT] = "Insertion";
    SortAlgorithmNames[HEAP_SORT] = "Heap";
    SortAlgorithmNames[QUICK_SORT] = "Quick";
    SortAlgorithmNames[MERGE_SORT] = "Merge";
    SortAlgorithmNames[SELECTION_SORT] = "Selection";
    SortAlgorithmNames[SHELL_SORT] = "Shell";
    SortAlgorithmNames[SHELL_SORT_HIBBARD] = "Shell Hibbard";
    SortAlgorithmNames[SHELL_SORT_PRATT] = "Shell Pratt";

    DataModeNames[SORTED] = "Sorted";
    DataModeNames[ALMOST_SORTED] = "Almost Sorted";
    DataModeNames[REVERSE_SORTED] = "Reverse Sorted";
    DataModeNames[RANDOMIZED] = "Randomized";

    static const size_t DATA_SIZE_MIN = 10000;
    static const size_t DATA_SIZE_MAX = 1000000;
    static const size_t DATA_SIZE_STEP = 10000;

    std::unordered_map<int, std::vector<int>> test_data;

    for (int i = 0; i < DATA_SIZE_MAX; i++)
    {
        test_data[SORTED].push_back(i);
        test_data[REVERSE_SORTED].push_back(DATA_SIZE_MAX - i - 1);
    }
    test_data[RANDOMIZED] = test_data[SORTED];
    std::random_shuffle(test_data[RANDOMIZED].begin(), test_data[RANDOMIZED].end());
    test_data[ALMOST_SORTED] = test_data[SORTED];
    for (int i = 0; i < DATA_SIZE_MAX / 20; ++i)
    {
        // чтобы получить 10% / 90% отсортированную последовательность
        // мы берем отсортированную и меняем 10% случайно взятых элементов местами
        std::swap(test_data[ALMOST_SORTED][rand() % test_data[ALMOST_SORTED].size()], test_data[ALMOST_SORTED][rand() % test_data[ALMOST_SORTED].size()]);
    }

    for (int i = 0; i < SORTING_ALGORITHM_MAX; ++i) 
    {
        std::cout << SortAlgorithmNames[(SortAlgorithm)i] << std::endl;
        for (int j = 0; j < DATA_MODE_MAX; ++j) 
        {
            std::cout << DataModeNames[(DataMode)j] << std::endl;
            for (size_t data_size = DATA_SIZE_MIN; data_size <= DATA_SIZE_MAX; data_size += DATA_SIZE_STEP) 
            {
                std::vector<int> to_sort;
                to_sort.insert(to_sort.begin(), test_data[(DataMode)j].begin(), test_data[(DataMode)j].begin() + data_size);
                std::vector<int> gaps;
                switch ((SortAlgorithm)i) 
                {
                    case SHELL_SORT:
                    gaps = Shell_gaps(data_size);
                    break;
                    case SHELL_SORT_HIBBARD:
                    gaps = Hibbard_gaps(data_size);
                    break;
                    case SHELL_SORT_PRATT:
                    gaps = Pratt_gaps(data_size);
                    break;
                    default:
                    break;
                }


                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
                switch ((SortAlgorithm)i) 
                {
                    case BUBBLE_SORT:
                        bubble_sort(to_sort);
                        break;
                    case INSERTION_SORT:
                        insertion_sort(to_sort);
                        break;
                    case HEAP_SORT:
                        heap_sort(to_sort);
                        break;
                    case QUICK_SORT:
                        q_sort(to_sort);
                        break;
                    case MERGE_SORT:
                        merge_sort(to_sort);
                        break;
                    case SELECTION_SORT:
                        selection_sort(to_sort);
                        break;
                    case SHELL_SORT:
                        shell_sort(to_sort, gaps);
                        break;
                    case SHELL_SORT_HIBBARD:
                        shell_sort(to_sort, gaps);
                        break;
                    case SHELL_SORT_PRATT:
                        shell_sort(to_sort, gaps);
                        break;
                    default:
                    break;
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << SortAlgorithmNames[(SortAlgorithm)i]<< " " << DataModeNames[(DataMode)j] << " " << data_size << " elements " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << " ms" << std::endl;
            }
        }
    }
    return 0;
}