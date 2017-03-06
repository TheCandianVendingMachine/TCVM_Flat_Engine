#include "engine.hpp"
#include <iostream>

fe::engine::engine() : m_logger(nullptr)
    {}

void fe::engine::startUp(unsigned long long totalMemory, unsigned long long stackMemory)
    {
        m_memoryManager.startUp(totalMemory, stackMemory);


        m_logger = new logger;
        m_logger->startUp("log.log");
    }

void fe::engine::shutDown()
    {
        m_logger->shutDown();
        m_memoryManager.shutDown();
    }
