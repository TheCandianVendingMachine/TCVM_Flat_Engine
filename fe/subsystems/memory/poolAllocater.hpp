// poolAllocater.hpp
// A memory allocater that will allow the user to define an object in which to pool memory blocks for
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../debug/logger.hpp"
#include "../../feAssert.hpp"
#include "memoryManager.hpp"

namespace fe
    {
        template<typename T>
        class FLAT_ENGINE_API poolAllocater
            {
                private:
                    size_t m_bufferSize;

                    unsigned int m_objectCount;

                    bool m_canAllocate;
                    bool *m_freeIndicies; // if there is a block that can be allocated, the corresponding index in this array will be "true"
                    char *m_buffer;

                public:
                    void startUp(const unsigned int objectCount, char *buffer);

                    // allocates a block of memory for the wanted object
                    template<typename TRet = T, typename ...Args>
                    TRet *alloc(Args ...args);

                    // Frees the object at the address it is allocated to
                    void free(T *address);

                    // clears the allocated memory to allow for a new 
                    void clear();

            };

        template<typename T>
        void poolAllocater<T>::startUp(const unsigned int objectCount, char *buffer)
            {
                m_canAllocate = false;

                auto *memManager = &memoryManager::get();
                if (memManager)
                    {
                        m_freeIndicies = static_cast<bool*>(memManager->alloc(objectCount * sizeof(T)));
                        memManager->logAllocation("PoolAllocater", objectCount * sizeof(T), fe::memoryLogger::ALLOC_DIRECT);

                        std::memset(m_freeIndicies, true, sizeof(m_freeIndicies));

                        m_buffer = buffer;
                        m_bufferSize = objectCount * sizeof(T);
                        m_objectCount = objectCount;

                        m_canAllocate = true;
                        clear();
                    }
            }

        template<typename T>
        template<typename TRet, typename ...Args>
        TRet *poolAllocater<T>::alloc(Args ...args)
            {
                if (sizeof(TRet) % sizeof(T) != 0)
                    {
                        // if the size of the defined type, and the actual size are not the same we are going to allocate as much as we need, and then some
                        FE_LOG_WARNING("Objects in memory pool are not equal sizes. Buffer not guarenteed to be continious. Memory difference: ", sizeof(TRet) % sizeof(T));
                    }

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

                FE_LOG_DEBUG(sizeof(TRet));
                if (index <= m_objectCount)
                    {
                        int maxIndex = index + (sizeof(TRet) / sizeof(T)) + (sizeof(TRet) % sizeof(T));
                        std::memset(m_freeIndicies + index, false, maxIndex);

                        TRet *retMem = new(m_buffer + (index * sizeof(T))) TRet(args...);
                        return retMem;
                    }

                FE_LOG_WARNING("No memory allocated in pool. Attempted allocation of", sizeof(sizeof(TRet)), "bytes");
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
        void poolAllocater<T>::clear()
            {
                memset(m_freeIndicies, true, m_objectCount);
            }
    }