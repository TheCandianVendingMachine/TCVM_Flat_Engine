// dynamicMemoryAllocater.hpp
// Allows for dynamically allocated memory. Will be used in FE_NEW.cpp to allocate for the engine
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"
#include "../../dataStructures/singlyLinkedList.hpp"

namespace fe
    {
        class dynamicMemoryAllocater
            {
                private:
                    struct memoryHeader
                        {
                            fe::uInt64 m_blockSize = 0; // how much memory is present past the block
                            fe::uInt8 m_padding;
                        };

                    struct freeHeader
                        {
                            fe::uInt64 m_blockSize = 0; // how much memory is present after the block
                        };

                    using listNode = fe::singlyLinkedList<freeHeader>::node;

                    fe::singlyLinkedList<freeHeader> m_freeList;

                    static constexpr fe::uInt64 MEMORY_HEADER_SIZE = sizeof(memoryHeader);
                    static constexpr fe::uInt64 FREE_BLOCK_SIZE = sizeof(freeHeader);

                    const fe::uInt8 CHUNK_SIZE; // the size of how much memory is allocated in one pass. A 'chunk' is how much memory will be allocated from the buffer

                    fe::uInt64 m_totalSize;
                    fe::uInt8 *m_memoryBuffer;

                    FLAT_ENGINE_API constexpr fe::uInt64 calculateAllocSize(const fe::uInt64 size);

                public:
                    FLAT_ENGINE_API dynamicMemoryAllocater(const fe::uInt8 chunkSize = 32);

                    FLAT_ENGINE_API void startUp(fe::uInt8 *buffer, const fe::uInt64 size);

                    FLAT_ENGINE_API void *alloc(const fe::uInt64 size, const fe::uInt8 alignment);
                    FLAT_ENGINE_API void free(void *memory);

                    FLAT_ENGINE_API void clear();

            };
    }