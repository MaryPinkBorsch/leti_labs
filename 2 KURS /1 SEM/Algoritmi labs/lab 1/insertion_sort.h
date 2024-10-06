#pragma once
// СОРТИРОВКА ВСТАВАКАМИ
#include <vector>
#include <algorithm>

// типо "универсальный" тип Т
// это шаблон (позволяет нашей сортировке работать с векторами содержащими
// элементы любых типов)
template <typename T> 
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

// бинарный поиск value в подмассиве [start, finish) массива to_sort
// функция возвращает либо индекс первого элемента со значением value
// либо количество элементов которые меньше value
template <typename T> 
int insertion_binary_search(std::vector<T> &to_sort, int start, int finish, T value) 
{
    int L = start;
    int R = finish;
    while (L < R) 
    {
        int mid = L + (R-L)/2;
        if (to_sort[mid] < value)
        {
            L = mid + 1;
        }
        else if (to_sort[mid] >= value)
        {
            R = mid;
        }
    }
    return L;
}

// улучшенная версия
template <typename T> 
void insertion_sort_improved(std::vector<T> &to_sort)
{
   for (int i = 0; i < to_sort.size(); ++i)  
   {
        // идем слева направо, все что слева от i отсортировано а то что справа еще нет
        if (i > 0)  // если отсортированная часть не пустая
        {
            // сохраняем итый элемент и ищем его бинарным поиском в отсортированной части
            T value = to_sort[i];
            int pos = insertion_binary_search(to_sort, 0, i, value);
            if (pos != i) 
            {
                for (int j = i; j != pos; --j) // сдвигаем часть осторированной части начиная с пос на единицу вправо (перезапишет value)
                    to_sort[j] = to_sort[j-1];
                to_sort[pos] = value; // вставляем на освободившееся место value
            }
        }
   }
}