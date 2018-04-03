// resourceManager.hpp
// loads and stores all resources so they don't go out of scope for the program
#pragma once
#include <unordered_map>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include "../../typeDefines.hpp"
#include "../../objectManagement/str.hpp"
#include "../../debug/logger.hpp"
#include "../graphic/texturePacker.hpp"
#include "fontData.hpp"

namespace fe 
    {
        template<typename T>
        class resourceManager
            {
                private:
                    std::unordered_map<fe::str, T*> m_resources;

                public:
                    const T *load(const char* filepath, const char* id);
                    const T *get(const char* id);
                    const T *get(fe::str id);

                    void remove(const char* id);
                    void remove(fe::str id);

                    void shutDown();
            };

        template<>
        class resourceManager<sf::Texture>
            {
                private:
                    std::vector<const sf::Texture*> m_textures;
                    fe::texturePacker m_packed;

                public:
                    resourceManager();

                    sf::Texture *load(const char* filepath, const char* id);
                    sf::Texture *add(sf::Texture *texture, const char *id);

                    // Creates the TTF font into a bitmap font
                    fe::fontData addFont(const sf::Font *font, const char *id, unsigned int size);

                    sf::Texture *getTexture(const char* id);
                    sf::Texture *getTexture(fe::str id);

                    [[deprecated("Deprecated. Use \"getTexturePosition(const char*)\" instead")]] fe::Vector2<unsigned int> getTextureOffset(const char* id);
                    [[deprecated("Deprecated. Use \"getTexturePosition(fe::str)\" instead")]] fe::Vector2<unsigned int> getTextureOffset(fe::str id);

                    fe::Vector2<unsigned int> getTexturePosition(const char* id);
                    fe::Vector2<unsigned int> getTexturePosition(fe::str id);

                    void remove(const char* id);
                    void remove(fe::str id);

                    // Get the packed texture
                    sf::Texture &get();

                    void shutDown();

            };

    }

#include "resourceManagerGeneric.inl"
#include "resourceManagerTexture.inl"