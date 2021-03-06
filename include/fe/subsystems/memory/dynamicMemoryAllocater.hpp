// dynamicMemoryAllocater.hpp
// Allows for dynamically allocated memory. Will be used in FE_NEW.cpp to allocate for the engine
#pragma once
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"
#include "../../dataStructures/singlyLinkedList.hpp"

namespace fe
    {
        class dynamicMemoryAllocater
            {
                private:
                    constexpr static fe::uInt16 m_VALID_FREE_HEADER = 0xDEAD;
                    constexpr static fe::uInt16 m_VALID_USED_HEADER = 0xBEEF;

                    struct freeHeader
                        {
                            fe::uInt64 m_blockSize = 0; // how much memory is present after the block
                            fe::uInt16 m_header = m_VALID_FREE_HEADER; // Header to ensure data is proper. 0xDEAD == free header | 0xBEEF == used header
                        };

                    using listNode = fe::singlyLinkedList<freeHeader>::node;

                    fe::singlyLinkedList<freeHeader> m_freeList;

                    static constexpr fe::uInt64 FREE_BLOCK_SIZE = sizeof(listNode);

                    const fe::uInt8 CHUNK_SIZE; // the size of how much memory is allocated in one pass. A 'chunk' is how much memory will be allocated from the buffer

                    fe::uInt64 m_totalSize;
                    fe::uInt8 *m_memoryBuffer;

                    bool m_startedUp;

                    FLAT_ENGINE_API constexpr fe::uInt64 calculateAllocSize(const fe::uInt64 size);

                    // ensures that the free list is non-looping and full size
                    FLAT_ENGINE_API bool debug();

                public:
                    FLAT_ENGINE_API dynamicMemoryAllocater(const fe::uInt8 chunkSize = 32);

                    FLAT_ENGINE_API void startUp(fe::uInt8 *buffer, const fe::uInt64 size);

                    FLAT_ENGINE_API void *alloc(const fe::uInt64 size, const fe::uInt8 alignment);
                    FLAT_ENGINE_API void free(void *memory);

                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API bool startedUp() const;

                    // Returns whether or not memory is withing allocation range
                    FLAT_ENGINE_API bool memoryInRegion(void *memory) const;

            };
    }