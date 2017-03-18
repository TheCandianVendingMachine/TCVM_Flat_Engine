#include "spriteBatch.hpp"
#include "../../entity/transformable.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::spriteBatch::spriteBatch() : m_batch(sf::PrimitiveType::Quads)
    {
    }

void fe::spriteBatch::add(const sf::VertexArray &entity, fe::transformable &transform)
    {
        for (unsigned int i = 0; i < entity.getVertexCount(); i++)
            {
                m_batch.append(sf::Vertex(transform.getMatrix().transformPoint(entity[i].position).convertToSfVec2(), entity[i].color, entity[i].texCoords));
            }
    }

void fe::spriteBatch::clear()
    {
        m_batch.clear();
    }

void fe::spriteBatch::draw(sf::RenderTarget &app)
    {
        app.draw(m_batch);
        clear();
    }
