#include "fe/gui/listButton.hpp"
#include "fe/debug/profiler.hpp"

void fe::gui::listButton::onStateChange(dialogStates previous, dialogStates next)
    {
        buttonState nextState = m_buttonState;
        switch (next)
            {
                case fe::gui::dialogStates::ACTIVE:
                    setDrawColour(m_colourOnActive);
                    if (m_buttonState != buttonState::SELECTED)
                        {
                            nextState = buttonState::NONE;
                        }
                    break;
                case fe::gui::dialogStates::DISABLED:
                    setDrawColour(m_colourOnDeactive);
                    break;
                case fe::gui::dialogStates::HIGHLIGHTED:
                    setDrawColour(m_colourOnHighlight);
                    if (m_buttonState != buttonState::SELECTED)
                        {
                            nextState = buttonState::HIGHLIGHT;
                        }
                    break;
                case fe::gui::dialogStates::PRESSED:
                    setDrawColour(m_colourOnPress);
                    nextState = (m_buttonState == buttonState::SELECTED) ? buttonState::HIGHLIGHT : buttonState::SELECTED;
                    break;
                default:
                    break;
            }

        onButtonStateChange(m_buttonState, nextState);
        m_buttonState = nextState;
    }

void fe::gui::listButton::drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix)
    {
        FE_ENGINE_PROFILE("gui_list_button", "draw");
        //drawPolygon(getControlPolygon(), target, drawMatrix, getDrawColour());
        drawPolygon(m_outline, drawMatrix, getDrawColour());

        if (m_buttonState > buttonState::NONE)
            {
                fe::matrix3d checkMatrix = drawMatrix;
                checkMatrix.translate(fe::lightVector2d(m_outlineWidth, m_outlineWidth));

                sf::Color drawColour = getDrawColour();
                if (m_buttonState == buttonState::HIGHLIGHT)
                    {
                        drawColour.a = 150;
                    }

                drawPolygon(m_mark, checkMatrix, drawColour);
            }

        draw(target);
        FE_END_PROFILE;
    }

void fe::gui::listButton::addMarkPoint(float x, float y)
    {
        m_mark.addPoint(fe::lightVector2d(x, y));
    }

void fe::gui::listButton::addOutlinePoint(float x, float y)
    {
        m_outline.addPoint(fe::lightVector2d(x, y));
    }

void fe::gui::listButton::clearMark()
    {
        m_mark.clear();
    }

void fe::gui::listButton::clearOutline()
    {
        m_outline.clear();
    }

void fe::gui::listButton::createMark()
    {
        m_mark.createPolygon();
    }

void fe::gui::listButton::createOutline()
    {
        m_outline.createPolygon();
    }

fe::gui::listButton::listButton(float radius, float boxOutlineWidth, float checkMarkWidthFromSide) :
    m_checkRadius(radius),
    m_outlineWidth(boxOutlineWidth),
    m_distanceFromSide(checkMarkWidthFromSide),
    m_buttonState(NONE),
    m_colourOnActive(sf::Color::White),
    m_colourOnDeactive(sf::Color::White),
    m_colourOnHighlight(sf::Color::White),
    m_colourOnPress(sf::Color::White)
    {}

void fe::gui::listButton::setCheckRadius(float radius)
    {
        m_checkRadius = radius;
    }

void fe::gui::listButton::setDistanceFromSide(float distance)
    {
        m_distanceFromSide = distance;
    }

void fe::gui::listButton::setOutlineWidth(float width)
    {
        m_outlineWidth = width;
    }

float fe::gui::listButton::getCheckRadius() const
    {
        return m_checkRadius;
    }

float fe::gui::listButton::getDistanceFromSide() const
    {
        return m_distanceFromSide;
    }

float fe::gui::listButton::getOutlineWidth() const
    {
        return m_outlineWidth;
    }

void fe::gui::listButton::setColourOnActive(sf::Color colour)
    {
        m_colourOnActive = colour;
    }

void fe::gui::listButton::setColourOnDeactive(sf::Color colour)
    {
        m_colourOnDeactive = colour;
    }

void fe::gui::listButton::setColourHighlight(sf::Color colour)
    {
        m_colourOnHighlight = colour;
    }

void fe::gui::listButton::setColourOnPress(sf::Color colour)
    {
        m_colourOnPress = colour;
    }

void fe::gui::listButton::setSelected(bool value)
    {
        m_buttonState = static_cast<buttonState>(value);
        setState(static_cast<dialogStates>(value));
    }

bool fe::gui::listButton::isSelected() const
    {
        return m_buttonState == buttonState::SELECTED;
    }

