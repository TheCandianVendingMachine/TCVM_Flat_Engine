#include "spriteBatch.hpp"
#include "../../physics/transformable.hpp"
#include "../../../debug/profiler.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::spriteBatch::spriteBatch() : m_batch(sf::PrimitiveType::Quads), m_maxVertexCount(0), m_lastVertexCount(0)
    {
    }

void fe::spriteBatch::add(const fe::renderObject *entity, fe::transformable &transform)
    {
        m_currentEntityCount++;
        const fe::matrix3d &matrix = transform.getMatrix();
        fe::lightVector2d transform0 = matrix.transformPoint({ entity->m_verticies[0],  entity->m_verticies[1] });
        fe::lightVector2d transform1 = matrix.transformPoint({ entity->m_verticies[0] + entity->m_verticies[3],  entity->m_verticies[1] });
        fe::lightVector2d transform2 = matrix.transformPoint({ entity->m_verticies[0] + entity->m_verticies[3],  entity->m_verticies[1] + entity->m_verticies[3] });
        fe::lightVector2d transform3 = matrix.transformPoint({ entity->m_verticies[0],  entity->m_verticies[1] + entity->m_verticies[3] });
        sf::Color entColour = sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]);
        fe::lightVector2d texCoord(entity->m_texCoords[0], entity->m_texCoords[1]);

        if (m_needsPurge || m_index >= m_lastVertexCount) 
            {
                m_batch.append(sf::Vertex(transform0.convertToSfVec2(), entColour, texCoord.convertToSfVec2()));
                m_batch.append(sf::Vertex(transform1.convertToSfVec2(), entColour, texCoord.convertToSfVec2()));
                m_batch.append(sf::Vertex(transform2.convertToSfVec2(), entColour, texCoord.convertToSfVec2()));
                m_batch.append(sf::Vertex(transform3.convertToSfVec2(), entColour, texCoord.convertToSfVec2()));
            }
        else
            {
                m_batch[m_index + 0].position.x = transform0.x;
                m_batch[m_index + 0].position.y = transform0.y;
                m_batch[m_index + 0].color = entColour;
                m_batch[m_index + 0].texCoords.x = texCoord.x;
                m_batch[m_index + 0].texCoords.y = texCoord.y;

                m_batch[m_index + 1].position.x = transform1.x;
                m_batch[m_index + 1].position.y = transform1.y;
                m_batch[m_index + 1].color = entColour;
                m_batch[m_index + 1].texCoords.x = texCoord.x;
                m_batch[m_index + 1].texCoords.y = texCoord.y;

                m_batch[m_index + 2].position.x = transform2.x;
                m_batch[m_index + 2].position.y = transform2.y;
                m_batch[m_index + 2].color = entColour;
                m_batch[m_index + 2].texCoords.x = texCoord.x;
                m_batch[m_index + 2].texCoords.y = texCoord.y;

                m_batch[m_index + 3].position.x = transform3.x;
                m_batch[m_index + 3].position.y = transform3.y;
                m_batch[m_index + 3].color = entColour;
                m_batch[m_index + 3].texCoords.x = texCoord.x;
                m_batch[m_index + 3].texCoords.y = texCoord.y;
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
