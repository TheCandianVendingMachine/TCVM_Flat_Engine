#include "dynamicMemoryAllocater.hpp"
#include "../../feAssert.hpp"
#include "../../debug/profiler.hpp"
#include <cstring>
#include <algorithm>
#include <cmath>

fe::dynamicMemoryAllocater::dynamicMemoryAllocater() : m_freeBlocks(nullptr), m_memoryBuffer(nullptr)
    {
    }

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
        FE_ASSERT(size > 0, "Allocating zero sized memory");
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
        FE_ASSERT(allocBlock->m_size > 0, "Memory Header Size == 0");
        // Allocate new free block node
        freeBlock *newBlock = nullptr;
        newBlock = static_cast<freeBlock*>(static_cast<void*>(reinterpret_cast<fe::uInt8*>(allocBlock) + size + sizeOfFreeBlock));
        if (allocBlock->m_next && reinterpret_cast<fe::uInt8*>(newBlock) + sizeOfFreeBlock > reinterpret_cast<fe::uInt8*>(allocBlock->m_next))
            {
                newBlock->m_next = allocBlock->m_next->m_next;
                allocBlock->m_next = newBlock;
            }
        else
            {
                newBlock->m_next = allocBlock->m_next;
            }
        newBlock->m_size = allocBlock->m_size - size - sizeOfFreeBlock;
        FE_ASSERT(verify(), "Size of dynamic memory allocater not total size");

        if (previousNode)
            {
                previousNode->m_next = newBlock;
            }
        else
            {
                m_freeBlocks = newBlock;
            }
        FE_ASSERT(verify(), "Size of dynamic memory allocater not total size");

        // Allocate data in memory buffer
        reinterpret_cast<memoryHeader*>(allocBlock)->m_size = size;
        data = static_cast<void*>(reinterpret_cast<fe::uInt8*>(allocBlock) + sizeOfHeader);

        std::memset(data, 0, size);

        FE_ASSERT(verify(), "Size of dynamic memory allocater not total size");
        FE_ASSERT(verifyMemoryHeader(data), "Memory Header Size == 0");
        return data;
    }

void fe::dynamicMemoryAllocater::free(void *memory)
    {
        if((fe::uInt8*)memory < (fe::uInt8*)m_memoryBuffer || (fe::uInt8*)memory >(fe::uInt8*)m_memoryBuffer + m_totalSize) return;
        FE_ASSERT(verify(), "Size of dynamic memory allocater not total size");
        FE_ASSERT(verifyMemoryHeader(memory), "Memory Header Size == 0");

        auto size = sizeof(memoryHeader);
        fe::uInt8 *memoryLoc = reinterpret_cast<fe::uInt8*>(memory) - sizeof(memoryHeader); // index of the memory INCLUDING the header
        auto mem = reinterpret_cast<memoryHeader*>(memoryLoc);
        fe::uInt64 sizeOfMemory = reinterpret_cast<memoryHeader*>(memoryLoc)->m_size;

        FE_ASSERT(sizeOfMemory > 0, "Size of memory has to be > 0");

        std::memset(memoryLoc, 0, sizeOfMemory + sizeof(memoryHeader));

        freeBlock *newBlock = reinterpret_cast<freeBlock*>(memoryLoc);
        static_assert(sizeof(freeBlock) >= sizeof(memoryHeader), "Freeblock size smaller than memory header - math error");
        newBlock->m_size = sizeOfMemory + sizeof(freeBlock) - sizeof(memoryHeader);
        newBlock->m_next = nullptr;

        FE_ASSERT(newBlock->m_size > 0, "Header Size == 0");

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

        FE_ASSERT(iter, "Iterator is nullptr");

        while (iter->m_next)
            {
                ++iterations;
                FE_ASSERT(static_cast<fe::int8*>((void*)iter->m_next) - static_cast<fe::int8*>((void*)iter) >= 0, "Size not Unsinged (Subtracted to negative)");
                size += static_cast<fe::uInt8*>((void*)iter->m_next) - static_cast<fe::uInt8*>((void*)iter);
                iter = iter->m_next;
            }

        size += iter->m_size;
        size += sizeof(freeBlock);

        int dif = m_totalSize - size;
        return size == m_totalSize;
    }

bool fe::dynamicMemoryAllocater::verifyMemoryHeader(void *memory) const
    {
        fe::uInt64 size = *static_cast<fe::uInt64*>(reinterpret_cast<void*>(reinterpret_cast<fe::uInt8*>(memory) - sizeof(fe::uInt64)));
        return size > 0;
    }
