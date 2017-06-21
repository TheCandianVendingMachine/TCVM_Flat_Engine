#include "spriteBatch.hpp"
#include "../../../entity/transformable.hpp"
#include "../../../debug/logger.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::spriteBatch::spriteBatch() : m_batch(sf::PrimitiveType::Quads), m_maxVertexCount(0), m_lastVertexCount(0)
    {
    }

void fe::spriteBatch::add(const fe::renderObject *entity, fe::transformable &transform)
    {
        m_currentEntityCount++;
        if (m_needsPurge || m_index >= m_lastVertexCount) 
            {
                m_batch.append(sf::Vertex(transform.getMatrix().transformPoint({ entity->m_verticies[0], entity->m_verticies[1] }).convertToSfVec2(),
                               sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]),
                               { entity->m_texCoords[0], entity->m_texCoords[1] }));

                m_batch.append(sf::Vertex(transform.getMatrix().transformPoint({ entity->m_verticies[0] + entity->m_verticies[3], entity->m_verticies[1] }).convertToSfVec2(),
                               sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]),
                               { entity->m_texCoords[0], entity->m_texCoords[1] }));

                m_batch.append(sf::Vertex(transform.getMatrix().transformPoint({ entity->m_verticies[0] + entity->m_verticies[3], entity->m_verticies[1] + entity->m_verticies[4] }).convertToSfVec2(),
                               sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]),
                               { entity->m_texCoords[0], entity->m_texCoords[1] }));

                m_batch.append(sf::Vertex(transform.getMatrix().transformPoint({ entity->m_verticies[0], entity->m_verticies[1] + entity->m_verticies[4] }).convertToSfVec2(),
                               sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]),
                               { entity->m_texCoords[0], entity->m_texCoords[1] }));
            }
        else
            {
                m_batch[m_index + 0].position = transform.getMatrix().transformPoint({ entity->m_verticies[0], entity->m_verticies[1] }).convertToSfVec2();
                m_batch[m_index + 1].position = transform.getMatrix().transformPoint({ entity->m_verticies[0] + entity->m_verticies[3], entity->m_verticies[1] }).convertToSfVec2();
                m_batch[m_index + 2].position = transform.getMatrix().transformPoint({ entity->m_verticies[0] + entity->m_verticies[3], entity->m_verticies[1] + entity->m_verticies[4] }).convertToSfVec2();
                m_batch[m_index + 3].position = transform.getMatrix().transformPoint({ entity->m_verticies[0], entity->m_verticies[1] + entity->m_verticies[4] }).convertToSfVec2();

                m_batch[m_index + 0].color = sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]);
                m_batch[m_index + 1].color = sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]);
                m_batch[m_index + 2].color = sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]);
                m_batch[m_index + 3].color = sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]);

                m_batch[m_index + 0].texCoords = { entity->m_texCoords[0],                          entity->m_texCoords[1] };
                m_batch[m_index + 1].texCoords = { entity->m_texCoords[0] + entity->m_verticies[3], entity->m_texCoords[1] };
                m_batch[m_index + 2].texCoords = { entity->m_texCoords[0] + entity->m_verticies[3], entity->m_texCoords[1] + entity->m_verticies[4] };
                m_batch[m_index + 3].texCoords = { entity->m_texCoords[0],                          entity->m_texCoords[1] + entity->m_verticies[4] };
            }

        m_maxVertexCount++;
        m_index++;
    }

void fe::spriteBatch::clear()
    {
        m_lastVertexCount = m_lastVertexCount > m_maxVertexCount ? m_lastVertexCount : m_maxVertexCount;
        m_maxVertexCount = 0;
        m_index = 0;
        m_needsPurge = false;

        if (m_currentEntityCount < m_lastEntityCount)
            {
                m_needsPurge = true;
                m_lastEntityCount = 0;
                m_lastVertexCount = 0;
                m_batch.clear();
            }

        m_lastEntityCount = m_currentEntityCount;
        m_currentEntityCount = 0;
    }

void fe::spriteBatch::draw(sf::RenderTarget &app, sf::RenderStates states)
    {
        app.draw(m_batch, states);
    }
