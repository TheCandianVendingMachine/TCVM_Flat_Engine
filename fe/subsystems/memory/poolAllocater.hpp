// poolAllocater.hpp
// A memory allocater that will allow the user to define an object in which to pool memory blocks for
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineInclude.hpp"
#include "memoryManager.hpp"

namespace fe
    {
        template<typename T>
        class FLAT_ENGINE_API poolAllocater
            {
                private:
                    size_t m_bufferSize;

                    unsigned int m_objectCount;

                    bool *m_freeIndicies;
                    char *m_buffer;

                public:
                    void startUp(const unsigned int objectCount, char *buffer);

                    // allocates a block of memory for the wanted object
                    T *alloc();

                    // Frees the object at the address it is allocated to
                    void free(T *address);

                    // clears the allocated memory to allow for a new 
                    void clear();

            };
        template<typename T>
        void poolAllocater<T>::startUp(const unsigned int objectCount, char *buffer)
            {
                m_freeIndicies = static_cast<bool*>(memoryManager::get().alloc(objectCount * sizeof(T)));
                m_buffer = buffer;
                m_bufferSize = objectCount * sizeof(T);
                m_objectCount = objectCount;

                clear();
            }

        template<typename T>
        T *poolAllocater<T>::alloc()
            {
                int index = 0;
                while (!m_freeIndicies[index])
                    {
                        index += 1;
                    }

                if (index <= m_objectCount)
                    {
                        m_freeIndicies[index] = false;

                        void *retMem = m_buffer + (index * sizeof(T));
                        return static_cast<T*>(retMem);
                    }

                FE_ASSERT(false, "Error: Pool Allocater out of memory");
                return nullptr;
            }

        template<typename T>
        void poolAllocater<T>::free(T *address)
            {
                size_t offset = address - static_cast<void*>(&m_buffer[0]);
                m_freeIndicies[offset] = true;
            }

        template<typename T>
        void poolAllocater<T>::clear()
            {
                memset(m_freeIndicies, true, m_objectCount);
            }
    }