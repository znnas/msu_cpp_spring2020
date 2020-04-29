#include "Allocator.h"

void Allocator::makeAllocator(size_t maxSize)
{
    start = (char*)malloc(maxSize * sizeof(char));
    if (start != nullptr) {
        end = start+(maxSize - 1) * sizeof(char);
    }
    else {
        end = nullptr;
    }
    cur = start;
}

char* Allocator::alloc(size_t size)
{
    if (start == nullptr) {
        return nullptr;
    }
    if (cur + (size - 1) * sizeof(char) > end) {
        return nullptr;
    }
    else {
        char* cur_start = cur;
        cur += size * sizeof(char);
        return cur_start;
    }
}

void Allocator::reset()
{
    cur = start;
}

Allocator::~Allocator()
{
    if (start != nullptr) {
        free(start);
    }
}

