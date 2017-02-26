#include "stackAllocater.hpp"
#include <iostream>

void fe::stackAllocater::startUp(char *buffer, size_t bufferSize)
    {
        m_topOfBuffer = reinterpret_cast<Marker>((void*)buffer);

        m_memoryBuffer = buffer;
        m_bufferSize = bufferSize;

        m_offset = 0;
    }

void *fe::stackAllocater::alloc(size_t size)
    {
        if (m_offset + size <= m_bufferSize)
            {
                void *retMem = m_memoryBuffer + m_offset;
                m_offset += size;
                return retMem;
            }

        FE_ASSERT(false, "Error: Stack Allocater Out of Memory");
        return nullptr;
    }

fe::stackAllocater::Marker fe::stackAllocater::getMarker()
    {
        return m_offset;
    }

void fe::stackAllocater::freeToMarker(Marker freeTo)
    {
        m_offset = freeTo;
    }

void fe::stackAllocater::clear()
    {
        freeToMarker(0);
    }
