#pragma once

#include <math.h>
#include <random>
#include <algorithm>
#include <vector>
#include <iostream>
// у нас открытая адрессация линейное зондирование

// пример решения коллизий для ОТКРЫТОЙ АДРЕСАЦИИ
// https://ru.wikipedia.org/wiki/%D0%9B%D0%B8%D0%BD%D0%B5%D0%B9%D0%BD%D0%BE%D0%B5_%D0%B7%D0%BE%D0%BD%D0%B4%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5

// ВСТАВКА
// 1 - посчитать хэш от ключа, который мы хотим вставить
// 2 - взять от хэша модуль по размеру таблицы и получить инлекс
// 3 - проверить, пуста ли ячейка под этим индексом
// 4 - если пуста, то просто вставить и все
// 5 - а если занята, то надо циклом идти увеличивать индекс на 1 и брать от него модуль
// по размеру мапы, пока не наткнешься на пустое место, куда можно вставить

// ! сохранять текущий размер и сравнивать его с максимальным размером хэштаблицы
// если они равны то вызвать рехэш (масштабировать)
//!!! если таблица перегружена (занято больше 80% ) == фактор загруженности = 0,8 = он превышен,
// то делаем рехэш
// все ключи УНИКАЛЬНЫ ---> если такой уже есть, вернем false как не вставленный

// ПОИСК
//  ! если есть коллизия, то надо искать от места куда привел ХЭШ до 1го пустого места в мапе,
//  если в этом промежутке не нашли, то в мапе нет данногго ключа

// 1 - посчитать хэш от ключа
// 2 - взять модуль от хэша по размеру таблицы и получить таким образом индекс
// 3 - проверить лежит ли под этим индексом искомый ключ
// 4 - если не совпал ключ, значит возможно произошла коллизия и надо проверить
// до 1го пустого места от текущего индекса (если наткунились на пустой и не нашли - ключа нет)

// УДАЛЕНИЕ

// 1 - посчитать хэш от ключа которой мы хотим удалить
// 2 - взять модуль от хэша по размеру таблицы и получить таким образом индекс
// 3 - найти удаляемый ключ начиная с индекса до пустой дырки (если не нашли, то удаляемого элемента в таблице нет выходим)
// 4 - если нашли, запомнить индекс удаляемого ключа (ИД индекс дырки после удаления)
// 5 - идти вниз до первой дырки либо до элемента, ключ которого хэшируется (с взятием модуля)
// в индекс ИТ, который меньше или равен ИД -------> ИТ <= ИД
// ИД = idx_dirka

// 6.1 - если нашли дырку, выходим
// 6.2 - если нашли такой элемент, то меняем его меставми с текущей дыркой и повторяем цикл п. 5
// (ключ под индексом ИТ переезжаем в ячейку с индексом ИД, а индекс ИТ становится новой дыркой ИД для цикла 5)

// шаблон включающий в себя 3 типа - тип ключа, тип значения и тип хэша для ключа
template <typename K, typename V, typename Hasher = std::hash<K>>
struct OA_hash_map
{
    int cur_size = 0; // количество занятых мест в storage
    int max_size = 1000;
    std::vector<std::pair<K, V>> storage; // массив хранящий пары ключ-значение
    const double MAX_LOAD_FACTOR = 0.8;

    // массив показывает по индексу, занято ли это место в мапе (сторедже)
    std::vector<bool> occupied;

    Hasher hasher;

    OA_hash_map(int in_max_size)
    {
        max_size = in_max_size;
        hasher = Hasher();
        storage.resize(max_size);
        occupied.resize(max_size);
    }
    // масштабирование
    void my_rehash(int new_size)
    {
        OA_hash_map nextMap(2 * max_size);
        for (int i = 0; i < max_size; ++i)
        {
            if (occupied[i])
                nextMap.insert(storage[i].first, storage[i].second);
        }
        storage = nextMap.storage;
        occupied = nextMap.occupied;
        max_size *= 2;
    }

    // ВСТАВКА
    bool insert(K key, V val)
    {
        if (((double)cur_size)/storage.size() > MAX_LOAD_FACTOR)
            my_rehash(cur_size); // потом рехэш

        size_t cur_hash = hasher(key); // считаем хэш
        int idx = cur_hash % storage.size();
        while (occupied[idx] == 1)
        {
            // если есть такое значение
            if (storage[idx].first == key)
            {
                // cout вставить не вышло
                return false;
            }
            else
            {
                idx = (idx + 1) % storage.size();
            }
        }
        storage[idx].first = key;
        storage[idx].second = val;
        occupied[idx] = true; // занято
        cur_size++;
        return true;
    }

    // поиск по ключу и получение значения (вернет тру если нашел)
    bool find_and_get(K key, V &val)
    {
        size_t cur_hash = hasher(key); // считаем хэш
        int idx = cur_hash % storage.size();
        // пока не дошли до дырки
        while (occupied[idx] == 1)
        {
            // если есть такое значение
            if (storage[idx].first == key)
            {
                // нашли
                // cout
                val = storage[idx].second;
                return true;
            }
            else
            {
                idx = (idx + 1) % storage.size();
            }
        }
        // cout ne nashli
        return false;
    }

    // УДАЛЕНИЕ
    bool remove(K key)
    {
        size_t cur_hash = hasher(key); // считаем хэш
        int idx = cur_hash % storage.size();
        int dirka_idx = -1; // индекс удаляемого элемента (дырки)
        while (occupied[idx] == 1)
        {
            // если есть такое значение
            if (storage[idx].first == key)
            {
                dirka_idx = idx;
                break;
            }
            else
            {
                idx = (idx + 1) % storage.size();
            }
        }
        if (dirka_idx == -1) // не найден элемент для удаления
            return false;

        bool mod_wrapped = false;    // флаг для отслеживания оборота модуля (типо было 999 а стало 0)
        occupied[dirka_idx] = false; // удаляем
        idx = (idx + 1);
        if (idx == storage.size())
        {
            mod_wrapped = true;
            idx = 0;
        }
        // флаг отслеживающий  обмен дырки и cur_idx
        bool swapped = 1;
        while (swapped)
        {
            swapped = 0;
            // цикл пункта 5 (ходим от дырки к дырке и проверяем, не надо ли поднять на ее место какое-то значение)
            while (occupied[idx] == 1)
            {
                cur_hash = hasher(storage[idx].first);   // считаем хэш
                int cur_idx = cur_hash % storage.size(); // индекс для сравнения куда должен был попоасть элемент
                // если cur_idx <= dirka_idx
                // значит элемент надо поставить на место дырки_идкс

                // из-за модуля при расчета следующего индекса
                //(которые может обернуться) иф снизу должен быть усложнен
                // если модуль при поиске обернулся в ноль, то по флагу отслеживаем и берем условие строго больше
                if ((!mod_wrapped && (cur_idx <= dirka_idx)) || (mod_wrapped && (cur_idx > dirka_idx)))
                {
                    storage[dirka_idx] = storage[idx];
                    occupied[dirka_idx] = true;
                    occupied[idx] = false;
                    dirka_idx = idx;
                    swapped = 1;
                    mod_wrapped = false;
                }
                // отслеживание оборота индекса по модулю
                //(если индекс достиг размера массива сторедж --> обнуляем его, как бы беря модуль)

                idx = (idx + 1);
                if (idx == storage.size())
                {
                    mod_wrapped = true;
                    idx = 0;
                }
            }
        }
        return true;
    }
};
