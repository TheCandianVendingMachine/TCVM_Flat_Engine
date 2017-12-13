#include "toggleButton.hpp"
#include "panel.hpp"
#include "../subsystems/messaging/gameEvent.hpp"
#include "../subsystems/messaging/eventSender.hpp"
#include "../engine.hpp"

fe::gui::toggleButton::toggleButton(const fe::Vector2d &size, const std::function<void()> &callback) : fe::gui::button(size, callback), m_toggle(false)
    {
    }

void fe::gui::toggleButton::update()
    {
        if (m_parentPanel->mouseHover(getPosition(), m_size) && m_parentPanel->getMousePressed() && !m_pressed)
                {
                    m_pressed = true;
                    m_toggle = !m_toggle;
                    if (m_toggle)
                        {
                            m_callback();
                            fe::gameEvent event(m_event, 1);
                            event.args[0].arg.TYPE_VOIDP = this;
                            event.args[0].argType = gameEventArgument::type::TYPE_VOIDP;
                            fe::engine::get().getEventSender().sendEngineEvent(event, m_event);
                            setActive(true);
                        }
                    else
                        {
                            fe::gameEvent event(m_extraEvent, 1);
                            event.args[0].arg.TYPE_VOIDP = this;
                            event.args[0].argType = gameEventArgument::type::TYPE_VOIDP;
                            fe::engine::get().getEventSender().sendEngineEvent(event, m_extraEvent);
                            setActive(false);
                        }
                }
            else if (!m_parentPanel->getMousePressed() && m_pressed)
                {
                    m_pressed = false;
                }

            if (m_parentPanel->mouseHover(getPosition(), m_size) || m_toggle)
                {
                    setActive(true);
                }
            else
                {
                    setActive(false);
                }
    }

void fe::gui::toggleButton::setToggle(bool value)
    {
        m_toggle = value;
        setActive(m_toggle);
        if (m_toggle)
            {
                m_callback();
            }
    }
