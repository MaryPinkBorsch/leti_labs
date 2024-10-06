#pragma once
// СОРТИРОВКА СЛИЯНИЕМ
#include <vector>
#include <algorithm>

// шаблон писать к кажой функ-ии/ классу, который должен принимать шаблон
template <typename T> // старт и енд - открытый интервал [start,end) т.е. мы енд не включаем
void merge_sort_recursia(std::vector<T> &to_sort, int start, int end)
{
    if (end - start == 1) // любой массив длиной 1 уже отсортирован
        return;
    int mid = start + (end - start) / 2;
    merge_sort_recursia(to_sort, start, mid);
    merge_sort_recursia(to_sort, mid, end);
    // после того как рекурсивные вызовы отработают интервалы [start,mid) и [mid,end) отсортированы
    // теперь их надо соединить
    std::vector<T> tmp;
    int p1 = start; // указатель по 1му подмассиву
    int p2 = mid;   // указатель по 2му подмассиву
    while (true)
    {
        if (p1 != mid && p2 != end) // оба подмассива еще не кончились
        {
            if (to_sort[p1] < to_sort[p2])
            {
                tmp.push_back(to_sort[p1]);
                p1++;
            }
            else
            {
                tmp.push_back(to_sort[p2]);
                p2++;
            }
        }
        else if (p1 == mid && p2 != end) // 1й кончился, а 2й не кончился, надо запихать весь второй в тмп
        {
            while (p2 != end)
            {
                tmp.push_back(to_sort[p2]);
                p2++;
            }
        }
        else if (p1 != mid && p2 == end) // 2й кончился, а 1й не кончился, надо запихать весь 1й в тмп
        {
            while (p1 != mid)
            {
                tmp.push_back(to_sort[p1]);
                p1++;
            }
        }
        else
            break;
    }
    // на данном этапе тмп содержит отсортированную посл-ть элементов из обоих подмассивов
    // надо перезаписать в подмассив ту_сорт содержимое тмп
    for (int i = start; i < end; i++)
    {
        to_sort[i] = tmp[i - start];
    }
}

// типо "универсальный" тип Т
// это шаблон (позволяет нашей сортировке работать с векторами содержащими
// элементы любых типов)
template <typename T>
void merge_sort(std::vector<T> &to_sort)
{
    merge_sort_recursia(to_sort, 0, to_sort.size());
}