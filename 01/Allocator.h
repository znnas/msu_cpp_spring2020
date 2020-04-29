#pragma once
#include <cstdlib>

class Allocator
{
    public:
        void makeAllocator(size_t maxSize);
        char* alloc(size_t size);
        void reset();
        ~Allocator();

    private:
        char* start;
        char* end;
        char* cur;

};
