#include "dynamicMemoryAllocater.hpp"
#include "../../feAssert.hpp"
#include "../../debug/profiler.hpp"
#include <cstring>
#include <algorithm>
#include <cmath>

constexpr fe::uInt64 fe::dynamicMemoryAllocater::calculateAllocSize(const fe::uInt64 size)
    {
        fe::uInt64 modSize = size % CHUNK_SIZE;
        if (modSize <= 0)
            {
                return size;
            }
        return size + (CHUNK_SIZE % modSize);
    }

fe::dynamicMemoryAllocater::dynamicMemoryAllocater(const fe::uInt8 chunkSize) : m_memoryBuffer(nullptr), CHUNK_SIZE(chunkSize)
    {

    }

void fe::dynamicMemoryAllocater::startUp(fe::uInt8 *buffer, const fe::uInt64 size)
    {
        m_totalSize = size;
        m_memoryBuffer = buffer;
        clear();
    }

void *fe::dynamicMemoryAllocater::alloc(const fe::uInt64 size, const fe::uInt8 alignment)
    {
        fe::uInt64 trueSize = calculateAllocSize(size + alignment + FREE_BLOCK_SIZE + alignof(freeHeader));
        listNode *it = m_freeList.head();
        listNode *itPrev = nullptr;
        while (it)
            {
                if (it->m_data.m_blockSize >= trueSize)
                    {
                        break;
                    }
                itPrev = it;
                it = it->m_next;
            }
        FE_ASSERT(it, "Dynamic Memory Manager out of memory!");

        void *testMemLoc = static_cast<void*>(reinterpret_cast<fe::uInt8*>(it) + calculateAllocSize(size + alignment));
        if (*static_cast<fe::uInt64*>(testMemLoc) == 0)
            {
                listNode *next = static_cast<listNode*>(static_cast<void*>(reinterpret_cast<fe::uInt8*>(it) + trueSize));
                next->m_data.m_blockSize = it->m_data.m_blockSize - calculateAllocSize(size + alignment);
                m_freeList.insert(it, next);
            }
        m_freeList.remove(itPrev, it);

        void *memory = it;
        freeHeader *memHead = new(memory) freeHeader;
        memHead->m_blockSize = size + alignment;
        return static_cast<void*>(reinterpret_cast<fe::uInt8*>(memHead) + FREE_BLOCK_SIZE + alignof(freeHeader));
    }

void fe::dynamicMemoryAllocater::free(void *memory)
    {
        fe::uInt8 *header = reinterpret_cast<fe::uInt8*>(memory) - FREE_BLOCK_SIZE - alignof(freeHeader);
        const freeHeader *headerObj = reinterpret_cast<freeHeader*>(header);

        FE_ASSERT(headerObj->m_header == 0xDEAD, "Memory Header Wrong");
    }

void fe::dynamicMemoryAllocater::clear()
    {
        std::memset(m_memoryBuffer, 0, m_totalSize);
        m_freeList.clear(false);
        listNode *base = reinterpret_cast<listNode*>(m_memoryBuffer);
        base->m_data.m_blockSize = m_totalSize - FREE_BLOCK_SIZE;
        base->m_next = nullptr;
        m_freeList.insert(base);
    }
