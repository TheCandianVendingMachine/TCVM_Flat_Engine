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

void fe::memoryLogger::allocation(const std::string &id, size_t size, allocationTypes type)
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

void fe::memoryLogger::deallocation(const std::string &id, size_t size, allocationTypes type)
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

std::string fe::memoryLogger::getLog()
    {
        std::string returnLog;

        for (auto &log : m_memoryLog)
            {
                returnLog += log.first + " --" + log.second.first + "-- " + std::to_string(log.second.second) + " Bytes\n";
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
