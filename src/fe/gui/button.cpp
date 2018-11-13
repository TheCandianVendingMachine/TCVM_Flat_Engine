#include "fe/gui/button.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/engine.hpp"
#include "fe/objectManagement/str.hpp"
#include "fe/gui/dialogStates.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include "..\..\..\include\fe\gui\button.hpp"

void fe::gui::button::onStateChange(dialogStates previous, dialogStates next)
    {
        fe::gameEvent buttonEvent(m_eventOnRelease, 1);
        buttonEvent.args[0].arg.TYPE_VOIDP = this;
        buttonEvent.args[0].argType = fe::gameEventArgument::type::TYPE_VOIDP;

        switch (previous)
            {
                case fe::gui::dialogStates::PRESSED:
                    fe::engine::get().getEventSender().send(buttonEvent, m_eventOnRelease);
                    break;
                default:
                    break;
            }

        switch (next)
            {
                case fe::gui::dialogStates::ACTIVE:
                    setDrawColour(m_colourOnActive);
                    break;
                case fe::gui::dialogStates::DISABLED:
                    setDrawColour(m_colourOnDeactive);
                    break;
                case fe::gui::dialogStates::HIGHLIGHTED:
                    setDrawColour(m_colourOnHighlight);
                    break;
                case fe::gui::dialogStates::PRESSED:
                    setDrawColour(m_colourOnPress);
                    fe::engine::get().getEventSender().send(buttonEvent, m_eventOnPress);
                    break;
                default:
                    break;
            }
    }

void fe::gui::button::drawDialogElements(sf::RenderTarget &target, const fe::transformable &drawMatrix)
    {
        FE_ENGINE_PROFILE("gui_button", "draw");
        drawPolygon(getControlPolygon(), const_cast<fe::transformable&>(drawMatrix).getMatrix(), getDrawColour());
        drawToScreen(target, drawMatrix);
        FE_END_PROFILE;
    }

fe::gui::button::button(std::initializer_list<fe::lightVector2d> shape) :
    m_eventOnPress(FE_STR("gui_button_on_press")),
    m_eventOnRelease(FE_STR("gui_button_on_release")),
    m_colourOnActive(sf::Color::White),
    m_colourOnDeactive(sf::Color::White),
    m_colourOnHighlight(sf::Color::White),
    m_colourOnPress(sf::Color::White)
    {
        for (auto &point : shape)
            {
                addPoint(point);
            }
        getControlPolygon().createPolygon();
    }

void fe::gui::button::setEventOnPress(fe::str event)
    {
        m_eventOnPress = event;
    }

void fe::gui::button::setEventOnRelease(fe::str event)
    {
        m_eventOnRelease = event;
    }

void fe::gui::button::setColourOnActive(sf::Color colour)
    {
        m_colourOnActive = colour;
    }

void fe::gui::button::setColourOnDeactive(sf::Color colour)
    {
        m_colourOnDeactive = colour;
    }

void fe::gui::button::setColourHighlight(sf::Color colour)
    {
        m_colourOnHighlight = colour;
    }

void fe::gui::button::setColourOnPress(sf::Color colour)
    {
        m_colourOnPress = colour;
    }
