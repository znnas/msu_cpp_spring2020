#pragma once

#include <iterator>
using namespace std;

template <class T>
class Iterator: public iterator<bidirectional_iterator_tag, T>
{
private:
    T* ptr_;

public:
    Iterator() : ptr_(nullptr) {}
    explicit Iterator(T* ptr) : ptr_(ptr) {}

    bool operator==(const Iterator<T>& other) const {return ptr_ == other.ptr_;}
    bool operator!=(const Iterator<T>& other) const {return !(*this == other);}

    T& operator[](size_t n) const {return ptr_[n];}
    T& operator*() const {return *ptr_;}

    Iterator<T>& operator++() {++ptr_; return *this;}
    Iterator<T>& operator--() {--ptr_; return *this;}
};

