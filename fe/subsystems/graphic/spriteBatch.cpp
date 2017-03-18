#include "spriteBatch.hpp"
#include "../../entity/transformable.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::spriteBatch::spriteBatch() : m_batch(sf::PrimitiveType::Quads), m_maxVertexCount(0), m_lastVertexCount(0)
    {
    }

void fe::spriteBatch::add(const sf::VertexArray &entity, fe::transformable &transform)
    {
        for (unsigned int i = 0; i < entity.getVertexCount(); i++)
            {
                if (m_index >= m_lastVertexCount) 
                    {
                        m_batch.append(sf::Vertex(transform.getMatrix().transformPoint(entity[i].position).convertToSfVec2(), entity[i].color, entity[i].texCoords));
                    }
                else
                    {
                        m_batch[m_index].position = transform.getMatrix().transformPoint(entity[i].position).convertToSfVec2();
                        m_batch[m_index].color = entity[i].color;
                        m_batch[m_index].texCoords = entity[i].texCoords;
                    }
                m_maxVertexCount += 1;
                m_index++;
            }
    }

void fe::spriteBatch::clear()
    {
        m_lastVertexCount = m_lastVertexCount > m_maxVertexCount ? m_lastVertexCount : m_maxVertexCount;
        m_maxVertexCount = 0;
        m_index = 0;
    }

void fe::spriteBatch::draw(sf::RenderTarget &app)
    {
        app.draw(m_batch);
        clear();
    }
