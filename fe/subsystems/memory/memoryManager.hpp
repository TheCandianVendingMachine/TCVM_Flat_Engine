// memoryManager.hpp
// a sub-system that allocates a block of memory on startup to be used around the program
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "stackAllocater.hpp"

namespace fe 
    {
        class memoryManager
            {
                private:
                    stackAllocater m_stackAllocater;

                    size_t m_bufferSize;
                    size_t m_currentOffset;

                    char *m_allocatedBuffer;
                    static memoryManager *m_instance;

                    bool m_shutDown;

                public:
                    FLAT_ENGINE_API void startUp(size_t bufferSize, size_t stackSize);
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API static memoryManager &get();

                    // allocate memory from the "heap" of specific size
                    FLAT_ENGINE_API void *alloc(size_t size);

                    // not to be used
                    FLAT_ENGINE_API stackAllocater &getStackAllocater();

                    FLAT_ENGINE_API char *getBuffer() const;

                    FLAT_ENGINE_API ~memoryManager();

            };
    }

#ifndef FE_ALLOC_DIRECT
    // Allocates memory directly from the heap. Should not be used outside of special use cases
    #define FE_ALLOC_DIRECT(size) fe::memoryManager::get().alloc(size)
#endif // !FE_ALLOC_DIRECT


#ifndef FE_ALLOC_STACK
    // Allocate memory from the stack. Use memory that needs to be allocated fast, and deallocated fast
    #define FE_ALLOC_STACK(size) fe::memoryManager::get().getStackAllocater().alloc(size)
#endif // !FE_ALLOC_STACK

#ifndef FE_FREE_STACK
    // Free memory from the bottom of the stack to the marker. Will not invalidate pointers, but will allow them to be overwritten
    #define FE_FREE_STACK(marker) fe::memoryManager::get().getStackAllocater().freeToMarker(marker)
#endif // !FE_FREE_STACK
