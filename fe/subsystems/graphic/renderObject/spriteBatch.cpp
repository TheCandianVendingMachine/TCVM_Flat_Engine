#include "spriteBatch.hpp"
#include "../../physics/transformable.hpp"
#include "../../../math/matrix.hpp"
#include "../../../debug/profiler.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::spriteBatch::spriteBatch()
    {
    }

void fe::spriteBatch::add(fe::renderObject *entity, unsigned int &index)
    {
        const fe::matrix3d &matrix = entity->m_tempTransform.getMatrix();
        fe::lightVector2d transform0 = matrix.transformPoint({  entity->m_verticies[0],                             entity->m_verticies[1] });
        fe::lightVector2d transform1 = matrix.transformPoint({  entity->m_verticies[0] + entity->m_verticies[2],    entity->m_verticies[1] });
        fe::lightVector2d transform2 = matrix.transformPoint({  entity->m_verticies[0] + entity->m_verticies[2],    entity->m_verticies[1] + entity->m_verticies[3] });
        fe::lightVector2d transform3 = matrix.transformPoint({  entity->m_verticies[0],                             entity->m_verticies[1] + entity->m_verticies[3] });
        sf::Color entColour = sf::Color(entity->m_vertColour[0],entity->m_vertColour[1], entity->m_vertColour[2],   entity->m_vertColour[3]);
        fe::lightVector2d texCoord(entity->m_texCoords[0], entity->m_texCoords[1]);

        m_batch[index + 0].position.x = transform0.x;
        m_batch[index + 0].position.y = transform0.y;
        m_batch[index + 0].color = entColour;
        m_batch[index + 0].texCoords.x = texCoord.x;
        m_batch[index + 0].texCoords.y = texCoord.y;

        m_batch[index + 1].position.x = transform1.x;
        m_batch[index + 1].position.y = transform1.y;
        m_batch[index + 1].color = entColour;
        m_batch[index + 1].texCoords.x = texCoord.x;
        m_batch[index + 1].texCoords.y = texCoord.y;

        m_batch[index + 2].position.x = transform2.x;
        m_batch[index + 2].position.y = transform2.y;
        m_batch[index + 2].color = entColour;
        m_batch[index + 2].texCoords.x = texCoord.x;
        m_batch[index + 2].texCoords.y = texCoord.y;

        m_batch[index + 3].position.x = transform3.x;
        m_batch[index + 3].position.y = transform3.y;
        m_batch[index + 3].color = entColour;
        m_batch[index + 3].texCoords.x = texCoord.x;
        m_batch[index + 3].texCoords.y = texCoord.y;

        index += 4;
    }

void fe::spriteBatch::add(fe::renderText *text, unsigned int &index)
    {
        text->computeVerticies(text->m_tempTransform.getMatrix());
        for (unsigned int i = 0; i < text->m_verticies.getVertexCount(); i++)
            {
                m_batch[index++] = text->m_verticies[i];
            }
    }

void fe::spriteBatch::clear()
    {}

void fe::spriteBatch::draw(sf::RenderTarget &app, sf::RenderStates states, unsigned int vertexCount)
    {
        app.draw(m_batch, vertexCount, sf::PrimitiveType::Quads, states);
    }
