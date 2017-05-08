// drawable.hpp
// allows an entity to be drawn. Has basic functions to ease the process of getting graphics on screen
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class transformable;
        class spriteBatch;

        class drawable
            {
                protected:
                    sf::VertexArray m_verticies;
                    sf::Texture *m_texture;

                    sf::Color m_colour;

                    friend class sceneGraph;

                public:
                    FLAT_ENGINE_API drawable();
                    FLAT_ENGINE_API void draw(fe::spriteBatch &batch, fe::transformable &transform);

                    FLAT_ENGINE_API void setTexture(sf::Texture *texture);
                    FLAT_ENGINE_API sf::Texture *getTexture() const;

                    FLAT_ENGINE_API void setColour(const sf::Color &colour);
                    FLAT_ENGINE_API sf::Color getColour() const;


            };
    }