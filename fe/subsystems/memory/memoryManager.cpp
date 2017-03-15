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
                m_memoryLogger = new(alloc(sizeof(memoryLogger))) memoryLogger;
            }

        m_shutDown = false;
        m_memoryLogger->startUp(bufferSize, stackSize);
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
        if (!m_instance)
            {
                FE_LOG_WARNING("Memory manager not started. Returning nullptr");
            }

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

        printDebugInformation();
        FE_ASSERT(false, "Memory Manager - Out of memory");
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

void fe::memoryManager::logAllocation(const char *id, const size_t size, memoryLogger::allocationTypes type)
    {
        m_memoryLogger->allocation(id, size, type);
    }

void fe::memoryManager::logDeallocation(const char *id, const size_t size, memoryLogger::allocationTypes type)
    {
        m_memoryLogger->deallocation(id, size, type);
    }

fe::memoryLogger &fe::memoryManager::getMemoryLogger()
    {
        return *m_memoryLogger;
    }

void fe::memoryManager::printDebugInformation()
    {
        FE_LOG( "Memory Debug Data\n",
                "Memory Allocation Log: ",      m_memoryLogger->getLog(),

                "\nMemory Allocated: ",         m_memoryLogger->getMemoryAllocated(),
                "\nMemory Left: ",              m_memoryLogger->getMemoryLeft(),
                "\nTotal Memory: ",             m_memoryLogger->getMemory(), "\n"

                "\nMemory Allocated Stack: ",   m_memoryLogger->getMemoryAllocatedStack(),
                "\nMemory Left Stack: ",        m_memoryLogger->getMemoryLeftStack(),
                "\nTotal Stack Memory: ",       m_memoryLogger->getMemoryStack(), "\n"

                "\nMemory Allocated Direct: ",  m_memoryLogger->getMemoryAllocatedDirect(),
                "\nMemory Left Direct: ",       m_memoryLogger->getMemoryLeftDirect(),
                "\nTotal Memory Direct: ",      m_memoryLogger->getMemoryDirect(), "\n",
                "\nMemory Allocated Via New: ", m_memoryLogger->memoryAllocatedOverloadedNew(), "\n");
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
