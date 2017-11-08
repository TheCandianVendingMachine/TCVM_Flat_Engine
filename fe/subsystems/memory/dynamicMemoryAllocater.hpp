// dynamicMemoryAllocater.hpp
// Allows for dynamically allocated memory. Will be used in FE_NEW.cpp to allocate for the engine
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"

namespace fe
    {
        // Inspired by https://github.com/mtrebi/memory-allocators
        class dynamicMemoryAllocater
            {
                private:
                    struct memoryHeader
                        {
                            fe::uInt64 m_size = 0; // does NOT include size of header
                        };

                    struct freeBlock
                        {
                            freeBlock *m_next = nullptr;
                            fe::uInt64 m_size = 0; // does NOT include size of header
                        };
                    
                    fe::uInt64 m_totalSize;
                    freeBlock *m_freeBlocks;
                    fe::uInt8 *m_memoryBuffer;

                public:
                    FLAT_ENGINE_API dynamicMemoryAllocater();

                    FLAT_ENGINE_API void startUp(fe::uInt8 *buffer, fe::uInt64 size);

                    FLAT_ENGINE_API void *alloc(fe::uInt64 size);
                    FLAT_ENGINE_API void free(void *memory);

                    FLAT_ENGINE_API void clear();

                    // Verifies that the memory block list still is the correct size
                    FLAT_ENGINE_API bool verify() const;
                    FLAT_ENGINE_API bool verifyMemoryHeader(void *memory) const;

            };
    }