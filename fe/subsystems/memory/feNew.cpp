#include "memoryManager.hpp"
#include <cstdlib>

void *operator new(std::size_t bytes)
    {
        if (fe::memoryManager::exists() && fe::memoryManager::get().getDynamicAllocater().startedUp())
            {
                return fe::memoryManager::get().getDynamicAllocater().alloc(bytes, 2);
            }

        return malloc(bytes);
    }

void operator delete(void *memory)
    {
        if (fe::memoryManager::exists() && fe::memoryManager::get().getDynamicAllocater().memoryInRegion(memory))
            {
                fe::memoryManager::get().getDynamicAllocater().free(memory);
            }
        else
            {
                free(memory);
            }
    }
