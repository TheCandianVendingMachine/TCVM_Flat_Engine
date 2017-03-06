// memoryManager.hpp
// a sub-system that allocates a block of memory on startup to be used around the program
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "stackAllocater.hpp"

#include "../../debug/memoryLogger.hpp"

namespace fe 
    {
        class logger;

        class memoryManager
            {
                private:
                    stackAllocater m_stackAllocater;
                    memoryLogger *m_memoryLogger;

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
                    // IF YOU CALL THIS YOU MUST LOG IT MANUALLY.
                    FLAT_ENGINE_API void *alloc(size_t size);

                    // not to be used
                    FLAT_ENGINE_API stackAllocater &getStackAllocater();

                    FLAT_ENGINE_API char *getBuffer() const;
                    
                    FLAT_ENGINE_API void logAllocation(const char *id, const size_t size, memoryLogger::allocationTypes type);
                    FLAT_ENGINE_API void logDeallocation(const char *id, const size_t size, memoryLogger::allocationTypes type);
                    FLAT_ENGINE_API memoryLogger &getMemoryLogger();

                    FLAT_ENGINE_API void printDebugInformation();

                    FLAT_ENGINE_API ~memoryManager();

            };
    }


// Allocates memory directly from the heap. Should not be used outside of special use cases
#define FE_ALLOC_DIRECT(id, size) \
    ([]()\
        { /* Since we cant define an inline function, we declare a lamda that does everything we need and then call it to return the value. */ \
          /* This is so we can log the allocation before we do the allocaiton, so if we get OOM errors we will know the last allocation */ \
            fe::memoryManager::get().logAllocation(id, size, fe::memoryLogger::ALLOC_DIRECT); \
            return fe::memoryManager::get().alloc(size); \
        })();

// Allocate memory from the stack. Use memory that needs to be allocated fast, and deallocated fast
#define FE_ALLOC_STACK(id, size)\
    ([]()\
        {\
            fe::memoryManager::get().logAllocation(id, size, fe::memoryLogger::ALLOC_STACK);\
            return fe::memoryManager::get().getStackAllocater().alloc(size);\
        })();

// Free memory from the bottom of the stack to the marker. Will not invalidate pointers, but will allow them to be overwritten
#define FE_FREE_STACK(id, marker)\
    fe::memoryManager::get().logDeallocation(id, 0, fe::memoryLogger::ALLOC_STACK);\
    fe::memoryManager::get().getStackAllocater().freeToMarker(marker);
