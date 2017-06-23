// poolAllocater.hpp
// A memory allocater that will allow the user to define an object in which to pool memory blocks for
#pragma once
#include "../../debug/logger.hpp"
#include "../../feAssert.hpp"
#include "memoryManager.hpp"

namespace fe
    {
        template<typename T>
        class poolAllocater
            {
                private:
                    size_t m_bufferSize;

                    unsigned int m_objectCount;
                    unsigned int m_lastObjectIndex;

                    bool m_canAllocate;
                    bool *m_freeIndicies; // if there is a block that can be allocated, the corresponding index in this array will be "true"
                    char *m_buffer;

                public:
                    void startUp(const unsigned int objectCount);
                    unsigned int getObjectAllocCount() const;

                    // allocates a block of memory for the wanted object
                    template<typename ...Args>
                    T *alloc(Args ...args);

                    // Frees the object at the address it is allocated to
                    void free(T *address);
                    // Frees the object at the address it is allocated to
                    void free(const T *address);

                    // Returns the object constructed at the address
                    T *at(unsigned int index);

                    // clears the allocated memory to allow for a new 
                    void clear();

            };

        template<typename T>
        void poolAllocater<T>::startUp(const unsigned int objectCount)
            {
                m_canAllocate = false;

                auto *memManager = &memoryManager::get();
                if (memManager)
                    {
                        m_freeIndicies = static_cast<bool*>(memManager->alloc(objectCount * sizeof(T)));
                        std::memset(m_freeIndicies, true, sizeof(m_freeIndicies));

                        auto size = sizeof(T) * objectCount;
                        m_buffer = static_cast<char*>(fe::memoryManager::get().getStackAllocater().alloc(size));

                        m_bufferSize = objectCount * sizeof(T);
                        m_objectCount = objectCount;
                        m_lastObjectIndex = 0;

                        m_canAllocate = true;
                        clear();
                    }
            }

        template<typename T>
        unsigned int poolAllocater<T>::getObjectAllocCount() const
            {
                return m_lastObjectIndex;
            }

        template<typename T>
        template<typename ...Args>
        T *poolAllocater<T>::alloc(Args ...args)
            {
                if (!m_canAllocate)
                    {
                        FE_LOG_WARNING("Cannot allocate memory in pool");
                        return nullptr;
                    }

                int index = 0;
                while (!m_freeIndicies[index])
                    {
                        index++;
                    }

                if (index <= m_objectCount)
                    {
                        m_freeIndicies[index] = false;
                        m_lastObjectIndex = index >= m_lastObjectIndex ? index + 1 : m_lastObjectIndex;
                        return new(m_buffer + (index * sizeof(T))) T(args...);
                    }

                FE_LOG_WARNING("No memory allocated in pool. Attempted allocation of", sizeof(sizeof(T)), "bytes");
                FE_ASSERT(m_objectCount == 0, "Pool Allocater out of memory");
                return nullptr;
            }

        template<typename T>
        void poolAllocater<T>::free(T *address)
            {
                size_t offset = address - static_cast<void*>(&m_buffer[0]);
                m_freeIndicies[offset] = true;
            }

        template<typename T>
        void poolAllocater<T>::free(const T *address)
            {
                size_t offset = address - static_cast<void*>(&m_buffer[0]);
                m_freeIndicies[offset] = true;
            }

        template<typename T>
        T *poolAllocater<T>::at(unsigned int index)
            {
                return reinterpret_cast<T*>(m_buffer + (index * sizeof(T)));
            }

        template<typename T>
        void poolAllocater<T>::clear()
            {
                std::memset(m_freeIndicies, true, m_objectCount);
                std::memset(m_buffer, 0, m_objectCount);
            }
    }