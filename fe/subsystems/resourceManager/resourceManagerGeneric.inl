template<typename T>
const T *fe::resourceManager<T>::load(const char* filepath, const char* id)
    {
        const T *cResource = get(id);
        if (cResource)
            {
                return cResource;
            }

        T *resource = new T();
        if (resource)
            {
                resource->loadFromFile(filepath);

                m_resources[FE_STR(id)] = resource;
                return resource;
            }
        else
            {
                FE_LOG_WARNING(id, "from filepath", filepath, "cannot be loaded into memory");
            }

        return nullptr;
    }

template<typename T>
const T *fe::resourceManager<T>::get(const char* id)
    {
        return m_resources[FE_STR(id)];
    }

template<typename T>
const T *fe::resourceManager<T>::get(fe::str id)
    {
        return m_resources[id];
    }

template<typename T>
void fe::resourceManager<T>::remove(const char* id)
    {
        m_resources[FE_STR(id)]->~T();
        m_resources.erase(FE_STR(id));
    }

template<typename T>
void fe::resourceManager<T>::remove(fe::str id)
    {
        m_resources[id]->~T();
        m_resources.erase(id);
    }

template<typename T>
void fe::resourceManager<T>::shutDown()
    {
        for (auto &obj : m_resources)
            {
                obj.second->~T();
            }
    }