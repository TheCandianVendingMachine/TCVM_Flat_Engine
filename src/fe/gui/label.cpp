#include "fe/gui/label.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void fe::gui::label::drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix)
    {
        m_text.setPosition(matrix.transformPoint(getPosition()).convertToSfVec2());
        target.draw(m_text, m_texture);
    }

fe::gui::label::label(const sf::Font &font, const char *text)
    {
        m_text.setFont(font);
        m_text.setString(text);

        setCharacterSize(m_text.getCharacterSize());
    }

fe::gui::label::label(const sf::Font &font, const std::string &text)
    {
        m_text.setFont(font);
        m_text.setString(text);

        setCharacterSize(m_text.getCharacterSize());
    }

void fe::gui::label::fitToParent()
    {
        float positionRelative = (m_textPosition.x / m_parentElement->getSize().x) - 0.5f;
        int sign = (0 <= positionRelative) - (positionRelative < 0); // 0 is considered positive

        for (unsigned int i = m_text.getCharacterSize(); i > 0; i--)
            {
                setCharacterSize(i);
                float dx = (m_textPosition.x + (m_text.getLocalBounds().width / 2.f)) - m_parentElement->getSize().x;
                dx *= sign;
                if (dx <= 0.f)
                    {
                        break;
                    }
            }
    }

void fe::gui::label::setPositionRelative(fe::Vector2d pos)
    {
        setPositionRelative(pos.x, pos.y);
    }

void fe::gui::label::setPositionRelative(float x, float y)
    {
        setPosition(m_parentElement->getSize().x * x, m_parentElement->getSize().y * y);
    }

void fe::gui::label::setPosition(fe::Vector2d pos)
    {
        setPosition(pos.x, pos.y);
    }

void fe::gui::label::setPosition(float x, float y)
    {
        m_textPosition.x = x;
        m_textPosition.y = y;
    }

fe::Vector2d fe::gui::label::getPosition() const
    {
        return m_textPosition;
    }

void fe::gui::label::setCharacterSize(unsigned int point)
    {
        m_text.setCharacterSize(point);

        sf::FloatRect textRect = m_text.getLocalBounds();
        m_text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

        setSize({ m_text.getLocalBounds().width, m_text.getLocalBounds().height });
    }

unsigned int fe::gui::label::getCharacterSize() const
    {
        return m_text.getCharacterSize();
    }

void fe::gui::label::setPixelSize(float pixel)
    {
        m_text.setCharacterSize(static_cast<unsigned int>(std::ceilf(pixel * (72.f / 96.f))));

        sf::FloatRect textRect = m_text.getLocalBounds();
        m_text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

        setSize({ m_text.getGlobalBounds().width, m_text.getLocalBounds().height });
    }

float fe::gui::label::getPixelSize() const
    {
        return (m_text.getCharacterSize() * 96.f) / 72.f;
    }

void fe::gui::label::setString(const char *str)
    {
        m_text.setString(str);
        setSize({ m_text.getLocalBounds().width, m_text.getLocalBounds().height });
    }

void fe::gui::label::setString(const std::string &str)
    {
        m_text.setString(sf::String(str));
        setSize({ m_text.getLocalBounds().width, m_text.getLocalBounds().height });
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

const sf::Text &fe::gui::label::getText()
    {
        return m_text;
    }
