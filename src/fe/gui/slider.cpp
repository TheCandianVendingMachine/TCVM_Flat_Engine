#include "fe/gui/slider.hpp"

void fe::gui::slider::drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix)
    {
        drawPolygon(m_sliderContainer, drawMatrix, getDrawColour());

        fe::matrix3d tempTransform = drawMatrix;
        tempTransform.translate(m_sliderPosition);
        drawPolygon(getControlPolygon(), tempTransform, getDrawColour());

        draw(target);
    }

void fe::gui::slider::onStateChange(dialogStates previous, dialogStates next)
    {
        
    }

fe::gui::slider::slider(sliderOrientation orientation, float sliderControlSize, float scrollSize, float lengthOfOutline, float outlineWidth, float sliderControlWidthFromEdge) :
    m_percentageScrolled(0.f)
    {
        // how much of the slider the control has filled. Axis which determines scroll
        float sliderFill = scrollSize * (lengthOfOutline - outlineWidth * 2.f) - (sliderControlWidthFromEdge * 2.f);
        // how much of the slider the control takes up. No effect on scroll data
        float sliderLength = sliderControlSize - (sliderControlWidthFromEdge * 2.f);

        switch (orientation)
            {
                case fe::gui::slider::sliderOrientation::HORIZONTAL:
                    m_sliderSize = fe::Vector2d(sliderFill,         sliderLength);
                    m_containerSize = fe::Vector2d(lengthOfOutline, sliderControlSize + (sliderControlWidthFromEdge * 2.f));
                    break;
                case fe::gui::slider::sliderOrientation::VERTICAL:
                    m_sliderSize = fe::Vector2d(sliderLength,                                               sliderFill);
                    m_containerSize = fe::Vector2d(sliderControlSize + (sliderControlWidthFromEdge * 2.f),  lengthOfOutline);
                    break;
                default:
                    break;
            }

        fe::Vector2d distFromEdge(outlineWidth, outlineWidth);

        m_sliderContainer.addPoint(fe::Vector2d(0.f,                                0.f));
        m_sliderContainer.addPoint(fe::Vector2d(m_containerSize.x,                  0.f));
        m_sliderContainer.addPoint(fe::Vector2d(m_containerSize.x,                  m_containerSize.y));
        m_sliderContainer.addPoint(fe::Vector2d(0.f,                                m_containerSize.y));

        m_sliderContainer.addPoint(fe::Vector2d(0.f,                                0.f));

        m_sliderContainer.addPoint(fe::Vector2d(distFromEdge.x,                     m_containerSize.y - distFromEdge.y));
        m_sliderContainer.addPoint(fe::Vector2d(m_containerSize.x - distFromEdge.x, m_containerSize.y - distFromEdge.y));
        m_sliderContainer.addPoint(fe::Vector2d(m_containerSize.x - distFromEdge.x, distFromEdge.y));
        m_sliderContainer.addPoint(fe::Vector2d(distFromEdge.x,                     distFromEdge.y));

        m_sliderContainer.addPoint(fe::Vector2d(distFromEdge.x,                     m_containerSize.y - distFromEdge.y));

        m_sliderContainer.createPolygon();

        addPoint(0.f,               0.f);
        addPoint(m_sliderSize.x,    0.f);
        addPoint(m_sliderSize.x,    m_sliderSize.y);
        addPoint(0.f,               m_sliderSize.y);
        getControlPolygon().createPolygon();

        m_containerOutlineWidth = distFromEdge;

        m_sliderPosition.x = outlineWidth + sliderControlWidthFromEdge;
        m_sliderPosition.y = outlineWidth + sliderControlWidthFromEdge;
    }

