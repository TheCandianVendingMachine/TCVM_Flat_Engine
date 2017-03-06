#include "feNew.hpp"
#include "memoryManager.hpp"

void *feAlloc(size_t bytes)
    {
        fe::memoryManager::get().getMemoryLogger().allocateOverloadedNew(bytes);
        return fe::memoryManager::get().alloc(bytes);
    }

void feDelete(void *memory)
    {
        // since the memory manager manages a memory buffer, we dont
        // need to free any memory allocated. If we implement a dynamic
        // memory allocater however, we will implement this
    }