#pragma once
// СОРТИРОВКА ПУЗЫРЬКОМ
#include <vector>
#include <algorithm>

template <typename T> // типо "универсальный" тип Т
// это шаблон (позволяет нашей сортировке работать с векторами содержащими
// элементы любых типов)

void bubble_sort(std::vector<T> &to_sort)
{
    int n = to_sort.size();
    bool Sorted = false;
    while (Sorted == false)
    {
        Sorted = true;
        for (int i = 0; i < n - 1; i++)
        {
            if (to_sort[i] > to_sort[i + 1])
            {
                std::swap(to_sort[i], to_sort[i + 1]);
                Sorted = false;
            }
        }
    } 
}