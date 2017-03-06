#include "engine.hpp"
#include <iostream>

fe::engine::engine() : m_logger(nullptr)
    {}

void fe::engine::startUp()
    {
        m_memoryManager.startUp(sizeof(char) * 1024, 512);


        m_logger = new logger;
        m_logger->startUp("log.log");
    }

void fe::engine::shutDown()
    {
        m_logger->shutDown();
        m_memoryManager.shutDown();
    }
