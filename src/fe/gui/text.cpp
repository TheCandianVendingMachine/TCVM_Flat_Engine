#include "fe/gui/text.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/resourceManager/resourceManager.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void fe::gui::text::drawDialogText(sf::RenderTarget &target, const fe::matrix3d &drawMatrix)
    {
        m_renderText.setFillColor(getDrawColour());

        m_renderText.setPosition(getPosition().convertToSfVec2());
        m_renderText.setRotation(getRotation() * 180.f / 3.14159f);

        target.draw(m_renderText);
    }

void fe::gui::text::updateOrigin()
    {
        sf::FloatRect textRect = m_renderText.getLocalBounds();
        m_renderText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
    }

fe::gui::text::text(const sf::Font *data)
    {
        m_fontData = data;
        m_renderText.setFont(*data);
        updateOrigin();
    }

void fe::gui::text::setText(const char *text)
    {
        m_renderText.setString(text);
        updateOrigin();
    }

void fe::gui::text::setText(const std::string &text)
    {
        setText(text.c_str());
    }

void fe::gui::text::setCharacterSize(unsigned int size)
    {
        m_renderText.setCharacterSize(size);
        updateOrigin();
    }
