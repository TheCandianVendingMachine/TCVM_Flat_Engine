#include "memoryLogger.hpp"

fe::memoryLogger::memoryLogger() :
    m_totalMemory(0),
    m_stackMemory(0),
    m_directMemory(0),
    m_memoryAllocatedStack(0),
    m_memoryAllocatedDirect(0),
    m_totalMemoryAllocated(0),
    m_memoryAllocatedNew(0)
    {
    }

void fe::memoryLogger::startUp(const size_t totalMemory, const size_t stackMemory)
    {
        m_totalMemory = totalMemory;
        m_stackMemory = stackMemory;
        m_directMemory = m_totalMemory - m_stackMemory;
    }

void fe::memoryLogger::allocation(const char *id, size_t size, allocationTypes type)
    {
        switch (type)
            {
                case fe::memoryLogger::ALLOC_STACK:
                    m_memoryLog[id] = std::make_pair("Allocated Stack", m_memoryLog[id].second + size);
                    m_memoryAllocatedStack += size;
                    break;
                case fe::memoryLogger::ALLOC_DIRECT:
                    m_memoryLog[id] = std::make_pair("Allocated Direct", m_memoryLog[id].second + size);
                    m_memoryAllocatedDirect += size;
                    break;
                default:
                    break;
            }

        m_totalMemoryAllocated += size;
    }

void fe::memoryLogger::deallocation(const char *id, size_t size, allocationTypes type)
    {
        switch (type)
            {
                case fe::memoryLogger::ALLOC_STACK:
                    m_memoryLog[id] = std::make_pair("Deallocated Stack", m_memoryLog[id].second - size);
                    m_memoryAllocatedStack -= size;
                    break;
                case fe::memoryLogger::ALLOC_DIRECT:
                    m_memoryLog[id] = std::make_pair("Deallocated Direct", m_memoryLog[id].second - size);
                    m_memoryAllocatedDirect -= size;
                    break;
                default:
                    break;
            }

        m_totalMemoryAllocated -= size;
    }

const char *fe::memoryLogger::getLog()
    {
        const size_t MAX_LOG_SIZE = 2048;

        char returnLog[MAX_LOG_SIZE];
        int end = 0;

        for (auto &log : m_memoryLog)
            {
                std::strncpy(returnLog, log.first, sizeof(log.first));
                std::strncpy(returnLog, " --", 4u);
                std::strncpy(returnLog, log.second.first, sizeof(log.second.first));
                std::strncpy(returnLog, "-- ", 4u);

                char buf[128];
                std::strncpy(returnLog, itoa(log.second.second, buf, sizeof(log.second.second)), sizeof(log.second.second)); // itoa is non-portable, but it will work on MSVC.
                std::strncpy(returnLog, " Bytes\n", 8u);
            }

        return returnLog;
    }

const size_t fe::memoryLogger::getMemoryAllocatedStack() const
    {
        return m_memoryAllocatedStack + m_memoryAllocatedNew;;
    }

const size_t fe::memoryLogger::getMemoryLeftStack() const
    {
        return m_stackMemory - m_memoryAllocatedStack - m_memoryAllocatedNew;
    }

const size_t fe::memoryLogger::getMemoryStack() const
    {
        return m_stackMemory;
    }

const size_t fe::memoryLogger::getMemoryAllocatedDirect() const
    {
        return m_memoryAllocatedDirect;
    }

const size_t fe::memoryLogger::getMemoryLeftDirect() const
    {
        return m_directMemory - m_memoryAllocatedDirect;
    }

const size_t fe::memoryLogger::getMemoryDirect() const
    {
        return m_directMemory;
    }

const size_t fe::memoryLogger::getMemoryAllocated() const
    {
        return m_totalMemoryAllocated + m_memoryAllocatedNew;
    }

const size_t fe::memoryLogger::getMemoryLeft() const
    {
        return m_totalMemory - m_totalMemoryAllocated - m_memoryAllocatedNew;
    }

const size_t fe::memoryLogger::getMemory() const
    {
        return m_totalMemory;
    }

const size_t fe::memoryLogger::memoryAllocatedOverloadedNew() const
    {
        return m_memoryAllocatedNew;
    }

void fe::memoryLogger::allocateOverloadedNew(size_t size)
    {
        if (this) 
            {
                m_memoryAllocatedNew += size;
            }
    }
