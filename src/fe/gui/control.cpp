#include "fe/gui/control.hpp"
#include "fe/debug/profiler.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

void fe::gui::control::drawPolygon(fe::polygon2d &poly, const fe::matrix3d &drawMatrix, sf::Color drawColour)
    {
        FE_ENGINE_PROFILE("gui_control", "emplace_verts");
        m_vertexCount += poly.m_verticies.size() * 3;
        for (unsigned int i = 0; i < poly.m_verticies.size(); i++)
            {
                if (m_vertexCount > m_verticies.size())
                    {
                        m_verticies.emplace_back(fe::Vector2d(drawMatrix.transformPoint(std::move(poly.m_verticies[i][0]))).convertToSfVec2(), drawColour);
                        m_verticies.emplace_back(fe::Vector2d(drawMatrix.transformPoint(std::move(poly.m_verticies[i][1]))).convertToSfVec2(), drawColour);
                        m_verticies.emplace_back(fe::Vector2d(drawMatrix.transformPoint(std::move(poly.m_verticies[i][2]))).convertToSfVec2(), drawColour);
                    }
                else
                    {
                        m_verticies[m_activeVertexCount + 0].position = fe::Vector2d(drawMatrix.transformPoint(std::move(poly.m_verticies[i][0]))).convertToSfVec2();
                        m_verticies[m_activeVertexCount + 1].position = fe::Vector2d(drawMatrix.transformPoint(std::move(poly.m_verticies[i][1]))).convertToSfVec2();
                        m_verticies[m_activeVertexCount + 2].position = fe::Vector2d(drawMatrix.transformPoint(std::move(poly.m_verticies[i][2]))).convertToSfVec2();

                        m_verticies[m_activeVertexCount + 0].color = drawColour;
                        m_verticies[m_activeVertexCount + 1].color = drawColour;
                        m_verticies[m_activeVertexCount + 2].color = drawColour;
                    }

                m_activeVertexCount += 3;
            }
        FE_END_PROFILE;
    }

void fe::gui::control::draw(sf::RenderTarget &target)
    {
        FE_ENGINE_PROFILE("gui_control", "draw_to_app");
        target.draw(m_verticies.data(), m_activeVertexCount, sf::PrimitiveType::Triangles);
        m_activeVertexCount = 0;
        m_vertexCount = 0;
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

void fe::gui::control::setControlOffset(fe::lightVector2d offset)
    {
        m_controlOffset = offset;
    }

fe::gui::control::control() :
    m_drawColour(sf::Color::White),
    m_vertexCount(0),
    m_activeVertexCount(0)
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
                            realMousePos = getDrawMatrix().getMatrix().transformPointToLocalSpace(std::forward<const fe::lightVector2d>(realMousePos)) - m_controlOffset;

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
                            realMousePos = getDrawMatrix().getMatrix().transformPointToLocalSpace(std::forward<const fe::lightVector2d>(realMousePos)) - m_controlOffset;

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
                                    realMousePos = getDrawMatrix().getMatrix().transformPointToLocalSpace(std::forward<const fe::lightVector2d>(realMousePos)) - m_controlOffset;

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

fe::lightVector2d fe::gui::control::getSize() const
	{
		fe::lightVector2d max(0.f, 0.f);
		fe::lightVector2d min(0.f, 0.f);

		for (auto &vert : m_controlPolygon.m_points)
			{
				min.x = std::min(min.x, vert.x);
				min.y = std::min(min.y, vert.y);

				max.x = std::max(max.x, vert.x);
				max.y = std::max(max.y, vert.y);
			}

		return max - min;
	}
