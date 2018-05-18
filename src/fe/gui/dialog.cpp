#include "fe/gui/dialog.hpp"
#include <algorithm>
#include <SFML/Window/Event.hpp>

void fe::gui::dialog::addPoint(fe::lightVector2d point)
    {
        addPoint(point.x, point.y);
    }

void fe::gui::dialog::addPoint(float x, float y)
    {
        m_dialogPolygon.addPoint({ x, y });
        m_polygonNeedsCreation = true;
    }

void fe::gui::dialog::attach(dialog *element)
    {
        m_containedDialogs.push_back(element);
    }

void fe::gui::dialog::detach(dialog *element)
    {
        m_containedDialogs.erase(std::remove(m_containedDialogs.begin(), m_containedDialogs.end(), element), m_containedDialogs.end());
    }

bool fe::gui::dialog::isElementAttached(const dialog *element) const
    {
        return std::find(m_containedDialogs.begin(), m_containedDialogs.end(), element) != m_containedDialogs.end();
    }

bool fe::gui::dialog::isAttachedTo(const dialog *element) const
    {
        return element->isElementAttached(this);
    }

void fe::gui::dialog::kill()
    {
        m_killed = true;
    }

bool fe::gui::dialog::isKilled() const
    {
        return m_killed;
    }

void fe::gui::dialog::setDrawOrder(int focus)
    {
        m_drawOrder = focus;
    }

int fe::gui::dialog::getDrawOrder() const
    {
        return m_drawOrder;
    }

void fe::gui::dialog::setState(dialogStates state)
    {
        onStateChange(m_dialogState, state);
        m_dialogState = state;
    }

fe::gui::dialogStates fe::gui::dialog::getState() const
    {
        return m_dialogState;
    }

void fe::gui::dialog::handleEvent(const sf::Event &event)
    {
        if (m_polygonNeedsCreation)
            {
                m_dialogPolygon.createPolygon();
            }

        bool stateUpdate = false;
        dialogStates current = m_dialogState;
        dialogStates next = dialogStates::NONE;

        switch (event.type)
            {
                case sf::Event::MouseMoved:
                    if (current != dialogStates::DISABLED && current != dialogStates::PRESSED)
                        {
                            if (m_dialogPolygon.pointInPolygon(event.mouseMove.x, event.mouseMove.y))
                                {
                                    next = dialogStates::HIGHLIGHTED;
                                    stateUpdate = true;
                                }
                            else
                                {
                                    next = dialogStates::ACTIVE;
                                    stateUpdate = true;
                                }
                        }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (current != dialogStates::DISABLED)
                        {
                            if (current == dialogStates::HIGHLIGHTED)
                                {
                                    next = dialogStates::PRESSED;
                                    stateUpdate = true;
                                }
                        }
                    break;
                case sf::Event::MouseButtonReleased:
                    if (current != dialogStates::DISABLED)
                        {
                            if (current == dialogStates::PRESSED)
                                {
                                    next = dialogStates::ACTIVE;
                                    stateUpdate = true;
                                }
                        }
                    break;
                default:
                    break;
            }

        if (stateUpdate)
            {
                if (current != next)
                    {
                        onStateChange(current, next);
                    }
                m_dialogState = next;
            }

        handleWindowEvent(event);
    }

void fe::gui::dialog::draw(sf::RenderTarget &target, fe::gui::guiBatch &guiBatch)
    {
        std::sort(m_containedDialogs.begin(), m_containedDialogs.end(), [] (fe::gui::dialog *a, fe::gui::dialog *b) { return a->getDrawOrder() < b->getDrawOrder(); });

        for (auto &child : m_containedDialogs)
            {
                child->draw(target, guiBatch);
            }

        drawDialogElements(guiBatch);
        drawDialogText(target);
    }

fe::gui::dialog &fe::gui::dialog::operator=(dialog &rhs)
    {
        if (&rhs != this)
            {
                m_containedDialogs = rhs.m_containedDialogs;
                m_killed = rhs.m_killed;
            }
        return *this;
    }
