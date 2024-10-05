#pragma once
// СОРТИРОВКА ВЫБОРОМ
#include <vector>
#include <algorithm>

template <typename T> // типо "универсальный" тип Т
// это шаблон (позволяет нашей сортировке работать с векторами содержащими
// элементы любых типов)
void selection_sort(std::vector<T> & to_sort)
{
    if (to_sort.empty())
        return;

    for (int i = 0; i < to_sort.size(); i++)
    {
        T min = to_sort[i];
        int idx = i;
        for (int j = i; j < to_sort.size(); j++)
        {
            if (to_sort[j] < min)
            {
                idx = j;
                min = to_sort[j];
            }
        }
        if (idx != i)
            std::swap(to_sort[i], to_sort[idx]);
    }
    
}