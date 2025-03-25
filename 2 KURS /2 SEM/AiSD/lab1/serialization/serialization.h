#pragma once

#include <type_traits>
#include <deque> // двустороння очередь
#include <cstring>

// простейшая сериализация \ десериализация
// штука для записи (и считывания) разных структур в бинарный файл (чтоб они сохранились)

// запись
template <typename T>
typename std::enable_if<std::is_fundamental<T>::value>::type // эта реализация штук только
// для фундаментальных типов (простейших) типо инт, чар бул и тп
serialize(std::deque<char> &buffer, const T &val)
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
deserialize(const std::deque<char> &buffer, T &val, size_t &idx)
{
    std::memcpy(&val, &(buffer[idx]), sizeof(T)); // записали память в val из буфера по индексу
    idx += sizeof(T);
}