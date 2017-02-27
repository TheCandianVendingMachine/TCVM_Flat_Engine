// stackAllocater.hpp
// Allows the user to allocate a specific amount of memory without having to allocaate each frame
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <cstdint>

namespace fe
    {
        class FLAT_ENGINE_API stackAllocater
            {
                public:
                    typedef uintptr_t Marker;

                private:
                    // the very top of the stack
                    uintptr_t m_topOfBuffer;
                    // the offset from the top of the allocated memory which will be allocated
                    Marker m_offset;

                    size_t m_bufferSize;
                    char *m_memoryBuffer;

                public:
                    void startUp(char *buffer, size_t bufferSize);

                    // allocates memory
                    void *alloc(size_t size);

                    // returns a marker (uint32) to allow the user to free to
                    Marker getMarker();
                    // frees the memory up to the marker from the bottom of the stack
                    void freeToMarker(Marker freeTo);

                    // clears the allocated memory to the top of the stack
                    void clear();

            };
    }