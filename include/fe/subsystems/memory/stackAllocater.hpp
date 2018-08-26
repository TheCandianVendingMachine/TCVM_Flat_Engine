// stackAllocater.hpp
// Allows the user to allocate a specific amount of memory without having to allocaate each frame
#pragma once
#include "../../flatEngineExport.hpp"
#include "../../typeDefines.hpp"

namespace fe
    {
        class stackAllocater
            {
                public:
                    typedef fe::uIntPtr Marker;

                private:
                    // the very top of the stack
                    Marker m_topOfBuffer;
                    // the offset from the top of the allocated memory which will be allocated
                    Marker m_offset;

                    fe::uInt64 m_bufferSize;
                    fe::uInt8 *m_memoryBuffer;

                public:
                    FLAT_ENGINE_API void startUp(fe::uInt8 *buffer, fe::uInt64 bufferSize);

                    // allocates memory
                    FLAT_ENGINE_API void *alloc(fe::uInt64 size);

                    // returns a marker (uint32) to allow the user to free to
                    FLAT_ENGINE_API Marker getMarker();
                    // frees the memory up to the marker from the bottom of the stack
                    FLAT_ENGINE_API void freeToMarker(Marker freeTo);

                    // clears the allocated memory to the top of the stack
                    FLAT_ENGINE_API void clear();

            };
    }