#include "drawable.hpp"
#include "transformable.hpp"
#include "../subsystems/graphic/spriteBatch.hpp"

fe::drawable::drawable() : m_verticies(sf::PrimitiveType::Quads, 4), animationActor(&m_verticies)
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

void fe::drawable::setSize(fe::Vector2d size)
    {
        m_verticies[0].position = fe::Vector2d(0, 0).convertToSfVec2();
        m_verticies[1].position = fe::Vector2d(size.x, 0).convertToSfVec2();
        m_verticies[2].position = fe::Vector2d(size.x, size.y).convertToSfVec2();
        m_verticies[3].position = fe::Vector2d(0, size.y).convertToSfVec2();
    }

fe::Vector2d fe::drawable::getSize() const
    {
        return m_verticies[2].position; // since this is corner is the farthest away, this is the largest point
    }
