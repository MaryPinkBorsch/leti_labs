#pragma once

#include <type_traits>
#include <vector>
#include <cstring>
#include <forward_list>

// простейшая сериализация \ десериализация
// штука для записи (и считывания) разных структур в бинарный файл (чтоб они сохранились)

// запись
template <typename T>
typename std::enable_if<std::is_fundamental<T>::value>::type // эта реализация штук только
// для фундаментальных типов (простейших) типо инт, чар бул и тп
serialize(std::vector<char> &buffer, const T &val)
{
    size_t last_idx = buffer.size();                   // индекс куда писать
    buffer.insert(buffer.end(), sizeof(T), 0);         // добавили место под T val
    std::memcpy(&(buffer[last_idx]), &val, sizeof(T)); // записали память из val в
    // только что выделенную память в buffer
}

// чтение
template <typename T>
typename std::enable_if<std::is_fundamental<T>::value>::type // эта реализация штук только
// для фундаментальных типов (простейших) типо инт, чар бул и тп
deserialize(const std::vector<char> &buffer, T &val, size_t &idx)
{
    std::memcpy(&val, &(buffer[idx]), sizeof(T)); // записали память в val из буфера по индексу
    idx += sizeof(T);
}

// это для сериализации вектора на шаблоне (можно любой тип туда пихнуть)
template <typename T>
void serialize(std::vector<char> &buffer, const std::vector<T> &val)
{
    serialize(buffer, val.size()); // это мы записали в начале сколько элементов в векторе мы сериализовали
    for (auto &it : val)
        serialize(buffer, it);
}

template <typename T>
void deserialize(const std::vector<char> &buffer, std::vector<T> &val, size_t &idx)
{
    size_t size = 0;
    deserialize(buffer, size, idx); // теперь мы знаем сколько элементов надо считать в вектор из вектора
    val.resize(size);
    for (int i = 0; i < size; i++)
        deserialize(buffer, val[i], idx);
}

// это для сериализации forward_list на шаблоне (можно любой тип туда пихнуть)
template <typename T>
void serialize(std::vector<char> &buffer, const std::forward_list<T> &val)
{
    serialize(buffer, val.size()); // это мы записали в начале сколько элементов в списке мы сериализовали
    for (auto &it : val)
        serialize(buffer, it);
}

template <typename T>
void deserialize(const std::vector<char> &buffer, std::forward_list<T> &val, size_t &idx)
{
    size_t size = 0;
    deserialize(buffer, size, idx); // теперь мы знаем сколько элементов надо считать в список из вектора

    for (int i = 0; i < size; i++)
    {
        T tmp;
        deserialize(buffer, tmp, idx);
        val.push_front(tmp);
    }
    val.reverse();
}