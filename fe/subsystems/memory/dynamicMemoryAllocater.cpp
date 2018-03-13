#include "dynamicMemoryAllocater.hpp"
#include "../../feAssert.hpp"
#include "../../debug/profiler.hpp"
#include "../../typeDefines.hpp"
#include <cstring>
#include <algorithm>
#include <cmath>

constexpr fe::uInt64 fe::dynamicMemoryAllocater::calculateAllocSize(const fe::uInt64 size)
    {
        fe::uInt64 additionToSize = (fe::int64(CHUNK_SIZE) - size) % CHUNK_SIZE;
        fe::uInt64 retSize = additionToSize + size;
        return retSize;
    }

bool fe::dynamicMemoryAllocater::debug()
    {
    #if not FE_DEBUG_ALLOCATER
        return true;
    #endif
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

fe::dynamicMemoryAllocater::dynamicMemoryAllocater(const fe::uInt8 chunkSize) : m_memoryBuffer(nullptr), CHUNK_SIZE(chunkSize), m_startedUp(false)
    {

    }

void fe::dynamicMemoryAllocater::startUp(fe::uInt8 *buffer, const fe::uInt64 size)
    {
        m_totalSize = size;
        m_memoryBuffer = buffer;
        clear();
        FE_ASSERT(debug(), "Debug Check Failed");
        m_startedUp = true;
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
        FE_ASSERT(it->m_data.m_header == 0xDEAD, "Memory header invalid");
        fe::uInt64 blockSize = it->m_data.m_blockSize;

        // Memory returned
        void *memoryLocation = static_cast<fe::uInt8*>(static_cast<void*>(it)) + FREE_BLOCK_SIZE;
        it->m_data.m_blockSize = trueSize - FREE_BLOCK_SIZE;
        it->m_data.m_header = 0xBEEF;

        // Generate header
        listNode *itNext = it->m_next;
        listNode *newNode = new(static_cast<fe::uInt8*>(static_cast<void*>(it)) + trueSize) listNode();
        newNode->m_data.m_header = 0xDEAD;
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
        if (!memoryInRegion(memory))
            {
                return;
            }
        FE_ASSERT(debug(), "Debug Check Failed");
        fe::uInt8 *header = reinterpret_cast<fe::uInt8*>(memory) - FREE_BLOCK_SIZE;
        freeHeader *headerObj = reinterpret_cast<freeHeader*>(header);

        FE_ASSERT(headerObj->m_header == 0xBEEF, "Memory header invalid");

        listNode *it = m_freeList.head();
        listNode *itPrev = nullptr;
        while (it)
            {
                if (static_cast<void*>(it) > static_cast<void*>(headerObj))
                    {
                        break;
                    }
                itPrev = it;
                it = it->m_next;
            }

        FE_ASSERT(debug(), "Debug Check Failed");
        fe::uInt64 blockSize = headerObj->m_blockSize;
        std::memset(header, 0, blockSize + FREE_BLOCK_SIZE);
        // Condense memory
        if (itPrev && static_cast<void*>(itPrev + itPrev->m_data.m_blockSize + FREE_BLOCK_SIZE) == static_cast<void*>(header))
            {
                itPrev->m_data.m_blockSize += blockSize + FREE_BLOCK_SIZE;
            }
        else
            {
                listNode *newNode = new(header) listNode();
                newNode->m_data.m_blockSize = blockSize;
                newNode->m_data.m_header = 0xDEAD;
                m_freeList.insert(itPrev, newNode);
                FE_ASSERT(newNode->m_data.m_header == 0xDEAD, "Memory header invalid");

            #if _DEBUG
                void *a = static_cast<fe::uInt8*>(static_cast<void*>(newNode)) + newNode->m_data.m_blockSize + FREE_BLOCK_SIZE;
                void *b = newNode->m_next;

                bool equal = a == b;

                if (a == b)
            #else
                if ((static_cast<fe::uInt8*>(static_cast<void*>(newNode)) + newNode->m_data.m_blockSize + FREE_BLOCK_SIZE) == newNode->m_next)
            #endif
                    {
                        newNode->m_data.m_blockSize += FREE_BLOCK_SIZE + newNode->m_next->m_data.m_blockSize;
                        newNode->m_next = newNode->m_next->m_next;
                        std::memset(static_cast<fe::uInt8*>(static_cast<void*>(newNode)) + FREE_BLOCK_SIZE, 0, newNode->m_data.m_blockSize);
                    }
            }

        FE_ASSERT(debug(), "Debug Check Failed");
        FE_ASSERT(!m_freeList.loop(), "Dynamic Memory Allocater loop");
        
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

bool fe::dynamicMemoryAllocater::startedUp() const
    {
        return m_startedUp;
    }

bool fe::dynamicMemoryAllocater::memoryInRegion(void *memory) const
    {
        return memory >= m_memoryBuffer && memory < m_memoryBuffer + m_totalSize;
    }
