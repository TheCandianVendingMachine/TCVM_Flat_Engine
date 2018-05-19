#include "fe/gui/control.hpp"
#include "fe/gui/guiBatch.hpp"
#include <SFML/Window/Event.hpp>

void fe::gui::control::drawDialogElements(fe::gui::guiBatch &target)
    {
        target.add(m_controlPolygon, m_drawColour);
    }

void fe::gui::control::addPoint(fe::lightVector2d point)
    {
        m_controlPolygon.addPoint(point);
        m_polygonNeedsCreation = true;
    }

void fe::gui::control::addPoint(float x, float y)
    {
        addPoint(x, y);
    }

fe::gui::control::control() :
    m_polygonNeedsCreation(false),
    m_drawColour(sf::Color::White)
    {
    }

void fe::gui::control::setDrawColour(sf::Color colour)
    {
        m_drawColour = colour;
    }

sf::Color fe::gui::control::getDrawColour() const
    {
        return m_drawColour;
    }

void fe::gui::control::handleEvent(const sf::Event &event)
    {
        if (m_polygonNeedsCreation)
            {
                m_controlPolygon.createPolygon();
                m_polygonNeedsCreation = false;
            }

        dialogStates current = getState();

        switch (event.type)
            {
                case sf::Event::MouseMoved:
                    if (current != dialogStates::DISABLED && current != dialogStates::PRESSED)
                        {
                            if (m_controlPolygon.pointInPolygon(event.mouseMove.x, event.mouseMove.y))
                                {
                                    setState(dialogStates::HIGHLIGHTED);
                                }
                            else
                                {
                                    setState(dialogStates::ACTIVE);
                                }
                        }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (current != dialogStates::DISABLED)
                        {
                            if (current == dialogStates::HIGHLIGHTED)
                                {
                                    setState(dialogStates::PRESSED);
                                }
                        }
                    break;
                case sf::Event::MouseButtonReleased:
                    if (current != dialogStates::DISABLED)
                        {
                            if (current == dialogStates::PRESSED)
                                {
                                    setState(dialogStates::ACTIVE);
                                }
                        }
                    break;
                default:
                    break;
            }

        handleWindowEvent(event);
    }
