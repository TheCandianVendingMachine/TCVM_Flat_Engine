#include "dynamicMemoryAllocater.hpp"
#include "../../feAssert.hpp"
#include "../../debug/profiler.hpp"
#include <cstring>
#include <algorithm>
#include <cmath>

constexpr fe::uInt64 fe::dynamicMemoryAllocater::calculateAllocSize(const fe::uInt64 size)
    {
        fe::uInt64 additionToSize = (fe::int64(CHUNK_SIZE) - size) % CHUNK_SIZE;
        fe::uInt64 retSize = additionToSize + size;
        return retSize;
    }

void fe::dynamicMemoryAllocater::condense()
    {
        listNode *it = m_freeList.head();
        while (it)
            {
                fe::uInt64 size = calculateAllocSize(FREE_BLOCK_SIZE + it->m_data.m_blockSize);
                if (static_cast<fe::uInt8*>(static_cast<void*>(it->m_next)) == static_cast<fe::uInt8*>(static_cast<void*>(it)) + size)
                    {
                        // Two blocks are touching, merge
                        it->m_data.m_blockSize += it->m_next->m_data.m_blockSize + FREE_BLOCK_SIZE;
                        it->m_data.m_blockSize = calculateAllocSize(it->m_data.m_blockSize);
                        it->m_next = it->m_next->m_next;
                    }
                else
                    {
                        it = it->m_next;
                    }
            }
    }

bool fe::dynamicMemoryAllocater::debug()
    {
        bool good = true;

        fe::uInt64 size = 0;
        listNode *it = m_freeList.head();
        listNode *itPrev = nullptr;
        while (it)
            {
                size += it->m_data.m_blockSize + FREE_BLOCK_SIZE;
                if (itPrev) 
                    {
                        size += static_cast<fe::uInt8*>(static_cast<void*>(it)) - (static_cast<fe::uInt8*>(static_cast<void*>(itPrev)) + FREE_BLOCK_SIZE + itPrev->m_data.m_blockSize);
                    }
                itPrev = it;
                it = it->m_next;
            }

        size += static_cast<fe::uInt8*>(static_cast<void*>(m_freeList.head())) - m_memoryBuffer;

        good = good && size == m_totalSize;

        good = good && !m_freeList.loop();
        return good;
    }

fe::dynamicMemoryAllocater::dynamicMemoryAllocater(const fe::uInt8 chunkSize) : m_memoryBuffer(nullptr), CHUNK_SIZE(chunkSize)
    {

    }

void fe::dynamicMemoryAllocater::startUp(fe::uInt8 *buffer, const fe::uInt64 size)
    {
        m_totalSize = calculateAllocSize(size);
        m_memoryBuffer = buffer;
        clear();
        FE_ASSERT(debug(), "Debug Check Failed");
    }

void *fe::dynamicMemoryAllocater::alloc(const fe::uInt64 size, const fe::uInt8 alignment)
    {
        FE_ASSERT(debug(), "Debug Check Failed");
        fe::uInt64 trueSize = calculateAllocSize(size + alignment + FREE_BLOCK_SIZE); // size of the wanted memory size + listNode rounded to the highest CHUNK_SIZE
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
        fe::uInt64 blockSize = it->m_data.m_blockSize;

        // Memory returned
        void *memoryLocation = static_cast<fe::uInt8*>(static_cast<void*>(it)) + FREE_BLOCK_SIZE;
        it->m_data.m_blockSize = trueSize - FREE_BLOCK_SIZE;

        // Generate header
        listNode *itNext = it->m_next;
        listNode *newNode = new(static_cast<fe::uInt8*>(static_cast<void*>(it)) + trueSize) listNode();
        m_freeList.replace(it, newNode);
        newNode->m_data.m_blockSize = blockSize - trueSize;

        FE_ASSERT(debug(), "Debug Check Failed");

#if _DEBUG
        std::memset(memoryLocation, 0, it->m_data.m_blockSize);
        FE_ASSERT(debug(), "Debug check failed!");
#endif

        return memoryLocation;
    }

void fe::dynamicMemoryAllocater::free(void *memory)
    {
        FE_ASSERT(debug(), "Debug Check Failed");
        fe::uInt8 *header = reinterpret_cast<fe::uInt8*>(memory) - FREE_BLOCK_SIZE;
        const freeHeader *headerObj = reinterpret_cast<freeHeader*>(header);

        FE_ASSERT(headerObj->m_header == 0xDEAD, "Memory header invalid");

        listNode *it = m_freeList.head();
        listNode *itPrev = nullptr;
        while (it)
            {
                if (static_cast<void*>(&it) > headerObj)
                    {
                        break;
                    }
                itPrev = it;
                it = it->m_next;
            }

        FE_ASSERT(debug(), "Debug Check Failed");
        fe::uInt64 blockSize = headerObj->m_blockSize;
        std::memset(header + FREE_BLOCK_SIZE, 0, blockSize);
        listNode *newNode = new(header) listNode();
        newNode->m_data.m_blockSize = blockSize;
        if (itPrev < newNode) 
            {
                m_freeList.insert(itPrev, newNode);
            }
        else
            {
                m_freeList.insert(newNode, itPrev);
            }

        FE_ASSERT(debug(), "Debug Check Failed");
        FE_ASSERT(newNode->m_data.m_header == 0xDEAD, "Memory header invalid");
        condense();
        FE_ASSERT(!m_freeList.loop(), "Dynamic Memory Allocater loop");

        FE_ASSERT(debug(), "Debug Check Failed");
    }

void fe::dynamicMemoryAllocater::clear()
    {
        std::memset(m_memoryBuffer, 0, m_totalSize);
        m_freeList.clear(false);
        listNode *base = new(m_memoryBuffer) listNode();
        base->m_data.m_blockSize = m_totalSize - FREE_BLOCK_SIZE;
        base->m_next = nullptr;
        m_freeList.insert(base);
    }
