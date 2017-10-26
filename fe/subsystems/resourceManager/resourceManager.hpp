// resourceManager.hpp
// loads and stores all resources so they don't go out of scope for the program
#pragma once
#include <unordered_map>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include "../../objectManagement/guid.hpp"
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

                    // Get the packed texture
                    const sf::Texture &get();

                    void shutDown();

            };

    }

#include "resourceManagerGeneric.inl"
#include "resourceManagerTexture.inl"