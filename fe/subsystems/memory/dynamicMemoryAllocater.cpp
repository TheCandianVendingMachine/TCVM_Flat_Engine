#include "dynamicMemoryAllocater.hpp"
#include "../../feAssert.hpp"
#include <cstring>

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
        void *data = nullptr;
        const fe::uInt64 sizeOfHeader =     sizeof(dynamicMemoryAllocater::memoryHeader);
        const fe::uInt64 sizeOfFreeBlock =  sizeof(dynamicMemoryAllocater::freeBlock);
        const fe::uInt64 sizeDifference = abs((fe::int64)(sizeOfFreeBlock - sizeOfHeader));

        freeBlock *previousNode = nullptr;
        freeBlock *allocBlock = m_freeBlocks;
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
        FE_ASSERT(allocBlock, "Dynamic Memory Allocater out of memory 1");
        FE_ASSERT(fe::int64(allocBlock->m_size) - fe::int64(sizeOfFreeBlock) > fe::int64(size), "Dynamic Memory Allocater underflow");
        FE_ASSERT(allocBlock->m_size <= m_totalSize, "Dynamic Memory Manager out of range");
        if (previousNode) { previousNode->m_next = allocBlock->m_next; }

        // Allocate new free block node
        freeBlock *newBlock = nullptr;
        newBlock = static_cast<freeBlock*>(static_cast<void*>(m_memoryBuffer + ((fe::uInt8*)allocBlock - m_memoryBuffer) + size + sizeOfFreeBlock));
        newBlock->m_next = allocBlock->m_next;
        if (previousNode)
            {
                previousNode->m_next = newBlock;
            }
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

        return data;
    }

void fe::dynamicMemoryAllocater::free(void *memory)
    {
        if ((fe::uInt8*)memory < (fe::uInt8*)m_memoryBuffer || (fe::uInt8*)memory >(fe::uInt8*)m_memoryBuffer + m_totalSize) return;

        fe::uInt64 memoryPos = (static_cast<fe::uInt8*>(memory) - m_memoryBuffer) - sizeof(memoryHeader); // index of the memory INCLUDING the header
        fe::uInt64 sizeOfMemory = static_cast<memoryHeader*>(static_cast<void*>(m_memoryBuffer + memoryPos))->m_size;

        std::memset(m_memoryBuffer + memoryPos, 0, sizeOfMemory + sizeof(memoryHeader));

        freeBlock *newBlock = static_cast<freeBlock*>(static_cast<void*>(m_memoryBuffer + memoryPos));
        newBlock->m_size = sizeOfMemory;

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
                while (blockList)
                    {
                        if (blockList > newBlock)
                            {
                                newBlock->m_next = blockList;
                                break;
                            }
                        blockList = blockList->m_next;
                    }
            }
        

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
    }

void fe::dynamicMemoryAllocater::clear()
    {
        std::memset(m_memoryBuffer, 0, m_totalSize);
        m_freeBlocks = static_cast<freeBlock*>(static_cast<void*>(m_memoryBuffer));
        (m_freeBlocks + 0)->m_size = m_totalSize;
    }