// resourceManager.hpp
// loads and stores all resources so they don't go out of scope for the program
#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "../memory/feNew.hpp"
#include "../../debug/logger.hpp"
#include "../graphic/texturePacker.hpp"

namespace fe 
    {
        template<typename T>
        class resourceManager
            {
                private:
                    std::unordered_map<std::string, T*> m_resources;

                public:
                    const T *load(const std::string &filepath, const std::string &id);
                    const T *get(const std::string &id);
                    void remove(const std::string &id);

                    void shutDown();
            };

        template<typename T>
        const T *resourceManager<T>::load(const std::string &filepath, const std::string &id)
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
        const T *resourceManager<T>::get(const std::string &id)
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

        template<>
        class resourceManager<sf::Texture>
            {
                private:
                    std::vector<sf::Texture*> m_textures;
                    fe::texturePacker m_packed;

                public:
                    resourceManager();

                    const sf::Texture *load(const std::string &filepath, const std::string &id);
                    const sf::Texture *getTexture(const std::string &id);
                    fe::Vector2<unsigned int> getTextureOffset(const std::string &id);

                    const sf::Texture &get();
                    fe::Vector2<unsigned int> getTexturePosition(const std::string &id);
                    void remove(const std::string &id);

                    void shutDown();

            };

        inline resourceManager<sf::Texture>::resourceManager()
            {
                m_packed.createTexture();
            }

        inline const sf::Texture *resourceManager<sf::Texture>::load(const std::string &filepath, const std::string &id)
            {
                const sf::Texture *resource = getTexture(id);
                if (resource)
                    {
                        return resource;
                    }

                sf::Texture *added = new sf::Texture();
                if (added)
                    {
                        added->loadFromFile(filepath);

                        m_textures.push_back(added);
                        m_packed.addTexture(*added, id);
                        return added;
                    }
                else
                    {
                        FE_LOG_WARNING("Texture\"", id, "\"from filepath\"", filepath, "\"cannot be loaded into memory");
                    }

                return nullptr;
            }

        inline const sf::Texture *resourceManager<sf::Texture>::getTexture(const std::string &id)
            {
                return m_packed.getTexture(id);
            }

        inline fe::Vector2<unsigned int> resourceManager<sf::Texture>::getTextureOffset(const std::string &id)
            {
                return m_packed.getTexturePosition(id);
            }

        inline const sf::Texture &resourceManager<sf::Texture>::get()
            {
                return m_packed.getTexture();
            }

        inline fe::Vector2<unsigned int> resourceManager<sf::Texture>::getTexturePosition(const std::string &id)
            {
                return m_packed.getTexturePosition(id);
            }

        inline void remove(const std::string &id)
            {
        
            }

        inline void resourceManager<sf::Texture>::shutDown()
            {}
    }