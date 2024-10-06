#pragma once
// СОРТИРОВКА СЛУЧАЙНАЯ
#include <vector>
#include <algorithm>

template <typename T> // типо "универсальный" тип Т
// это шаблон (позволяет нашей сортировке работать с векторами содержащими
// элементы любых типов)
void boggo_sort(std::vector<T> &to_sort)
{
    if (to_sort.empty())
        return;

    while (true)
    {
        bool sorted = true;
        std::random_shuffle(to_sort.begin(), to_sort.end());
        for (int i = 0; i < to_sort.size() - 1; i++)
        {
            if (to_sort[i] > to_sort[i + 1])
            {
                sorted = false;
                break;
            }
        }
        if(sorted)
            break;
    }
}