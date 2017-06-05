#include "drawable.hpp"
#include "transformable.hpp"
#include "../subsystems/graphic/spriteBatch.hpp"

fe::drawable::drawable() : animationActor(&m_verticies)
    {
        m_verticies.resize(4);
        m_verticies.setPrimitiveType(sf::PrimitiveType::Quads);

        // On the packed texture, there is a 1x1 white pixel. This represents the position
        m_verticies[0].texCoords = fe::Vector2d(0, 0).convertToSfVec2();
        m_verticies[1].texCoords = fe::Vector2d(1, 0).convertToSfVec2();
        m_verticies[2].texCoords = fe::Vector2d(1, 1).convertToSfVec2();
        m_verticies[3].texCoords = fe::Vector2d(0, 1).convertToSfVec2();
    }

void fe::drawable::draw(fe::spriteBatch &batch, fe::transformable &transform)
    {
        batch.add(m_verticies, transform);
    }

void fe::drawable::setColour(const sf::Color &colour)
    {
        m_colour = colour;
        for (unsigned int i = 0; i < m_verticies.getVertexCount(); i++)
            {
                m_verticies[i].color = colour;
            }
    }

sf::Color fe::drawable::getColour() const
    {
        return m_colour;
    }
