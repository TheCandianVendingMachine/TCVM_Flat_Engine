// resourceManager.hpp
// loads and stores all resources so they don't go out of scope for the program
#pragma once
#include <unordered_map>
#include <string>
#include "../memory/feNew.hpp"
#include "../../debug/logger.hpp"

namespace fe 
    {
        template<typename T>
        class resourceManager
            {
                private:
                    std::unordered_map<std::string, T*> m_resources;

                public:
                    T *load(const std::string &filepath, const std::string &id);
                    T *get(const std::string &id);
                    void remove(const std::string &id);

                    void shutDown();
            };

        template<typename T>
        T *resourceManager<T>::load(const std::string &filepath, const std::string &id)
            {
                T *resource = get(id);
                if (resource)
                    {
                        return resource;
                    }

                resource = new T();
                if (resource)
                    {
                        resource->loadFromFile(filepath);

                        m_resources[id] = resource;
                        return resource;
                    }
                else
                    {
                        FE_LOG_WARNING(id, "from filepath", filepath, "cannot be loaded into memory");
                    }

                return nullptr;
            }

        template<typename T>
        T *resourceManager<T>::get(const std::string &id)
            {
                return m_resources[id];
            }

        template<typename T>
        void resourceManager<T>::remove(const std::string &id)
            {
                m_resources[id]->~T();
                m_resources.erase(id);
            }

        template<typename T>
        void resourceManager<T>::shutDown()
            {
                for (auto &obj : m_resources)
                    {
                        obj.second->~T();
                    }
            }
    }