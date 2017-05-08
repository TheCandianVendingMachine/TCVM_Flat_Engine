// handleManager.hpp
// stores handles to objects being pushed. Inherit off of to enable handles
#pragma once
#include "../debug/logger.hpp"

#include <vector>
#include <algorithm>

namespace fe
    {
        using Handle = int;

        template<typename T>
        class handleManager
            {
                private:
                    struct handleObject
                        {
                            Handle handle;
                            bool active;

                            handleObject(Handle handle) : handle(handle), active(true) {}
                        };

                    std::vector<T> m_objects;
                    std::vector<handleObject> m_handles;

                protected:
                    std::vector<T> &getObjects();
                    virtual void onAdd(T object) {}

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
        Handle handleManager<T>::addObject(T object)
            {
                m_objects.push_back(object);
                m_handles.push_back(handleObject(m_objects.size() - 1));
                return m_handles.size() - 1;
            }

        template<typename T>
        void handleManager<T>::removeObject(Handle handle)
            {
                if (m_handles.begin() + handle < m_handles.end())
                    {
                        m_objects.erase(m_objects.begin() + m_handles[handle].handle); 
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
                    }

                return m_objects[m_handles[handle].handle];
            }
    }