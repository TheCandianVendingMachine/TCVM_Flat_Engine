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
        const fe::matrix3d &matrix = text->m_tempTransform.getMatrix();
        sf::Color entColour = sf::Color(text->m_vertColour[0], text->m_vertColour[1], text->m_vertColour[2], text->m_vertColour[3]);
        fe::lightVector2d texCoord(text->m_texCoords[0], text->m_texCoords[1]);

        text->computeVerticies();
        for (unsigned int i = 0; i < text->m_strSize * 4; i += 4)
            {
                fe::lightVector2d transform0 = matrix.transformPoint({  text->m_verticies[i + 0],
                                                                        text->m_verticies[i + 1] });

                fe::lightVector2d transform1 = matrix.transformPoint({  text->m_verticies[i + 0] + text->m_verticies[i + 2],
                                                                        text->m_verticies[i + 1] });

                fe::lightVector2d transform2 = matrix.transformPoint({  text->m_verticies[i + 0] + text->m_verticies[i + 2],
                                                                        text->m_verticies[i + 1] + text->m_verticies[i + 3] });

                fe::lightVector2d transform3 = matrix.transformPoint({  text->m_verticies[i + 0],
                                                                        text->m_verticies[i + 1] + text->m_verticies[i + 3] });

                float tX = text->m_charTexCoords[i + 0];
                float tY = text->m_charTexCoords[i + 1];
                float tW = text->m_charTexCoords[i + 2];
                float tH = text->m_charTexCoords[i + 3];

                m_batch[index + 0].position.x = transform0.x;
                m_batch[index + 0].position.y = transform0.y;
                m_batch[index + 0].color = entColour;
                m_batch[index + 0].texCoords.x = tX;
                m_batch[index + 0].texCoords.y = tY;

                m_batch[index + 1].position.x = transform1.x;
                m_batch[index + 1].position.y = transform1.y;
                m_batch[index + 1].color = entColour;
                m_batch[index + 1].texCoords.x = tX + tW;
                m_batch[index + 1].texCoords.y = tY;

                m_batch[index + 2].position.x = transform2.x;
                m_batch[index + 2].position.y = transform2.y;
                m_batch[index + 2].color = entColour;
                m_batch[index + 2].texCoords.x = tX + tW;
                m_batch[index + 2].texCoords.y = tY + tH;

                m_batch[index + 3].position.x = transform3.x;
                m_batch[index + 3].position.y = transform3.y;
                m_batch[index + 3].color = entColour;
                m_batch[index + 3].texCoords.x = tX;
                m_batch[index + 3].texCoords.y = tY + tH;

                index += 4;
            }
    }

void fe::spriteBatch::clear()
    {}

void fe::spriteBatch::draw(sf::RenderTarget &app, sf::RenderStates states, unsigned int vertexCount)
    {
        app.draw(m_batch, vertexCount, sf::PrimitiveType::Quads, states);
    }
