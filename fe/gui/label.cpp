#include "label.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void fe::gui::label::drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix)
    {
        auto localMatrix = getMatrix() * matrix;
        m_text.setPosition(localMatrix.transformPoint(getPosition()).convertToSfVec2());
        target.draw(m_text);
    }

fe::gui::label::label(const sf::Font &font, const char *text)
    {
        m_text.setFont(font);
        m_text.setString(text);

        setCharacterSize(m_text.getCharacterSize());
    }

void fe::gui::label::setPosition(fe::Vector2d pos)
    {
        m_textPosition = pos;
    }

fe::Vector2d fe::gui::label::getPosition() const
    {
        return m_textPosition;
    }

void fe::gui::label::setCharacterSize(float point)
    {
        m_text.setCharacterSize(point);

        sf::FloatRect textRect = m_text.getLocalBounds();
        m_text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

        setSize({ m_text.getGlobalBounds().width, m_text.getGlobalBounds().height });
    }

float fe::gui::label::getCharacterSize() const
    {
        return m_text.getCharacterSize();
    }

void fe::gui::label::setPixelSize(float pixel)
    {
        m_text.setCharacterSize(pixel * (72.f / 96.f));

        sf::FloatRect textRect = m_text.getLocalBounds();
        m_text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

        setSize({ m_text.getGlobalBounds().width, m_text.getGlobalBounds().height });
    }

float fe::gui::label::getPixelSize() const
    {
        return (m_text.getCharacterSize() * 96.f) / 72.f;
    }

void fe::gui::label::setString(const char *str)
    {
        m_text.setString(str);
        setSize({ m_text.getGlobalBounds().width, m_text.getGlobalBounds().height });
    }

std::string fe::gui::label::getString() const
    {
        return m_text.getString();
    }

void fe::gui::label::setColour(sf::Color colour)
    {
        m_text.setFillColor(colour);
    }

sf::Color fe::gui::label::getColour()
    {
        return m_text.getFillColor();
    }
