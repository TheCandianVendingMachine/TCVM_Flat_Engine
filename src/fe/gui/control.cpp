#include "fe/gui/control.hpp"
#include "fe/debug/profiler.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

void fe::gui::control::drawPolygon(fe::polygon2d &poly, sf::RenderTarget &target, const fe::matrix3d &drawMatrix, sf::Color drawColour)
    {
        FE_ENGINE_PROFILE("gui_control", "emplace_verts");
        m_verticies.clear();
        for (unsigned int i = 0; i < poly.m_verticies.size(); i++)
            {
                m_verticies.emplace_back(fe::Vector2d(drawMatrix.transformPoint(std::forward<const fe::lightVector2d>(poly.m_verticies[i][0]))).convertToSfVec2());
                m_verticies.emplace_back(fe::Vector2d(drawMatrix.transformPoint(std::forward<const fe::lightVector2d>(poly.m_verticies[i][1]))).convertToSfVec2());
                m_verticies.emplace_back(fe::Vector2d(drawMatrix.transformPoint(std::forward<const fe::lightVector2d>(poly.m_verticies[i][2]))).convertToSfVec2());
            }
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("gui_control", "set_colour");
        for (unsigned int i = 0; i < m_verticies.size(); i++)
            {
                m_verticies[i].color = drawColour;
            }
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("gui_control", "draw_to_app");
        target.draw(m_verticies.data(), m_verticies.size(), sf::PrimitiveType::Triangles);
        FE_END_PROFILE;
    }

fe::polygon2d &fe::gui::control::getControlPolygon()
    {
        return m_controlPolygon;
    }

void fe::gui::control::addPoint(fe::lightVector2d point)
    {
        m_controlPolygon.addPoint(point);
    }

void fe::gui::control::addPoint(float x, float y)
    {
        addPoint(fe::lightVector2d(x, y));
    }

fe::gui::control::control() :
    m_drawColour(sf::Color::White)
    {
        setState(dialogStates::ACTIVE);
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
        dialogStates current = getState();

        switch (event.type)
            {
                case sf::Event::MouseMoved:
                    if (current != dialogStates::DISABLED && current != dialogStates::PRESSED)
                        {
                            fe::lightVector2d realMousePos(event.mouseMove.x, event.mouseMove.y);
                            realMousePos = getDrawMatrix().getMatrix().transformPointToLocalSpace(std::forward<const fe::lightVector2d>(realMousePos));

                            if (m_controlPolygon.pointInPolygon(realMousePos.x, realMousePos.y))
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
                            fe::lightVector2d realMousePos(event.mouseButton.x, event.mouseButton.y);
                            realMousePos = getDrawMatrix().getMatrix().transformPointToLocalSpace(std::forward<const fe::lightVector2d>(realMousePos));

                            if (m_controlPolygon.pointInPolygon(realMousePos.x, realMousePos.y))
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
                                    fe::lightVector2d realMousePos(event.mouseButton.x, event.mouseButton.y);
                                    realMousePos = getDrawMatrix().getMatrix().transformPointToLocalSpace(std::forward<const fe::lightVector2d>(realMousePos));

                                    if (m_controlPolygon.pointInPolygon(realMousePos.x, realMousePos.y))
                                        {
                                            setState(dialogStates::HIGHLIGHTED);
                                        }
                                    else
                                        {
                                            setState(dialogStates::ACTIVE);
                                        }
                                }
                        }
                    break;
                default:
                    break;
            }

        handleWindowEvent(event);
    }
