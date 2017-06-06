// handleManager.hpp
// stores handles to objects being pushed. Inherit off of to enable handles
#pragma once
#include "../debug/logger.hpp"
#include <iterator>
#include <vector>
#include <algorithm>

namespace fe
    {
        using Handle = int;

        /*
            The handles work by storing the object in a normal vector, and another vector full off indicies to the objects in the original vector
            When we create an object, we return the index to the handleObject which in turn has their handle pointing to the index of the wanted object
            This works because we can always assume the indicies of the handleObjects wont change, while we can remove and add objects dynamically
        */
        template<typename T>
        class handleManager
            {
                private:
                    struct handleObject
                        {
                            Handle handle; // Index to the object
                            bool active;

                            handleObject(Handle handle) : handle(handle), active(true) {}
                        };

                    std::vector<T> m_objects;
                    std::vector<handleObject> m_handles;

                protected:
                    std::vector<T> &getObjects();
                    typename std::vector<T>::iterator removeHandle(Handle handle);

                    fe::Handle getHandle(T object);

                    virtual void onAdd(T object, fe::Handle objectHandle) {}

                public:
                    Handle addObject(T object);
                    void removeObject(Handle handle);
                    bool handleActive(Handle handle);

                    T getObject(Handle handle) const;

            };

        template<typename T>
        inline std::vector<T> &handleManager<T>::getObjects()
            {
                return m_objects;
            }

        template<typename T>
        typename std::vector<T>::iterator handleManager<T>::removeHandle(Handle handle)
            {
                auto it = m_objects.erase(m_objects.begin() + m_handles[handle].handle); 
                m_handles[handle].active = false;

                // since we are erasing an entity, all handles above it will become invalid. To prevent this, we will subtract all handles
                // above and including the current one by one.
                for (auto it = m_handles.begin() + handle; it != m_handles.end(); ++it) 
                    {
                        it->handle -= 1;
                        if (it->handle < 0)
                            {
                                it->active = false;
                            }
                    }

                return it;
            }

        // Really inefficient, but this shouldnt be used at runtime anyway
        template<typename T>
        fe::Handle handleManager<T>::getHandle(T object)
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
        Handle handleManager<T>::addObject(T object)
            {
                m_objects.push_back(object);
                m_handles.push_back(handleObject(m_objects.size() - 1));
                onAdd(m_objects.back(), m_handles.size() - 1);
                return m_handles.size() - 1;
            }

        template<typename T>
        void handleManager<T>::removeObject(Handle handle)
            {
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
        bool handleManager<T>::handleActive(Handle handle)
            {
                if (handle >= m_handles.size())
                    {
                        FE_LOG_WARNING("Cannot get handle \"", handle, "\"");
                        return false;
                    }

                return m_handles[handle].active;
            }

        template<typename T>
        T handleManager<T>::getObject(Handle handle) const
            {
                if (handle >= m_handles.size())
                    {
                        FE_LOG_WARNING("Cannot retrieve object with handle \"", handle, "\"");
                        return T();
                    }
                else if (!m_handles[handle].active)
                    {
                        FE_LOG_DEBUG("Object with handle \"", handle, "\" inactive");
                        if (m_handles[handle].handle < 0)
                            {
                                FE_LOG_ERROR("Error: Handle", handle, "Attempting to get hidden-handle of < 0");
                                return T();
                            }
                    }

                return m_objects[m_handles[handle].handle];
            }
    }