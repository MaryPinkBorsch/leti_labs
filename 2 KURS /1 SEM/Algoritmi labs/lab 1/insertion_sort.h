#pragma once
// СОРТИРОВКА ВСТАВАКАМИ
#include <vector>
#include <algorithm>
template <typename T> // типо "универсальный" тип Т
// это шаблон (позволяет нашей сортировке работать с векторами содержащими
// элементы любых типов)

void insertion_sort(std::vector<T> &to_sort)
{
    if (to_sort.empty())
        return;

    for (int i = 0; i < to_sort.size(); i++)
    {
        int idx = i;
        for (int j = i - 1; j >= 0; j--)
        {
            if (to_sort[j] < to_sort[idx])
                break;
            else
            {
                std::swap(to_sort[j], to_sort[idx]);
                idx = j;
            }
        }
    }
}