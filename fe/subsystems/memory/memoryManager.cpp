#include "memoryManager.hpp"
#include <cstdlib>

fe::memoryManager *fe::memoryManager::m_instance = nullptr;

void fe::memoryManager::startUp()
    {
        if (!m_instance)
            {
                m_instance = this;
                m_bufferSize = sizeof(char) * 1024 * 1024;
                m_allocatedMemory = static_cast<char*>(malloc(m_bufferSize));

                // start up the different types of memory allocation
                size_t stackSize = sizeof(char) * 512;
                m_stackAllocater.startUp(static_cast<char*>(alloc(stackSize)), stackSize);
            }
    }

void fe::memoryManager::shutDown()
    {
        if (m_allocatedMemory)
            {
                // shut down all types of memory allocation and free the pointers allocated inside them
                m_stackAllocater.clear();

                free(m_allocatedMemory);
                m_allocatedMemory = nullptr;

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
                void *memReturn = m_allocatedMemory + m_currentOffset;
                m_currentOffset += size;
                return memReturn;
            }

        FE_ASSERT(false, "Out of memory");
        return nullptr;
    }

fe::stackAllocater &fe::memoryManager::getStackAllocater()
    {
        return m_stackAllocater;
    }
