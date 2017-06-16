#include "square.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void fe::gui::square::drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix)
    {
        m_shape[0].position = matrix.transformPoint({ 0.f, 0.f }).convertToSfVec2();
        m_shape[1].position = matrix.transformPoint({ m_size.x, 0.f }).convertToSfVec2();
        m_shape[2].position = matrix.transformPoint({ m_size.x, m_size.y }).convertToSfVec2();
        m_shape[3].position = matrix.transformPoint({ 0.f, m_size.y }).convertToSfVec2();

        target.draw(m_shape, m_texture);
    }

fe::gui::square::square()
    {
        m_shape = sf::VertexArray(sf::PrimitiveType::Quads, 4);
    }

fe::gui::square::square(fe::Vector2d size, sf::Color colour)
    {
        m_shape = sf::VertexArray(sf::PrimitiveType::Quads, 4);

        setSize(size);
        setColour(colour);

        m_shape[0].position = { 0, 0 };
        m_shape[1].position = { size.x, 0 };
        m_shape[2].position = { size.x, size.y };
        m_shape[3].position = { 0, size.y };
    }

fe::gui::square::square(sf::Texture *texture, fe::Vector2d texCoords)
    {
        m_shape = sf::VertexArray(sf::PrimitiveType::Quads, 4);
        setTexture(texture, texCoords);
    }

void fe::gui::square::setTexture(sf::Texture *texture, fe::Vector2d texCoords)
    {
        m_texture = texture;
        m_shape[0].texCoords = { 0.f, 0.f };
        m_shape[1].texCoords = { texCoords.x, 0.f };
        m_shape[2].texCoords = { texCoords.x, texCoords.y };
        m_shape[3].texCoords = { 0.f, texCoords.y };
    }
