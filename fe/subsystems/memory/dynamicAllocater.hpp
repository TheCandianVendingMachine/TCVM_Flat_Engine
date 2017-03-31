// dynamicAllocater.hpp
// dynamically allocates memory. Similar to the pool allocater, but allows for allocation of any object of any size
#pragma once
#include <cstdint>

namespace fe
    {
        class dynamicAllocater
            {
                private:
                    size_t m_bufferSize;
                    uintptr_t *m_buffer;

                public:
                    void startUp(void *buffer, size_t bufferSize);

                    void *alloc(size_t bytes);
                    void free(void *memory);

                    void clear();

            };
    }