#include "memoryManager.hpp"
#include <cstdlib>

fe::memoryManager *fe::memoryManager::m_instance = nullptr;

void fe::memoryManager::startUp()
    {
        if (!m_instance)
            {
                m_instance = this;
                m_allocatedMemory = static_cast<char*>(malloc(sizeof(char) * 1024 * 1024));

                // start up the different types of memory allocation
            }
    }

void fe::memoryManager::shutDown()
    {
        if (m_allocatedMemory)
            {
                // shut down all types of memory allocation and free the pointers allocated inside them
                

                free(m_allocatedMemory);
                m_allocatedMemory = nullptr;

                m_shutDown = true;
            }
    }

fe::memoryManager &fe::memoryManager::get()
    {
        return *m_instance;
    }
