#include "fe/subsystems/particle/particleBatch.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void fe::particleBatch::reset(unsigned int size)
    {
        m_verticies.clear();
        m_verticies.resize(size * 4);
    }

void fe::particleBatch::update(unsigned int index, float x, float y, float radius, sf::Color colour)
    {
        if ((index * 4) >= m_verticies.size())
            {
                m_verticies.resize((index + 1) * 4);
            }

        m_verticies[(index * 4) + 0].position.x = x - radius;
        m_verticies[(index * 4) + 0].position.y = y - radius;
        
        m_verticies[(index * 4) + 1].position.x = x + radius;
        m_verticies[(index * 4) + 1].position.y = y - radius;
        
        m_verticies[(index * 4) + 2].position.x = x + radius;
        m_verticies[(index * 4) + 2].position.y = y + radius;

        m_verticies[(index * 4) + 3].position.x = x - radius;
        m_verticies[(index * 4) + 3].position.y = y + radius;


        m_verticies[(index * 4) + 0].color = colour;
        m_verticies[(index * 4) + 1].color = colour;
        m_verticies[(index * 4) + 2].color = colour;
        m_verticies[(index * 4) + 3].color = colour;

        m_maxIndex = std::max(m_maxIndex, index);
    }


void fe::particleBatch::draw(sf::RenderTarget &target)
    {
        target.draw(m_verticies.data(), m_maxIndex * 4, sf::PrimitiveType::Quads);
        m_maxIndex = 0;
    }
