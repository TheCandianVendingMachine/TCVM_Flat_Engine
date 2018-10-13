// handleManagerVector.inl
// defines for the vector version of the handle manager
#include "handleManager.hpp"
#include <functional>
template<typename T>
inline const std::vector<T> &fe::handleManager<T, 0>::getObjects() const
    {
        return m_objects;
    }

template<typename T>
typename std::vector<T>::iterator fe::handleManager<T, 0>::removeHandle(Handle handle)
    {
        auto it = m_objects.erase(m_objects.begin() + m_handles[handle].handle); 
        m_handles[handle].active = false;

        fe::Handle arrayHandle = m_handles[handle].handle;

        // since we are erasing an entity, all handles above it will become invalid. To prevent this, we will subtract all handles
        // above and including the current one by one.
        for (auto it = m_handles.begin(); it != m_handles.end(); ++it) 
            {
                if (it->handle >= arrayHandle)
                    {
                        it->handle -= 1;
                        if (it->handle < 0)
                            {
                                it->active = false;
                            }
                    }
            }

        return it;
    }

// Really inefficient, but this shouldnt be used at runtime anyway
template<typename T>
fe::Handle fe::handleManager<T, 0>::getHandle(T object)
    {
        for (unsigned int i = 0; i < m_handles.size(); i++)
            {
                // we check if the handle is >= 0 here because we are looping through every handle - we can't guarentee there will be
                // safe handles.
                // we access the objects directly as for hte handles in m_handles are indicies to the object vector
                if (m_handles[i].handle >= 0 && m_objects[m_handles[i].handle] == object)
                    {
                        return i; // return the index to the handle
                    }
            }

        FE_LOG_WARNING("Cannot retrive handle with object");
        return -1;
    }

template<typename T>
fe::Handle fe::handleManager<T, 0>::addObject(T object, Handle handle)
    {
        if (m_handles.size() <= handle)
            {
                m_handles.resize(handle);
                return addObject(object);
            }
        
        m_objects.push_back(object);
        m_handles[handle].handle = m_objects.size() - 1;
        m_handles[handle].active = true;

        onAdd(&m_objects.back(), handle);

        return handle;
    }

template<typename T>
fe::Handle fe::handleManager<T, 0>::addObject(T object)
    {
        m_objects.push_back(object);
        m_handles.push_back(handleObject(static_cast<fe::Handle>(m_objects.size() - 1)));

        fe::Handle retHandle = fe::Handle(m_handles.size() - 1);

        onAdd(&m_objects.back(), retHandle);
        return retHandle;
    }

template<typename T>
void fe::handleManager<T, 0>::removeObject(Handle handle)
    {
        removeObject(handle, [](T*){});
    }

template<typename T>
void fe::handleManager<T, 0>::removeObject(Handle handle, std::function<void(T*)> onRemoveFunc)
    {
        if (handle < 0) 
            {
                FE_LOG_WARNING("Invalid Handle");
                return;
            }
        if (m_handles[handle].handle >= m_objects.size())
            {
                FE_LOG_WARNING("Handle out of range");
                return;
            }
        onRemoveFunc(&m_objects[m_handles[handle].handle]);
        onRemove(&m_objects[m_handles[handle].handle], m_handles[handle].handle);
        if (m_handles.begin() + handle < m_handles.end())
            {
                removeHandle(handle);
            }
        else if (m_handles.begin() + handle >= m_handles.end())
            {
                FE_LOG_WARNING("Cannot remove object with handle \"", handle, "\"");
            }
        else
            {
                FE_LOG_DEBUG("Object with handle \"", handle, "\" inactive");
            }
    }

template<typename T>
bool fe::handleManager<T, 0>::handleActive(Handle handle)
    {
        if (handle >= m_handles.size())
            {
                FE_LOG_WARNING("Cannot get handle \"", handle, "\"");
                return false;
            }

        return m_handles[handle].active;
    }

template<typename T>
inline void fe::handleManager<T, 0>::clearAllObjects()
    {
        clearAllObjects([](T*){});
    }

template<typename T>
inline void fe::handleManager<T, 0>::clearAllObjects(std::function<void(T*)> onRemoveFunc)
    {
        for (auto &obj : m_objects)
            {
                onRemove(&obj, -1);
                onRemoveFunc(&obj);
            }
        m_objects.clear();
        m_handles.clear();
    }

template<typename T>
typename std::remove_pointer<T>::type *fe::handleManager<T, 0>::getObject(Handle handle) const
    {
        if (handle >= m_handles.size())
            {
                FE_LOG_WARNING("Cannot retrieve object with handle \"", handle, "\"");
                return nullptr;
            }
        else if (!m_handles[handle].active)
            {
                FE_LOG_DEBUG("Object with handle \"", handle, "\" inactive");
                if (m_handles[handle].handle < 0)
                    {
                        FE_LOG_ERROR("Error: Handle", handle, "Attempting to get hidden-handle of < 0");
                        return nullptr;
                    }
            }

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