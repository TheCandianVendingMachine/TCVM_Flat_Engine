// handleManagerRaw.inl
// defines the raw array version of the handle manager
template<typename T, unsigned int objectCount>
fe::handleManager<T, objectCount>::handleManager()
    {
        handleObjectList *base = &m_baseHandleList;
        for (unsigned int i = 0; i < m_objectCount; i++)
            {
                m_handles[i].handle = i;
                base->m_handle = &m_handles[i];
                if (i + 1 < m_objectCount)
                    {
                        base->m_tail = new handleObjectList;
                        base = base->m_tail;
                    }
            }
        m_maxIndex = 0;
    }

template<typename T, unsigned int objectCount>
inline T *fe::handleManager<T, objectCount>::getObjects()
    {
        return m_objects;
    }

// Really inefficient, but this shouldnt be used at runtime anyway
template<typename T, unsigned int objectCount>
fe::Handle fe::handleManager<T, objectCount>::getHandle(T object)
    {
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
        m_maxIndex = testHandle->m_handle->handle + 1;
        onAdd(object, testHandle->m_handle->handle);
        return testHandle->m_handle->handle;
    }

template<typename T, unsigned int objectCount>
void fe::handleManager<T, objectCount>::removeObject(Handle handle)
    {
        m_handles[handle]->active = false;
        m_objects[m_handles[handle].handle] = T();
    }

template<typename T, unsigned int objectCount>
bool fe::handleManager<T, objectCount>::handleActive(Handle handle)
    {
        return m_handles[handle]->active;
    }

template<typename T, unsigned int objectCount>
T fe::handleManager<T, objectCount>::getObject(Handle handle) const
    {
        return m_objects[m_handles[handle].handle];
    }