#include "spriteBatch.hpp"
#include "../../physics/transformable.hpp"
#include "../../../debug/profiler.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::spriteBatch::spriteBatch()
    {
    }

void fe::spriteBatch::add(const fe::renderObject *entity, const fe::matrix3d *transform, unsigned int &index)
    {
        if (!transform) 
            {
                fe::lightVector2d transform0 = { entity->m_verticies[0],  entity->m_verticies[1] };
                fe::lightVector2d transform1 = { entity->m_verticies[0] + entity->m_verticies[3],  entity->m_verticies[1] };
                fe::lightVector2d transform2 = { entity->m_verticies[0] + entity->m_verticies[3],  entity->m_verticies[1] + entity->m_verticies[3] };
                fe::lightVector2d transform3 = { entity->m_verticies[0],  entity->m_verticies[1] + entity->m_verticies[3] };
                sf::Color entColour = sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]);
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
            }
        else
            {
                const fe::matrix3d &matrix = *transform;
                fe::lightVector2d transform0 = matrix.transformPoint({   entity->m_verticies[0],  entity->m_verticies[1] });
                fe::lightVector2d transform1 = matrix.transformPoint({   entity->m_verticies[0] + entity->m_verticies[3],  entity->m_verticies[1] });
                fe::lightVector2d transform2 = matrix.transformPoint({   entity->m_verticies[0] + entity->m_verticies[3],  entity->m_verticies[1] + entity->m_verticies[3] });
                fe::lightVector2d transform3 = matrix.transformPoint({   entity->m_verticies[0],  entity->m_verticies[1] + entity->m_verticies[3] });
                sf::Color entColour = sf::Color(entity->m_vertColour[0], entity->m_vertColour[1], entity->m_vertColour[2], entity->m_vertColour[3]);
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
            }

        index += 4;
    }

void fe::spriteBatch::clear()
    {}

void fe::spriteBatch::draw(sf::RenderTarget &app, sf::RenderStates states, unsigned int objectCount)
    {
        app.draw(m_batch, objectCount * 4, sf::PrimitiveType::Quads, states);
    }
