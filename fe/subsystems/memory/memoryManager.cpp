#include "memoryManager.hpp"
#include <cstdlib>
#include "../../feAssert.hpp"

fe::memoryManager *fe::memoryManager::m_instance = nullptr;

void fe::memoryManager::startUp(size_t bufferSize, size_t stackSize)
    {
        if (!m_instance)
            {
                m_instance = this;
                m_bufferSize = bufferSize;
                m_allocatedBuffer = static_cast<char*>(malloc(m_bufferSize));

                // start up the different types of memory allocaters
                m_stackAllocater.startUp(static_cast<char*>(alloc(stackSize)), stackSize);
            }

        m_shutDown = false;
    }

void fe::memoryManager::shutDown()
    {
        if (m_allocatedBuffer)
            {
                // shut down all types of memory allocation and free the pointers allocated inside them
                m_stackAllocater.clear();

                free(m_allocatedBuffer);
                m_allocatedBuffer = nullptr;

                m_shutDown = true;
            }
    }

fe::memoryManager &fe::memoryManager::get()
    {
        return *m_instance;
    }

void *fe::memoryManager::alloc(size_t size)
    {
        if (m_currentOffset + size <= m_bufferSize)
            {
                void *memReturn = m_allocatedBuffer + m_currentOffset;
                m_currentOffset += size;
                return memReturn;
            }

        FE_ASSERT(false, "Error: Memory Manager - Out of memory");
        return nullptr;
    }

fe::stackAllocater &fe::memoryManager::getStackAllocater()
    {
        return m_stackAllocater;
    }

char *fe::memoryManager::getBuffer() const
    {
        return m_allocatedBuffer;
    }

fe::memoryManager::~memoryManager()
    {
        FE_ASSERT(m_shutDown, "Error: Memory Manager not shut down. Possible memory leaks");
    }
