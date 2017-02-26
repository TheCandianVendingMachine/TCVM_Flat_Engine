// memoryManager.hpp
// a sub-system that allocates a block of memory on startup to be used around the program
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineInclude.hpp"
#include "../systemModule.hpp"

namespace fe 
    {
        class FLAT_ENGINE_API memoryManager : public systemModule
            {
                private:
                    size_t m_bufferSize;
                    size_t m_currentOffset;

                    char *m_allocatedMemory;

                    static memoryManager *m_instance;

                public:
                    void startUp();
                    void shutDown();
                    static memoryManager &get();

                    void *alloc(size_t size);

            };
    }

#ifndef FE_ALLOC_UNSAFE
    #define FE_ALLOC_UNSAFE(size) fe::memoryManager::get().alloc(size)
#endif // !FE_ALLOC_UNSAFE