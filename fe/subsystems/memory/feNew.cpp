#include "memoryManager.hpp"
#include <cstdlib>

void *operator new(size_t bytes)
    {
        if (fe::memoryManager::exists())
            {
                return fe::memoryManager::get().getDynamicAllocater().alloc(bytes);
            }

        return malloc(bytes);
    }

void operator delete(void *memory)
    {
        if (fe::memoryManager::exists())
            {
                fe::memoryManager::get().getDynamicAllocater().free(memory);
            }
        else
            {
                free(memory);
            }
    }
