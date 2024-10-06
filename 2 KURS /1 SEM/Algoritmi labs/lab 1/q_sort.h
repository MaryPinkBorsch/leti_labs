#pragma once
// СОРТИРОВКА ВЫБОРОМ
#include <vector>
#include <algorithm>

// типо "универсальный" тип Т
// это шаблон (позволяет нашей сортировке работать с векторами содержащими
// элементы любых типов)
template <typename T> // опять открытый интервал [start, finish)
void q_sort_recursia(std::vector<T> &to_sort, int start, int finish)
{
    if (finish - start == 2)
        return;
    int pivot_idx = start + (finish - start) / 2; // выбираем пивот где-то в середине
    T pivot = to_sort[pivot_idx];
    int L = start, R = finish - 1; // обозначим левую и правую границу

    while (L < R) // пока границы не наехали др на друга
    {
        while (to_sort[L] < pivot && L < R) // пока элементы в левой половине меньше пивота сдвигаем Л вправо 
            L++;
        while (to_sort[R] > pivot && L < R) // пока все элеме-ты в правой части больше или = пивота тоже сдвигаем
            R--;
        if (L < R) 
        {
            std::swap(to_sort[L], to_sort[R]); // меняем элементы местами
            //таким образом массив делим на 2 части - слева элементы <pivot, справа элементы >= pivot 
        }
        else //если границы переехались (L == R)
        {
            if (L > start) //проверка что есть левая часть
                q_sort_recursia(to_sort, start, L + 1);
            if (R + 1 < finish) //проверка что есть правая часть
                q_sort_recursia(to_sort, R + 1, finish);
        }
    }
}

template <typename T>
void q_sort(std::vector<T> &to_sort) 
{
    q_sort_recursia(to_sort, 0, to_sort.size());
}

