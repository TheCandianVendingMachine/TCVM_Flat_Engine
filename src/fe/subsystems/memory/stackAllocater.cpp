#include "fe/subsystems/memory/stackAllocater.hpp"
#include "fe/feAssert.hpp"

void fe::stackAllocater::startUp(fe::uInt8 *buffer, fe::uInt64 bufferSize)
    {
        m_topOfBuffer = reinterpret_cast<Marker>((void*)buffer);

        m_memoryBuffer = buffer;
        m_bufferSize = bufferSize;

        m_offset = 0;
    }

void *fe::stackAllocater::alloc(fe::uInt64 size)
    {
        if (m_offset + size <= m_bufferSize)
            {
                void *retMem = m_memoryBuffer + m_offset;
                m_offset += size;
                return retMem;
            }

        FE_ASSERT(m_offset + size <= m_bufferSize, "Stack Allocator out of Memory");
        FE_LOG_ERROR("Stack Allocater Out of Memory");
        return nullptr;
    }

fe::stackAllocater::Marker fe::stackAllocater::getMarker()
    {
        return m_offset;
    }

void fe::stackAllocater::freeToMarker(Marker freeTo)
    {
        m_offset = freeTo;
        std::memset(m_memoryBuffer + m_offset, 0, m_bufferSize - m_offset);
    }

void fe::stackAllocater::clear()
    {
        freeToMarker(0);
    }
