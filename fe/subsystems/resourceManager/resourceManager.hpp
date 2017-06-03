// resourceManager.hpp
// loads and stores all resources so they don't go out of scope for the program
#pragma once
#include <unordered_map>
#include <vector>
#include "../../objectManagement/guid.hpp"
#include "../memory/feNew.hpp"
#include "../../debug/logger.hpp"
#include "../graphic/texturePacker.hpp"

namespace fe 
    {
        template<typename T>
        class resourceManager
            {
                private:
                    std::unordered_map<fe::guid, T*> m_resources;

                public:
                    const T *load(const char* filepath, const char* id);
                    const T *get(const char* id);
                    const T *get(fe::guid id);

                    void remove(const char* id);
                    void remove(fe::guid id);

                    void shutDown();
            };

        template<typename T>
        const T *resourceManager<T>::load(const char* filepath, const char* id)
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
        const T *resourceManager<T>::get(const char* id)
            {
                return m_resources[FE_STR(id)];
            }

        template<typename T>
        const T *resourceManager<T>::get(fe::guid id)
            {
                return m_resources[id];
            }

        template<typename T>
        void resourceManager<T>::remove(const char* id)
            {
                m_resources[FE_STR(id)]->~T();
                m_resources.erase(FE_STR(id));
            }

        template<typename T>
        void resourceManager<T>::remove(fe::guid id)
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

                    const sf::Texture *load(const char* filepath, const char* id);

                    const sf::Texture *getTexture(const char* id);
                    const sf::Texture *getTexture(fe::guid id);

                    [[deprecated("Deprecated. Use \"getTexturePosition(const char*)\" instead")]] fe::Vector2<unsigned int> getTextureOffset(const char* id);
                    [[deprecated("Deprecated. Use \"getTexturePosition(fe::guid)\" instead")]] fe::Vector2<unsigned int> getTextureOffset(fe::guid id);

                    fe::Vector2<unsigned int> getTexturePosition(const char* id);
                    fe::Vector2<unsigned int> getTexturePosition(fe::guid id);

                    void remove(const char* id);
                    void remove(fe::guid id);

                    const sf::Texture &get();

                    void shutDown();

            };

        inline resourceManager<sf::Texture>::resourceManager()
            {
                m_packed.createTexture();
            }

        inline const sf::Texture *resourceManager<sf::Texture>::load(const char* filepath, const char* id)
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

        inline const sf::Texture *resourceManager<sf::Texture>::getTexture(const char* id)
            {
                return m_packed.getTexture(id);
            }

        inline const sf::Texture *resourceManager<sf::Texture>::getTexture(fe::guid id)
            {
                return m_packed.getTexture(id);
            }

        inline fe::Vector2<unsigned int> resourceManager<sf::Texture>::getTextureOffset(const char* id)
            {
                return m_packed.getTexturePosition(id);
            }

        inline fe::Vector2<unsigned int> resourceManager<sf::Texture>::getTextureOffset(fe::guid id)
            {
                return m_packed.getTexturePosition(id);
            }

        inline const sf::Texture &resourceManager<sf::Texture>::get()
            {
                return m_packed.getTexture();
            }

        inline fe::Vector2<unsigned int> resourceManager<sf::Texture>::getTexturePosition(const char* id)
            {
                return m_packed.getTexturePosition(id);
            }

        inline fe::Vector2<unsigned int> resourceManager<sf::Texture>::getTexturePosition(fe::guid id)
            {
                return m_packed.getTexturePosition(id);
            }

        inline void resourceManager<sf::Texture>::shutDown()
            {}
    }