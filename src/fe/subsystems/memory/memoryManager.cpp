#include "fe/subsystems/memory/memoryManager.hpp"
#include <cstdlib>
#include "fe/feAssert.hpp"
#include "fe/debug/logger.hpp"

fe::memoryManager *fe::memoryManager::m_instance = nullptr;

void fe::memoryManager::startUp(fe::uInt64 bufferSize)
    {
        if (!m_instance)
            {
                m_instance = this;
                m_bufferSize = bufferSize;
                m_allocatedBuffer = static_cast<fe::uInt8*>(std::malloc(m_bufferSize));
            }

        m_shutDown = false;
    }

void fe::memoryManager::startUpStack(fe::uInt64 stackSize)
    {
        m_stackAllocater.startUp(static_cast<fe::uInt8*>(alloc(stackSize)), stackSize);
    }

void fe::memoryManager::startUpDynamic(fe::uInt64 dynamicSize)
    {
        //m_dynamicAllocater.startUp(static_cast<fe::uInt8*>(alloc(dynamicSize)), dynamicSize);
    }

void fe::memoryManager::shutDown()
    {
        if (m_instance && m_allocatedBuffer)
            {
                //std::free(m_allocatedBuffer); // not freeing memory to allow the OS to do so.
                m_allocatedBuffer = nullptr;

                m_shutDown = true;
                m_instance = nullptr;
            }
    }

fe::memoryManager &fe::memoryManager::get()
    {
        if (!m_instance)
            {
                FE_LOG_WARNING("Memory manager not started. Returning nullptr");
            }

        return *m_instance;
    }

bool fe::memoryManager::exists()
    {
        return m_instance != nullptr;
    }

void *fe::memoryManager::alloc(fe::uInt64 size)
    {
        if (m_currentOffset + size <= m_bufferSize)
            {
                void *memReturn = m_allocatedBuffer + m_currentOffset;
                m_currentOffset += size;
                return memReturn;
            }

        //printDebugInformation();
        FE_ASSERT(false, "Memory Manager - Out of memory");
        return nullptr;
    }

fe::stackAllocater &fe::memoryManager::getStackAllocater()
    {
        return m_stackAllocater;
    }

fe::dynamicMemoryAllocater &fe::memoryManager::getDynamicAllocater()
    {
        return m_dynamicAllocater;
    }

fe::uInt8 *fe::memoryManager::getBuffer() const
    {
        return m_allocatedBuffer;
    }

fe::memoryManager::~memoryManager()
    {
        if (!m_shutDown && this == m_instance)
            {
                shutDown();
                m_shutDown = false;
            }
        FE_ASSERT(m_shutDown, "Memory Manager not shut down manually.");
    }
