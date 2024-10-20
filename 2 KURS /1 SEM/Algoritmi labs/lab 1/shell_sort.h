#pragma once
// СОРТИРОВКА РАКУШКА
#include <vector>
#include <math.h>
#include <algorithm>

// тут надо 3 типа gaps через 3 последовательности см табл
std::vector<int> Shell_gaps(int n)
{
    std::vector<int> gaps = {};
    int i = n / 2;
    while (i >= 1)
    {
        gaps.push_back(i);
        i /= 2;
    }
    return gaps;
}

std::vector<int> Hibbard_gaps(int n)
{
    int i = 1;
    std::vector<int> gaps = {};
    while (i < n)
    {
        gaps.push_back(pow(2, i) - 1);
        i++;
    }
    std::reverse(gaps.begin(), gaps.end());
    return gaps;
}

std::vector<int> Pratt_gaps(int n)
{
    static const size_t MAX_IJ = 6;
    // A = 2^i*3^j, i, j >= 0
    std::vector<int> gaps = {};

    for (int i = 0; i < MAX_IJ; ++i) 
    {
        for (int j = 0; j < MAX_IJ; ++j) 
        {
            gaps.push_back(pow(2,i)*pow(3,j));
        }
    }
    std::sort(gaps.begin(), gaps.end());
    std::reverse(gaps.begin(), gaps.end());
    return gaps;
}

template <typename T> // типо "универсальный" тип Т
// это шаблон (позволяет нашей сортировке работать с векторами содержащими
// элементы любых типов)
void shell_sort(std::vector<T> &to_sort, std::vector<int> &gaps)
{
    if (gaps.empty())
    {
        std::cout << " ОШИБКА gaps empty" << std::endl;
    }

    for (auto &gap : gaps)
    {
        for (int i = gap; i < to_sort.size(); i += gap)
        {
            T tmp = to_sort[i];
            int j = i;
            while (j >= gap && to_sort[j - gap] > tmp)
            {
                to_sort[j] = to_sort[j - gap];
                j = j - gap;
            }
            to_sort[j] = tmp;
        }
    }
}