// memoryLogger.hpp
// logs all of the memory being used by the program
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include <unordered_map>
#include <utility>
#include <string>

namespace fe
    {
        class memoryLogger
            {
                public:
                    enum allocationTypes
                        {
                            ALLOC_STACK,
                            ALLOC_DIRECT
                        };

                private:
                    std::unordered_map<const char*, std::pair<const char*, size_t>> m_memoryLog;

                    size_t m_memoryAllocatedNew;

                    size_t m_memoryAllocatedStack;
                    size_t m_memoryAllocatedDirect;
                    size_t m_totalMemoryAllocated;

                    size_t m_stackMemory;
                    size_t m_directMemory;
                    size_t m_totalMemory;
                    
                public:
                    FLAT_ENGINE_API memoryLogger();
                    FLAT_ENGINE_API void startUp(const size_t totalMemory, const size_t stackMemory);

                    FLAT_ENGINE_API void allocation(const char *id, size_t size, allocationTypes type);
                    FLAT_ENGINE_API void deallocation(const char *id, size_t size, allocationTypes type);

                    FLAT_ENGINE_API const char* getLog();

                    FLAT_ENGINE_API const size_t getMemoryAllocatedStack() const;
                    FLAT_ENGINE_API const size_t getMemoryLeftStack() const;
                    FLAT_ENGINE_API const size_t getMemoryStack() const;

                    FLAT_ENGINE_API const size_t getMemoryAllocatedDirect() const;
                    FLAT_ENGINE_API const size_t getMemoryLeftDirect() const;
                    FLAT_ENGINE_API const size_t getMemoryDirect() const;

                    FLAT_ENGINE_API const size_t getMemoryAllocated() const;
                    FLAT_ENGINE_API const size_t getMemoryLeft() const;
                    FLAT_ENGINE_API const size_t getMemory() const;

                    FLAT_ENGINE_API const size_t memoryAllocatedOverloadedNew() const;

                    // DO NOT CALL EVER
                    // Used to log the amount of memory the memory log is taking up
                    FLAT_ENGINE_API void allocateOverloadedNew(size_t size);

            };
    }