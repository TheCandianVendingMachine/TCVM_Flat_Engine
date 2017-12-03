// memoryManager.hpp
// a sub-system that allocates a block of memory on startup to be used around the program
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"
#include "stackAllocater.hpp"
#include "dynamicMemoryAllocater.hpp"

// User defined literals for easy conversion between bytes to other memory units

// Return the inputted bytes to Gigabytes (Really Gibibytes)
constexpr unsigned long long operator "" _GiB(unsigned long long bytes)
    {
        return bytes * 1073741824; // return the bytes * 1 Gibibyte (Gigabyte but the official name for it)
    }

// Return the inputted bytes to Megabytes (Really Mibibytes)
constexpr unsigned long long operator "" _MiB(unsigned long long bytes)
    {
        return bytes * 1048576;
    }

// Return the inputted bytes to Kilobytes (Really Kibibytes)
constexpr unsigned long long operator "" _KiB(unsigned long long bytes)
    {
        return bytes * 1024;
    }

namespace fe 
    {
        class logger;
        class memoryManager
            {
                private:
                    stackAllocater m_stackAllocater;
                    dynamicMemoryAllocater m_dynamicAllocater;

                    fe::uInt64 m_bufferSize;
                    fe::uInt64 m_currentOffset;

                    fe::uInt8 *m_allocatedBuffer;
                    static memoryManager *m_instance;

                    bool m_shutDown;

                public:
                    FLAT_ENGINE_API void startUp(fe::uInt64 bufferSize, fe::uInt64 stackSize, fe::uInt64 dynamicSize);
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API static memoryManager &get();

                    // Returns true if an instance exists
                    FLAT_ENGINE_API static bool exists();

                    // allocate memory from the "heap" of specific size
                    // IF YOU CALL THIS YOU MUST LOG IT MANUALLY.
                    FLAT_ENGINE_API void *alloc(fe::uInt64 size);

                    // not to be used
                    FLAT_ENGINE_API stackAllocater &getStackAllocater();
                    FLAT_ENGINE_API dynamicMemoryAllocater &getDynamicAllocater();

                    FLAT_ENGINE_API fe::uInt8 *getBuffer() const;

                    FLAT_ENGINE_API ~memoryManager();

            };
    }


// Allocates memory directly from the heap. Should not be used outside of special use cases
#define FE_ALLOC_DIRECT(id, size) \
    ([]()\
        { /* Since we cant define an inline function, we declare a lamda that does everything we need and then call it to return the value. */ \
          /* This is so we can log the allocation before we do the allocaiton, so if we get OOM errors we will know the last allocation */ \
            return fe::memoryManager::get().alloc(size); \
        })();

// Allocate memory from the stack. Use memory that needs to be allocated fast, and deallocated fast
#define FE_ALLOC_STACK(id, size)\
    ([]()\
        {\
            return fe::memoryManager::get().getStackAllocater().alloc(size);\
        })();

// Allocates memory dynamically from the memory pool. Allocates slower, but can be put back into the memory pool once its ocmpleted
#define FE_ALLOC_DYNAMIC(id, size)\
    ([]()\
        {\
            return fe::memoryManager::get().getDynamicAllocater().alloc(size);\
        })();\

// Free memory from the bottom of the stack to the marker. Will not invalidate pointers, but will allow them to be overwritten
#define FE_FREE_STACK(id, marker)\
    fe::memoryManager::get().getStackAllocater().freeToMarker(marker);

// Free memory back into the dynamic pool
#define FE_FREE_DYNAMIC(id, memory)\
    fe::memoryManager::get().getDynamicAllocater().free(memory);

// Allocates memory directly from the heap. Should not be used outside of special use cases. Use this when you define a size outside of the call
#define FE_ALLOC_DIRECT_CAPTURED(id, size) \
    ([&size]()\
        { /* Since we cant define an inline function, we declare a lamda that does everything we need and then call it to return the value. */ \
          /* This is so we can log the allocation before we do the allocaiton, so if we get OOM errors we will know the last allocation */ \
            return fe::memoryManager::get().alloc(size); \
        })();

// Allocate memory from the stack. Use memory that needs to be allocated fast, and deallocated fast. Use this when you define a size outside of the call
#define FE_ALLOC_STACK_CAPTURED(id, size)\
    ([&size]()\
        {\
            return fe::memoryManager::get().getStackAllocater().alloc(size);\
        })();

// Allocates memory dynamically from the memory pool. Allocates slower, but can be put back into the memory pool once its ocmpleted. Use this when you define a size outside of the call
#define FE_ALLOC_DYNAMIC_CAPTURED(id, size)\
    ([&size]()\
        {\
            return fe::memoryManager::get().getDynamicAllocater().alloc(size);\
        })();