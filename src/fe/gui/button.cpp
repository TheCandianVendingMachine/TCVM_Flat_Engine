#include "button.hpp"
#include "panel.hpp"
#include "../engine.hpp"
#include "../subsystems/messaging/eventSender.hpp"

void fe::gui::button::drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix)
    {
        m_shape[0].position = matrix.transformPoint({0.f, 0.f}).convertToSfVec2();
        m_shape[1].position = matrix.transformPoint({m_size.x, 0.f}).convertToSfVec2();
        m_shape[2].position = matrix.transformPoint({m_size.x, m_size.y}).convertToSfVec2();
        m_shape[3].position = matrix.transformPoint({0.f, m_size.y}).convertToSfVec2();

        target.draw(m_shape, m_texture);
    }

fe::gui::button::button(const fe::Vector2d &size, const std::function<void()> &press, const std::function<void()> &release) :
    m_pressCallback(press),
    m_releaseCallback(release)
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
                m_pressCallback();
                fe::gameEvent event(m_event, 1);
                event.args[0].arg.TYPE_VOIDP = this;
                event.args[0].argType = gameEventArgument::type::TYPE_VOIDP;
                fe::engine::get().getEventSender().sendEngineEvent(event, m_event);
            }
        else if (!m_parentPanel->getMousePressed() && m_pressed)
            {
                m_pressed = false;
                m_releaseCallback();

                fe::gameEvent event(m_extraEvent, 1);
                event.args[0].arg.TYPE_VOIDP = this;
                event.args[0].argType = gameEventArgument::type::TYPE_VOIDP;
                fe::engine::get().getEventSender().sendEngineEvent(event, m_extraEvent);
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

void fe::gui::button::setReleaseCallback(std::function<void()> callback)
    {
        m_releaseCallback = callback;
    }

void fe::gui::button::setPressCallback(std::function<void()> callback)
    {
        m_pressCallback = callback;
    }
