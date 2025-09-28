#ifndef ARRAY_HPP_INCLUDED
#define ARRAY_HPP_INCLUDED

#include <cstddef>
#include <memory>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <cstring>

using std::size_t;

#include "pjc-traits.hpp"


template <typename T>
void print_array(const T* arr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';
}

// Copy for constructor function
template <typename T>
typename std::enable_if<pjc::is_trivially_copyable<T>::value>::type
copy_array_const(const T* from, T* to, size_t size) {
    if (size > 0) {
        std::memcpy(to, from, size * sizeof(T));
    }
}

template <typename T>
typename std::enable_if<!pjc::is_trivially_copyable<T>::value>::type
copy_array_const(const T* from, T* to, size_t size) {
    std::cout << "Copy template" << std::endl;
    for (size_t i = 0; i < size; ++i) {
        to[i] = from[i];
    }
}

// Copy for resize function
template <typename T>
typename std::enable_if<pjc::is_trivially_copyable<T>::value>::type
copy_array(const T* from, T* to, size_t size) {
    std::memcpy(to, from, size * sizeof(T));
}

template <typename T>
typename std::enable_if<std::is_nothrow_move_assignable<T>::value && !pjc::is_trivially_copyable<T>::value>::type
copy_array(T* from, T* to, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        to[i] = std::move(from[i]);
    }
}

template <typename T>
typename std::enable_if<!std::is_nothrow_move_assignable<T>::value>::type
copy_array(const T* from, T* to, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        to[i] = from[i];
    }
}

template <typename T>
void resize_array(std::unique_ptr<T[]>& arr, size_t oldSize, size_t newSize) {
    auto newArr = std::make_unique<T[]>(newSize);
    size_t minSize = (oldSize < newSize) ? oldSize : newSize;
    copy_array(arr.get(), newArr.get(), minSize);
    newArr.swap(arr);
}
#endif
