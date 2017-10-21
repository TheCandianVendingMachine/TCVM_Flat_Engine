#include "dynamicMemoryAllocater.hpp"
#include "../../feAssert.hpp"
#include "../../debug/profiler.hpp"
#include <cstring>
#include <algorithm>
#include <cmath>

void fe::dynamicMemoryAllocater::startUp(fe::uInt8 *buffer, fe::uInt64 size)
    {
        m_totalSize = size;

        m_memoryBuffer = buffer;
        std::memset(m_memoryBuffer, 0, m_totalSize);
        m_freeBlocks = static_cast<freeBlock*>(static_cast<void*>(m_memoryBuffer));
        std::memset(m_freeBlocks, 0, size);

        (m_freeBlocks + 0)->m_size = size - sizeof(freeBlock);
    }

void *fe::dynamicMemoryAllocater::alloc(fe::uInt64 size)
    {
        FE_ASSERT(verify(), "Size of dynamic memory allocater not total size");
        void *data = nullptr;
        const fe::uInt64 sizeOfHeader =     sizeof(dynamicMemoryAllocater::memoryHeader);
        const fe::uInt64 sizeOfFreeBlock =  sizeof(dynamicMemoryAllocater::freeBlock);
        const fe::uInt64 sizeDifference = abs((fe::int64)(sizeOfFreeBlock - sizeOfHeader));

        freeBlock *previousNode = nullptr;
        freeBlock *allocBlock = m_freeBlocks;
        FE_ENGINE_PROFILE_NO_LOG("dynamicMemoryAllocater", "findFreeBlock");
        while (allocBlock)
            {
                fe::int64 a = (fe::int64)allocBlock->m_size - (fe::int64)sizeOfFreeBlock;
                if (a > fe::int64(size))
                    {
                        break;
                    }
                previousNode = allocBlock;
                allocBlock = allocBlock->m_next;
            }
        FE_END_PROFILE;
        FE_ASSERT(allocBlock, "Dynamic Memory Allocater out of memory 1");
        FE_ASSERT(fe::int64(allocBlock->m_size) - fe::int64(sizeOfFreeBlock) > fe::int64(size), "Dynamic Memory Allocater underflow");
        FE_ASSERT(allocBlock->m_size <= m_totalSize, "Dynamic Memory Manager out of range");
        FE_ASSERT(verify(), "Size of dynamic memory allocater not total size");
        // Allocate new free block node
        freeBlock *newBlock = nullptr;
        newBlock = static_cast<freeBlock*>(static_cast<void*>(m_memoryBuffer + ((fe::uInt8*)allocBlock - m_memoryBuffer) + size + sizeOfFreeBlock));
        newBlock->m_next = allocBlock->m_next;
        newBlock->m_size = allocBlock->m_size - size - sizeOfFreeBlock;

        if (previousNode)
            {
                previousNode->m_next = newBlock;
            }
        else
            {
                m_freeBlocks = newBlock;
            }

        // Allocate data in memory buffer
        fe::uInt64 allocBlockOffset = ((fe::uInt8*)allocBlock - m_memoryBuffer);
        memoryHeader *header = static_cast<memoryHeader*>(static_cast<void*>(m_memoryBuffer + allocBlockOffset));
        header->m_size = size;
        data = static_cast<void*>(m_memoryBuffer + allocBlockOffset + sizeOfHeader);

        std::memset(data, 0, size);

        FE_ASSERT(verify(), "Size of dynamic memory allocater not total size");
        return data;
    }

void fe::dynamicMemoryAllocater::free(void *memory)
    {
        if ((fe::uInt8*)memory < (fe::uInt8*)m_memoryBuffer || (fe::uInt8*)memory >(fe::uInt8*)m_memoryBuffer + m_totalSize) return;
        FE_ASSERT(verify(), "Size of dynamic memory allocater not total size");

        fe::uInt64 memoryPos = (static_cast<fe::uInt8*>(memory) - sizeof(memoryHeader)) - m_memoryBuffer; // index of the memory INCLUDING the header
        fe::uInt64 sizeOfMemory = static_cast<memoryHeader*>(static_cast<void*>(m_memoryBuffer + memoryPos))->m_size;

        std::memset(m_memoryBuffer + memoryPos, 0, sizeOfMemory + sizeof(memoryHeader));

        freeBlock *newBlock = static_cast<freeBlock*>(static_cast<void*>(m_memoryBuffer + memoryPos));
        static_assert(sizeof(freeBlock) >= sizeof(memoryHeader), "Freeblock size smaller than memory header - math error");
        newBlock->m_size = sizeOfMemory + sizeof(freeBlock) - sizeof(memoryHeader);
        newBlock->m_next = nullptr;

        freeBlock *blockList = m_freeBlocks;
        if (blockList > newBlock)
            {
                // If the first node is higher than the current freed node, we
                // need to set the first free block to the new block
                newBlock->m_next = blockList;
                m_freeBlocks = newBlock;
            }
        else
            {
                freeBlock *previous = nullptr;
                FE_ENGINE_PROFILE_NO_LOG("dynamicMemoryAllocater", "findSiblingBlock");
                while (blockList)
                    {
                        if (blockList > newBlock)
                            {
                                newBlock->m_next = blockList;
                                if (previous)
                                    {
                                        previous->m_next = newBlock;
                                    }
                                break;
                            }
                        previous = blockList;
                        blockList = blockList->m_next;
                    }
                FE_END_PROFILE;
            }

        FE_ASSERT(verify(), "Size of dynamic memory allocater not total size");

        FE_ENGINE_PROFILE_NO_LOG("dynamicMemoryAllocater", "mergeFreeBlocks");
        // Clean up any neighbor blocks
        blockList = m_freeBlocks;
        while (blockList)
            {
                if (!blockList->m_next) break;
                if (static_cast<fe::uInt8*>((void*)(blockList)) + blockList->m_size + sizeof(freeBlock) == static_cast<fe::uInt8*>((void*)(blockList->m_next)))
                    {
                        blockList->m_size += blockList->m_next->m_size + sizeof(freeBlock);
                        blockList->m_next = blockList->m_next->m_next;
                    }
                blockList = blockList->m_next;
            }
        FE_END_PROFILE;
        FE_ASSERT(verify(), "Size of dynamic memory allocater not total size");
    }

void fe::dynamicMemoryAllocater::clear()
    {
        std::memset(m_memoryBuffer, 0, m_totalSize);
        m_freeBlocks = static_cast<freeBlock*>(static_cast<void*>(m_memoryBuffer));
        (m_freeBlocks + 0)->m_size = m_totalSize;
    }

// Works by checking how much memory is in the free-blocks, and if there is a discrepancy between the reported size and actual size there is a problem
bool fe::dynamicMemoryAllocater::verify() const
    {
        fe::uInt64 size = 0;
        int iterations = 0;
        freeBlock *iter = m_freeBlocks;

        size += (static_cast<fe::uInt8*>((void*)m_freeBlocks) - m_memoryBuffer);

        while (iter->m_next)
            {
                ++iterations;
                size += static_cast<fe::uInt8*>((void*)iter->m_next) - static_cast<fe::uInt8*>((void*)iter);
                iter = iter->m_next;
            }

        size += iter->m_size;
        size += sizeof(freeBlock);

        int dif = m_totalSize - size;
        return size == m_totalSize;
    }
