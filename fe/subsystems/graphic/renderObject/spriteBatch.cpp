#include "spriteBatch.hpp"
#include "../../physics/transformable.hpp"
#include "../../../debug/profiler.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::spriteBatch::spriteBatch() : m_objectCount(0)
    {
    }

void fe::spriteBatch::add(const fe::renderObject *entity, fe::transformable &transform, unsigned int &index, unsigned int indexOffset)
    {
        const fe::matrix3d &matrix = transform.getMatrix();
        fe::lightVector2d transform0 = matrix.transformPoint({   entity->m_verticies[0],  entity->m_verticies[1] });
        fe::lightVector2d transform1 = matrix.transformPoint({   entity->m_verticies[0] + entity->m_verticies[3],  entity->m_verticies[1] });
        fe::lightVector2d transform2 = matrix.transformPoint({   entity->m_verticies[0] + entity->m_verticies[3],  entity->m_verticies[1] + entity->m_verticies[3] });
        fe::lightVector2d transform3 = matrix.transformPoint({   entity->m_verticies[0],  entity->m_verticies[1] + entity->m_verticies[3] });
        sf::Color entColour = sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]);
        fe::lightVector2d texCoord(entity->m_texCoords[0], entity->m_texCoords[1]);

        m_batch[index + 0 + indexOffset].position.x = transform0.x;
        m_batch[index + 0 + indexOffset].position.y = transform0.y;
        m_batch[index + 0 + indexOffset].color = entColour;
        m_batch[index + 0 + indexOffset].texCoords.x = texCoord.x;
        m_batch[index + 0 + indexOffset].texCoords.y = texCoord.y;

        m_batch[index + 1 + indexOffset].position.x = transform1.x;
        m_batch[index + 1 + indexOffset].position.y = transform1.y;
        m_batch[index + 1 + indexOffset].color = entColour;
        m_batch[index + 1 + indexOffset].texCoords.x = texCoord.x;
        m_batch[index + 1 + indexOffset].texCoords.y = texCoord.y;

        m_batch[index + 2 + indexOffset].position.x = transform2.x;
        m_batch[index + 2 + indexOffset].position.y = transform2.y;
        m_batch[index + 2 + indexOffset].color = entColour;
        m_batch[index + 2 + indexOffset].texCoords.x = texCoord.x;
        m_batch[index + 2 + indexOffset].texCoords.y = texCoord.y;

        m_batch[index + 3 + indexOffset].position.x = transform3.x;
        m_batch[index + 3 + indexOffset].position.y = transform3.y;
        m_batch[index + 3 + indexOffset].color = entColour;
        m_batch[index + 3 + indexOffset].texCoords.x = texCoord.x;
        m_batch[index + 3 + indexOffset].texCoords.y = texCoord.y;

        index += 4;
        m_objectCount++;
    }

void fe::spriteBatch::clear()
    {
        m_objectCount = 0;
    }

void fe::spriteBatch::draw(sf::RenderTarget &app, sf::RenderStates states)
    {
        app.draw(m_batch, (m_objectCount - 1) * 4, sf::PrimitiveType::Quads, states);
    }
