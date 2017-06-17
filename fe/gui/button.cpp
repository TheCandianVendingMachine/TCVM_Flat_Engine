#include "button.hpp"
#include "panel.hpp"

void fe::gui::button::drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix)
    {
        m_shape[0].position = matrix.transformPoint({0.f, 0.f}).convertToSfVec2();
        m_shape[1].position = matrix.transformPoint({m_size.x, 0.f}).convertToSfVec2();
        m_shape[2].position = matrix.transformPoint({m_size.x, m_size.y}).convertToSfVec2();
        m_shape[3].position = matrix.transformPoint({0.f, m_size.y}).convertToSfVec2();

        target.draw(m_shape);
    }

fe::gui::button::button(const fe::Vector2d &size, const std::function<void()> &callback) : m_callback(callback)
    {
        m_shape.setPrimitiveType(sf::PrimitiveType::Quads);
        m_shape.resize(4);

        //m_size = size;
        setSize(size);
        setActiveColour(sf::Color(255, 0, 0, 255));
        setInactiveColour(sf::Color(255, 0, 0, 220));
    }

void fe::gui::button::handleEvent(const sf::Event &event)
    {
    }

void fe::gui::button::update()
    {
        if (m_parentPanel->mouseHover(getPosition(), m_size) && m_parentPanel->getMousePressed() && !m_pressed)
            {
                m_pressed = true;
                m_callback();
            }
        else if (!m_parentPanel->getMousePressed() && m_pressed)
            {
                m_pressed = false;
            }

        if (m_parentPanel->mouseHover(getPosition(), m_size))
            {
                setActive(true);
            }
        else
            {
                setActive(false);
            }
    }
