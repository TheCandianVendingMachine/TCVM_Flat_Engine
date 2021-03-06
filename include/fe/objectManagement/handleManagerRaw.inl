// handleManagerRaw.inl
// defines the raw array version of the handle manager
#include <algorithm>
#include <functional>
#include "handleManager.hpp"
template<typename T, unsigned int objectCount>
fe::handleManager<T, objectCount>::handleManager()
    {
        handleObjectList *base = &m_baseHandleList;
        for (unsigned int i = 0; i < m_objectCount; i++)
            {
                m_handles[i].handle = i;
                m_handles[i].active = false;
                base->m_handle = &m_handles[i];
                if (i + 1 < m_objectCount)
                    {
                        base->m_tail = new handleObjectList;
                        base = base->m_tail;
                    }
            }
        m_maxIndex = 0;
    }

template<typename T, unsigned int TObjectCount>
fe::Handle fe::handleManager<T, TObjectCount>::addObject(T object, Handle handle)
    {
        handleObjectList *testHandle = &m_baseHandleList;
        while (testHandle->m_handle->handle != handle)
            {
                testHandle = testHandle->m_tail;
                if (!testHandle)
                    {
                        return -1;
                    }
            }

        testHandle->m_handle->active = true;
        m_objects[testHandle->m_handle->handle] = object;
        return handle;
    }

template<typename T, unsigned int objectCount>
inline const T *fe::handleManager<T, objectCount>::getObjects() const
    {
        return m_objects;
    }

// Really inefficient, but this shouldnt be used at runtime anyway
template<typename T, unsigned int objectCount>
fe::Handle fe::handleManager<T, objectCount>::getHandle(T object)
    {
        FE_LOG_WARNING("Not Implemented");
        return -1;
    }

template<typename T, unsigned int objectCount>
fe::Handle fe::handleManager<T, objectCount>::addObject(T object)
    {
        handleObjectList *testHandle = &m_baseHandleList;
        while (testHandle->m_handle->active)
            {
                testHandle = testHandle->m_tail;
                if (!testHandle)
                    {
                        return -1;
                    }
            }

        testHandle->m_handle->active = true;
        m_objects[testHandle->m_handle->handle] = object;
        m_maxIndex = std::max(static_cast<unsigned int>(testHandle->m_handle->handle + 1), m_maxIndex);
        onAdd(&object, testHandle->m_handle->handle);
        return testHandle->m_handle->handle;
    }

template<typename T, unsigned int objectCount>
void fe::handleManager<T, objectCount>::removeObject(Handle handle)
    {
        removeObject(handle, [](T*){});
    }

template<typename T, unsigned int TObjectCount>
void fe::handleManager<T, TObjectCount>::removeObject(Handle handle, std::function<void(T*)> onRemoveFunc)
    {
        if (handle < 0 || !m_handles[handle].active) return;
        onRemove(&m_objects[m_handles[handle].handle], m_handles[handle].handle);
        m_handles[handle].active = false;
        onRemoveFunc(&m_objects[m_handles[handle].handle]);
        m_objects[m_handles[handle].handle] = T();
    }

template<typename T, unsigned int objectCount>
bool fe::handleManager<T, objectCount>::handleActive(Handle handle) const
    {
        if (handle < 0) return false;
        return m_handles[handle].active;
    }

template<typename T, unsigned int objectCount>
inline void fe::handleManager<T, objectCount>::clearAllObjects()
    {
        clearAllObjects([](T*){});
    }

template<typename T, unsigned int TObjectCount>
inline void fe::handleManager<T, TObjectCount>::clearAllObjects(std::function<void(T*)> onRemoveFunc)
    {
        for (unsigned int i = 0; i < m_maxIndex + 1; i++)
            {
                removeObject(i, onRemoveFunc);
            }

        handleObjectList *handle = &m_baseHandleList;
        int index = 0;
        while (handle)
            {
                handle->m_handle->active = false;
                handle->m_handle->handle = index++;
                handle = handle->m_tail;
            }
        m_maxIndex = 0;
    }

template<typename T, unsigned int objectCount>
typename std::remove_pointer<T>::type *fe::handleManager<T, objectCount>::getObject(Handle handle) const
    {
        if (handle < 0) return T();
        if constexpr (std::is_pointer<T>::value) 
            {
                return m_objects[m_handles[handle].handle];
            }
        else
            {
                return const_cast<T*>(&m_objects[m_handles[handle].handle]);
            }
        return nullptr;
    }