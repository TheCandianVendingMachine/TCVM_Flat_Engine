#include "dynamicAllocater.hpp"
#include "memoryManager.hpp"
#include <iostream>

void fe::dynamicAllocater::startUp(void *buffer, size_t bufferSize)
    {
        auto *memManager = &memoryManager::get();
        if (memManager)
            {
                m_buffer = static_cast<uintptr_t*>(buffer);
                m_bufferSize = bufferSize;

                clear();
            }
    }

void *fe::dynamicAllocater::alloc(size_t bytes)
    {
        // since we assume the first few bytes of a memory address will be the distance
        // to the next free block of memory. That means if we take an address, and subtract
        // sizeof(uintptr_t), we will get the next memory address which is free.

        bool canAlloc = false;

        // the index of the buffer where the next address resides
        int index = 0;

        if (m_buffer[index] != 0)
            {
                int allocIndex = index;
                while (!canAlloc)
                    {
                        if (m_buffer[index + 1] == 0)
                            {
                                int nextAddrIndex = index + (uintptr_t*)m_buffer[index] - &m_buffer[index];
                                if ((nextAddrIndex - allocIndex) * sizeof(m_buffer[0]) >= bytes)
                                    {
                                        // If the next address minus the current has enough bytes to store the new allocation
                                        // we will use that instead
                                        canAlloc = true;
                                    }
                                else
                                    {
                                        if (m_buffer[nextAddrIndex] != 0)
                                            {
                                                // if the next block has and address, that means we need to repeat this process
                                                index += nextAddrIndex;
                                            }
                                        else
                                            {
                                                // the next block has not been allocated, so we can safely allocate at the index
                                                canAlloc = true;
                                            }
                                    }
                            }
                        else
                            {
                                index += (uintptr_t*)m_buffer[index] - &m_buffer[index];
                                allocIndex = index;
                            }
                    }
                index = allocIndex;
            }

        // Make the current index the address of the end of the memory allocated
        // "1" because we are adding an extra index since the current one would be 1 uintptr_t off
        // bytes / sizeof(m_buffer[0]) because that is how long the data we just allocated is. If we didn't have this here, we would be pointing to memory that may be in use
        m_buffer[index] = (uintptr_t)(m_buffer + index + 1 + sizeof(bytes / m_buffer[0]));

        index++;
        m_buffer[index] = 1; // allocate some random data so we can overwrite when needed, and when checking the data will contain something
        return m_buffer + index;
    }

void fe::dynamicAllocater::free(void *memory)
    {
        auto index = ((uintptr_t*)memory - m_buffer) - 1;
        auto buf = m_buffer + (((uintptr_t*)memory - m_buffer));

        // We set all of the values to zero up to the next memory address
        std::memset(buf, 0, sizeof(m_buffer[0]) * ((uintptr_t*)m_buffer[index] - &m_buffer[index] - 1));
    }

void fe::dynamicAllocater::clear()
    {
        std::memset(m_buffer, 0, m_bufferSize);
    }
