// memoryManager.hpp
// a sub-system that allocates a block of memory on startup to be used around the program
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineInclude.hpp"
#include "../systemModule.hpp"

#include "stackAllocater.hpp"

namespace fe 
    {
        class FLAT_ENGINE_API memoryManager : public systemModule
            {
                private:
                    stackAllocater m_stackAllocater;

                    size_t m_bufferSize;
                    size_t m_currentOffset;

                    char *m_allocatedMemory;

                    static memoryManager *m_instance;

                public:
                    void startUp();
                    void shutDown();
                    static memoryManager &get();

                    // allocate memory from the "heap" of specifiec size
                    void *alloc(size_t size);

                    // not to be used
                    stackAllocater &getStackAllocater();

            };
    }

#ifndef FE_ALLOC_UNSAFE
    #define FE_ALLOC_UNSAFE(size) fe::memoryManager::get().alloc(size)
#endif // !FE_ALLOC_UNSAFE

#ifndef FE_ALLOC_STACK
    #define FE_ALLOC_STACK(size) fe::memoryManager::get().getStackAllocater().alloc(size)
#endif // !FE_ALLOC_STACK

#ifndef FE_FREE_STACK
    #define FE_FREE_STACK(marker) fe::memoryManager::get().getStackAllocater().freeToMarker(marker)
#endif // !FE_FREE_STACK
