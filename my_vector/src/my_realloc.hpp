#pragma once
#include <iostream>

/**
 * @brief accept buffer with old_size elements
 * and create buffer with new_size elements
 * @tparam T
 * @param buf
 * @param old_size
 * @param new_size
 * @return buffer with new_size elements
 */

template <typename T>
T* my_realloc(T* buf, int old_size ,int new_size)
{
    if(old_size < 0 || new_size < 0) {
        std::cerr << "Size less than zero in" << __PRETTY_FUNCTION__ << std::endl;
        return nullptr;
    }

    T* new_array = new(std::nothrow) T[new_size];

    if(!new_array) {
        std::cerr << "Can't allocate mem! in" << __PRETTY_FUNCTION__ << std::endl;
        return nullptr;
    }

    int copy_size = 0;
    new_size > old_size ? copy_size = old_size : copy_size = new_size;

    for(int i = 0; i < copy_size; i++)
        new_array[i] = buf[i];

    if(old_size > 1)
        delete[] buf;
    else if (old_size == 1)
        delete buf;

    return new_array;
}
