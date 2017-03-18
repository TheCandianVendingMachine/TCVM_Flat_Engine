#include "drawable.hpp"
#include "transformable.hpp"
#include "../subsystems/graphic/spriteBatch.hpp"

fe::drawable::drawable() : m_verticies(sf::PrimitiveType::Quads, 4)
    {
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
