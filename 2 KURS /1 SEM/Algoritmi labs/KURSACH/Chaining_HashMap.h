#pragma once

#include <math.h>
#include <random>
#include <vector>

// мой литкод
// хэш мап это массив из списков, элементы которых каждый содержат ключи
//  и соответствующие им значения
// выбор списка в массиве осуществляется "хэшированием" ключа
//(разные ключи могут захешироваться в 1 число и один список)

template <typename K, typename V, typename Hasher = std::hash<K>>
class Chaining_HashMap
{
    struct Chaining_ListNode // элемент списка с значением и "ключом"
    {
        K key;
        V val;
        Chaining_ListNode *next = 0; // = nullptr
    };

public:
    int NUM_BUCKETS = 5000; // кол-во списоков (бУкетов = корзинок)
    std::vector<Chaining_ListNode *> buckets;
    int cur_size = 0;

    int MyHash(int key) // функция ХЭШИРОВАНИЯ (ставит в соответсвтвие каждому ключу число)
    {
        static const int prime1 = 31;
        static const int prime2 = 65521;
        // это простые числа
        return (key * prime1 + prime2);
    }

    Chaining_HashMap(int in_NUM_BUCKETS = 5000)
    {
        NUM_BUCKETS = in_NUM_BUCKETS;
        buckets.resize(NUM_BUCKETS);
    }
    // добавление ключа и валуе
    void put(K key, V value)
    {
        int list_len = 0;
        static const int REHASH_LIST_LEN = 10;
        int bucket_idx = MyHash(key) % NUM_BUCKETS; // вычисление  индекса списка в массиве бУкетов
        if (buckets[bucket_idx] == nullptr)         // новый эелемент если список пустой
        {
            buckets[bucket_idx] = new Chaining_ListNode();
            cur_size++;
            buckets[bucket_idx]->key = key;
            buckets[bucket_idx]->val = value;
        }
        else
        { // проход по списку и попытка найти там элемент с нужным ключом
            Chaining_ListNode *cur = buckets[bucket_idx];
            while (cur != nullptr)
            {
                if (cur->key == key)
                    break;
                cur = cur->next;
                list_len++;
            }
            if (cur != nullptr)
            {
                cur->val = value; // нашли и записали новое значение
            }
            else
            { // создали новый элемент и делаем его новоой головой в списке котроый лежит в массиве
                cur = new Chaining_ListNode();
                cur_size++;
                cur->key = key;
                cur->val = value;
                cur->next = buckets[bucket_idx]; // указывает на прошлую голову которая хранилась в массиве списков
                buckets[bucket_idx] = cur;       // кур записываем в массив (он стал новой головой)
            }
        }
        if (list_len > REHASH_LIST_LEN)
            rehash(NUM_BUCKETS * 2);
    }

    void rehash(int NEW_NUM_BUCKETS)
    {
        Chaining_HashMap nextMap(NEW_NUM_BUCKETS);
        for (auto &head : buckets)
        {
            Chaining_ListNode *cur = head;
            while (cur != nullptr)
            {
                nextMap.put(cur->key, cur->val);
                cur = cur->next;
            }
        }
        buckets = nextMap.buckets;
        NUM_BUCKETS = NEW_NUM_BUCKETS;
        cur_size = nextMap.cur_size;
    }

    bool get(K key, V &val) // поиск по ключу
    {
        int bucket_idx = MyHash(key) % NUM_BUCKETS;
        Chaining_ListNode *cur = buckets[bucket_idx];
        while (cur != nullptr)
        {
            if (cur->key == key)
                break;
            cur = cur->next;
        }
        if (cur != nullptr)
        {
            val = cur->val;
            return true;
        }
        else
            return false;
    }

    void remove(K key)
    {
        int bucket_idx = MyHash(key) % NUM_BUCKETS;
        Chaining_ListNode *cur = buckets[bucket_idx];
        Chaining_ListNode *prev = nullptr;
        while (cur != nullptr)
        {
            if (cur->key == key)
                break;
            prev = cur;
            cur = cur->next;
        }
        if (cur != nullptr)
        {
            if (prev != 0)
            {
                prev->next = cur->next;
                delete cur;
            }
            else
            {
                buckets[bucket_idx] = cur->next;
                delete cur;
            }
            cur_size--;
        }
    }
};
