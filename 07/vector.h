#pragma once

#include "iterator.h"
#include "allocator.h"
#include <cstring>

using namespace std;

template <class T, class Alloc = Allocator<T>>
class Vector
{
private:
    Alloc alloc_;

public:
    using size_type = size_t;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Alloc;
    using pointer = T*;

    using iterator = Iterator<T>;
    using reverse_iter = reverse_iterator<iterator>;

private:
    pointer data_;
    size_type size_;
    size_type capacity_;

public:

    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit Vector(size_type count)
    {
        size_ = count;
        capacity_ = 2 * count;
        data_ = alloc_.allocate(capacity_);
        pointer cur = data_;
        for ( size_t i = 0; i < size_; ++i ) {
            *cur = T();
            ++cur;
        }
    }

    Vector(size_type count, const value_type& value)
    {
        size_ = count;
        capacity_ = 2 * count;
        data_ = alloc_.allocate(capacity_);
        pointer cur = data_;
        for ( size_t i = 0; i < size_; ++i ) {
            *cur = value;
            ++cur;
        }
    }

    ~Vector() { 
        deallocate(); 
    }

    reference operator[](const size_type id) noexcept {return data_[id];}
    const_reference operator[](const size_type id) const {return data_[id];}

    //iter: begin, end, rbegin, rend
    iterator begin() noexcept {return iterator(data_);}
    iterator end() noexcept {return iterator(data_ + size_);}
    reverse_iter rbegin()  noexcept {return reverse_iter(end());}
    reverse_iter rend()  noexcept {return reverse_iter(begin());}

    //back: push_back, pop_back
    void emplace_back(T&& val)
    {
        if ( size_ == capacity_ ) {
            size_type new_capacity = ( size_ != 0 ) ? 2 * size_ : 1;
            reallocate(new_capacity); 
        }
        pointer dc = data_+ size_;
        *dc = std::move(val);
        ++size_;
    }

    template<typename... Args>
    void emplace_back(T&& val, Args&&... args)
    {
        emplace_back(std::forward<T>(val));
        emplace_back(std::forward<Args>(args)...);
    }

    void push_back(const value_type& value) { 
        emplace_back(static_cast<value_type>(value));
    }

    void push_back(value_type&& value) { 
        emplace_back(std::move(value));
    }

    void pop_back()
    {
        if ( size_ > 0 ) {
            size_--;
            pointer ptr = data_ + size_;
            ptr->~T();
        }
    }

    bool empty() const noexcept {return size_ == 0;}
    size_type size() const noexcept {return size_;}
    size_type capacity() const noexcept {return capacity_;}

    //memory change operation:
    void reserve(size_type new_capacity)
    {
        if (new_capacity > capacity_){
            pointer newData = alloc_.allocate(new_capacity);
            size_type new_size = size_;
            copy(data_, data_ + size_, newData);
            deallocate();
            data_ = newData;
            size_ = new_size;
            capacity_ = new_capacity;
        }
    }

    void resize(size_type new_size, const_reference value)
    {
        if (new_size > size()) {
            if (new_size > capacity()) {
                reallocate(2 * new_size);
            }
            pointer ptr = data_ + size_;
            pointer ptr_end = data_ + new_size;
            while (ptr < ptr_end) {
                *ptr = static_cast<T>(value);
                ++ptr;
            }
            size_ = new_size;
        }
        else if (new_size < size()) {
            pointer ptr = data_ + new_size;
            pointer ptr_end = data_ + size_;
            while (ptr < ptr_end) {
            	ptr->~T();
                ++ptr;
            }
            reallocate(new_size);
            size_ = new_size;
        }
    }

    void resize(size_type new_size)
    {
        if (new_size > size_) {
            if (new_size > capacity_) {
                reallocate(2 * new_size);
            }
            pointer ptr = data_ + size_;
            pointer ptr_end = data_ + new_size;
            while (ptr < ptr_end) {
                *ptr = static_cast<T>(T());
                ++ptr;
            }
            size_ = new_size;
        }
        else if (new_size < size()) {
            pointer ptr = data_ + new_size;
            pointer ptr_end = data_ + size_;
            while (ptr < ptr_end) {
            	ptr->~T();
                ++ptr;
            }
            size_ = new_size;
        }
    }

    void clear() noexcept
    {
        resize(0);
    }


private:
    void reallocate(size_type new_capacity)
    {
        if (new_capacity == 0) {
            deallocate();
        }
        else {
            pointer newData = alloc_.allocate(new_capacity);
            size_type new_size = (size_ <= new_capacity) ? size_ : new_capacity;
            if (data_) {
                copy(data_, data_ + new_size, newData);
                deallocate();
            }
            data_ = newData;
            size_ = new_size;
            capacity_ = new_capacity;
        }
    }

    void deallocate()
    {
        if (data_) {
            pointer cur = data_;
            for (size_t i = 0; i < size_; ++i) {
                cur->~T();
                ++cur;
            }
        }
        alloc_.deallocate(data_, capacity_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }
};

