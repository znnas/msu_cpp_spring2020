#pragma once

#include <memory>
//#include <limits>

using namespace std;

template <class T>
class Allocator
{
public:
	using value_type = T;
    using pointer = T*;
    using size_type = size_t;

public:
    pointer allocate(size_type count)
    {
        if (count > max_size())
            throw bad_alloc();
        T* ptr = static_cast<T*>(::operator new(sizeof(T) * count));
        if (!ptr)
            throw bad_alloc();
        return ptr;
    }

    void deallocate(pointer ptr, size_t count)
    {
        ::operator delete(ptr, sizeof(T) * count);
    }

    size_t max_size() const noexcept
    {
        return numeric_limits<size_t>::max();
    }
};
