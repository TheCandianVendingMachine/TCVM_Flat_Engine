#include "fe/gui/slider.hpp"
#include "fe/debug/logger.hpp"
#include <SFML/Window/Event.hpp>

void fe::gui::slider::drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix)
    {
        drawPolygon(m_sliderContainer, drawMatrix, getDrawColour());

        fe::matrix3d tempTransform = drawMatrix;
        tempTransform.translate(m_sliderPosition);
        setControlOffset(m_sliderPosition);
        drawPolygon(getControlPolygon(), tempTransform, getDrawColour());

        draw(target);
    }

void fe::gui::slider::onStateChange(dialogStates previous, dialogStates next)
    {
        
    }

void fe::gui::slider::handleWindowEvent(const sf::Event &event)
    {
        switch (event.type)
            {
                case sf::Event::MouseButtonPressed:
                    m_sliderGrabPoint.x = event.mouseButton.x - (getPosition().x + m_sliderPosition.x);
                    m_sliderGrabPoint.y = event.mouseButton.y - (getPosition().y + m_sliderPosition.y);
                    break;
                case sf::Event::MouseMoved:
                    if (getState() == dialogStates::PRESSED)
                        {
                            fe::Vector2d pos = getMatrix().transformPointToLocalSpace(fe::Vector2d(event.mouseMove.x, event.mouseMove.y) - m_sliderGrabPoint);
                            switch (m_orientation)
                                {
                                    case fe::gui::slider::sliderOrientation::HORIZONTAL:
                                        m_sliderPosition.y = pos.x;
                                        break;
                                    case fe::gui::slider::sliderOrientation::VERTICAL:
                                        m_sliderPosition.y = pos.y;
                                        break;
                                    default:
                                        break;
                                }
                        }
                    break;
                default:
                    break;
            }
    }

fe::gui::slider::slider(sliderOrientation orientation, float sliderControlSize, float scrollSize, float lengthOfOutline, float outlineWidth, float sliderControlWidthFromEdge) :
    m_orientation(orientation),
    m_sliderDistanceFromSide(sliderControlWidthFromEdge, sliderControlWidthFromEdge),
    m_containerOutlineWidth(outlineWidth, outlineWidth)
    {
        // how much of the slider the control has filled. Axis which determines scroll
        float sliderFill = scrollSize * (lengthOfOutline - outlineWidth * 2.f) - (sliderControlWidthFromEdge * 2.f);
        // how much of the slider the control takes up. No effect on scroll data
        float sliderLength = sliderControlSize - (sliderControlWidthFromEdge * 2.f) - (outlineWidth * 2.f);

        switch (orientation)
            {
                case fe::gui::slider::sliderOrientation::HORIZONTAL:
                    m_sliderSize = fe::Vector2d(sliderFill, sliderLength);
                    m_containerSize = fe::Vector2d(lengthOfOutline, sliderControlSize);
                    break;
                case fe::gui::slider::sliderOrientation::VERTICAL:
                    m_sliderSize = fe::Vector2d(sliderLength, sliderFill);
                    m_containerSize = fe::Vector2d(sliderControlSize, lengthOfOutline);
                    break;
                default:
                    break;
            }

        m_sliderContainer.addPoint(fe::Vector2d(0.f,                                0.f));
        m_sliderContainer.addPoint(fe::Vector2d(m_containerSize.x,                  0.f));
        m_sliderContainer.addPoint(fe::Vector2d(m_containerSize.x,                  m_containerSize.y));
        m_sliderContainer.addPoint(fe::Vector2d(0.f,                                m_containerSize.y));

        m_sliderContainer.addPoint(fe::Vector2d(0.f,                                0.f));

        m_sliderContainer.addPoint(fe::Vector2d(m_containerOutlineWidth.x,                     m_containerSize.y - m_containerOutlineWidth.y));
        m_sliderContainer.addPoint(fe::Vector2d(m_containerSize.x - m_containerOutlineWidth.x, m_containerSize.y - m_containerOutlineWidth.y));
        m_sliderContainer.addPoint(fe::Vector2d(m_containerSize.x - m_containerOutlineWidth.x, m_containerOutlineWidth.y));
        m_sliderContainer.addPoint(fe::Vector2d(m_containerOutlineWidth.x,                     m_containerOutlineWidth.y));

        m_sliderContainer.addPoint(fe::Vector2d(m_containerOutlineWidth.x,                     m_containerSize.y - m_containerOutlineWidth.y));

        m_sliderContainer.createPolygon();

        addPoint(0.f,               0.f);
        addPoint(m_sliderSize.x,    0.f);
        addPoint(m_sliderSize.x,    m_sliderSize.y);
        addPoint(0.f,               m_sliderSize.y);
        getControlPolygon().createPolygon();

        m_sliderPosition.x = outlineWidth + sliderControlWidthFromEdge;
        m_sliderPosition.y = outlineWidth + sliderControlWidthFromEdge;
    }

float fe::gui::slider::getPercentScrolled() const
    {
        fe::Vector2d halfSliderSize = m_sliderSize / 2.f;
        fe::Vector2d position = halfSliderSize + (m_sliderPosition - m_containerOutlineWidth - m_sliderDistanceFromSide);
        fe::Vector2d oppositePosition = m_containerSize - halfSliderSize - m_containerOutlineWidth;

        fe::Vector2d distanceToEnd = oppositePosition - position;
        fe::Vector2d originalDistanceToEnd = oppositePosition - halfSliderSize;

        float percentScrolled = 0.f;

        switch (m_orientation)
            {
                case sliderOrientation::HORIZONTAL:
                    percentScrolled = (originalDistanceToEnd.x - distanceToEnd.x) / originalDistanceToEnd.x;
                    break;
                case sliderOrientation::VERTICAL:
                    percentScrolled = (originalDistanceToEnd.y - distanceToEnd.y) / originalDistanceToEnd.y;
                    break;
                default:
                    break;
            }

        return percentScrolled;
    }

