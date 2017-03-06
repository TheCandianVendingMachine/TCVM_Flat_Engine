#include "engine.hpp"

void fe::engine::startUp()
    {
        m_logger.startUp("log.log");
        m_memoryManager.startUp(sizeof(char) * 1024, 512);
    }

void fe::engine::shutDown()
    {
        m_memoryManager.shutDown();
        m_logger.shutDown();
    }
