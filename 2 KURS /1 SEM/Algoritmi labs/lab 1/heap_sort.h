#pragma once
// СОРТИРОВКА HEAPom
#include <vector>
#include <algorithm>
// 1) в heap родитель должен быть больше или меньше обоих детей в зависимости от типа хипа (мин и макс) (сам хип можно представить как дерево)
// 2) если родитель находится по индексу i то его дети находятся по индексам 2*i + 1 и 2*i + 2

template <typename T> // n = len массива
void heapify(std::vector<T> &to_sort, int n, int i)
{
    int largest = i;   // берем корень за Наибольший
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // если левый деть больше Наибольшего
    if (l < n && to_sort[l] > to_sort[largest])
        largest = l;

    // если правый деть больше Наибольшего
    if (r < n && to_sort[r] > to_sort[largest])
        largest = r;

    // если Наибольший сменился
    if (largest != i)
    {
        std::swap(to_sort[i], to_sort[largest]);

        // рекрсивно восстанавливаем свойства макс хипа для его детей
        heapify(to_sort, n, largest);
    }
}

// типо "универсальный" тип Т
// это шаблон (позволяет нашей сортировке работать с векторами содержащими
// элементы любых типов)
template <typename T>
void heap_sort(std::vector<T> &to_sort)
{
    int n = to_sort.size(); // длина массива
    // хиипизируем массив
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(to_sort, n, i);

    for (int i = n - 1; i >= 0; i--)
    {
        // самый большой элемент(0й индекс в макс хипе) извлекается и меняется с элементом
        // на границе отсортированной последовательности в конце массива (он еще не отсортирован)
        std::swap(to_sort[0], to_sort[i]);

        // после обмена надо восстановить свойства хипа
        heapify(to_sort, i, 0);
    }
}
