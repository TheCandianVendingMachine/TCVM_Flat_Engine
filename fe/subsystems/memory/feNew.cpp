#include "memoryManager.hpp"
#include <cstdlib>

void *operator new(size_t bytes)
    {
        //fe::memoryManager::get().getMemoryLogger().allocateOverloadedNew(bytes);
        if (fe::memoryManager::exists())
            {
                //return fe::memoryManager::get().alloc(bytes);
                return fe::memoryManager::get().getStackAllocater().alloc(bytes);
            }

        return malloc(bytes);
    }

void operator delete(void *memory)
    {
        // since the memory manager manages a memory buffer, we dont
        // need to free any memory allocated. If we implement a dynamic
        // memory allocater however, we will implement this

        if (!fe::memoryManager::exists())
            {
                free(memory);
            }
    }
