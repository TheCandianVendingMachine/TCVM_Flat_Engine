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
                if (allocBlock->m_size + sizeDifference >= size)
                    {
                        break;
                    }
                previousNode = allocBlock;
                allocBlock = allocBlock->m_next;
            }
        FE_ASSERT(allocBlock, "Dynamic Memory Allocater out of memory");
        if (previousNode) { previousNode->m_next = allocBlock->m_next; }

        // Allocate new free block node
        freeBlock *newBlock = nullptr;
        if (allocBlock->m_size - sizeOfFreeBlock > size) // We subtract the size of the free block so when we free the memory we can add back the free block without fear
            {
                newBlock = static_cast<freeBlock*>(static_cast<void*>(m_memoryBuffer + ((fe::uInt8*)allocBlock - m_memoryBuffer) + size + sizeOfFreeBlock));
                if (previousNode)
                    {
                        newBlock->m_next = previousNode->m_next;
                        previousNode->m_next = newBlock;
                    }
                newBlock->m_size = allocBlock->m_size - size - sizeOfFreeBlock;
            }
        
        if (previousNode)
            {
                previousNode->m_next = nullptr;
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

        return data;
    }

void fe::dynamicMemoryAllocater::free(void *memory)
    {
        fe::uInt64 memoryPos = (static_cast<fe::uInt8*>(memory) - m_memoryBuffer) - sizeof(memoryHeader); // index of the memory INCLUDING the header
        fe::uInt64 sizeOfMemory = static_cast<memoryHeader*>(static_cast<void*>(m_memoryBuffer + memoryPos))->m_size;

        std::memset(m_memoryBuffer + memoryPos, 0, sizeOfMemory + sizeof(memoryHeader));

        freeBlock *newBlock = static_cast<freeBlock*>(static_cast<void*>(m_memoryBuffer + memoryPos));
        newBlock->m_size = sizeOfMemory;

        freeBlock *blockList = m_freeBlocks;
        while (blockList)
            {
                if (blockList > newBlock)
                    {
                        newBlock->m_next = blockList;
                        break;
                    }
                blockList = blockList->m_next;
            }
        m_freeBlocks = newBlock;

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
