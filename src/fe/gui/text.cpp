#include "fe/gui/text.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/resourceManager/resourceManager.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void fe::gui::text::drawDialogText(sf::RenderTarget &target, const fe::transformable &drawMatrix)
    {
        FE_ENGINE_PROFILE("gui_text", "draw");
        m_renderText.setFillColor(getDrawColour());

        sf::RenderStates s;
        s.transform.translate(target.mapPixelToCoords(sf::Vector2i(getPosition().convertToSfVec2())));
        s.transform.scale(drawMatrix.getScale().x, drawMatrix.getScale().y);
        s.transform.rotate(drawMatrix.getRotation() * 180.f / 3.14159f);
        target.draw(m_renderText, s);
        FE_END_PROFILE;
    }

void fe::gui::text::updateOrigin()
    {
        sf::FloatRect textRect = m_renderText.getLocalBounds();
        m_renderText.setOrigin(textRect.left, textRect.top);
    }

fe::gui::text::text(const sf::Font *data)
    {
        m_fontData = data;
        m_renderText.setFont(*data);
        updateOrigin();
    }

fe::lightVector2d fe::gui::text::getSize() const
    {
        sf::FloatRect textRect = m_renderText.getLocalBounds();
        return fe::transformable::getSize(textRect.left + textRect.width, textRect.top + textRect.height);
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

unsigned int fe::gui::text::getCharacterSize() const
    {
        return m_renderText.getCharacterSize();
    }
