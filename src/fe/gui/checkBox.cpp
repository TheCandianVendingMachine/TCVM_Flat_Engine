#include "fe/gui/checkBox.hpp"

void fe::gui::checkBox::onStateChange(dialogStates previous, dialogStates next)
    {
        switch (next)
            {
                case fe::gui::dialogStates::ACTIVE:
                    setDrawColour(m_colourOnActive);
                    if (m_checkState != checkState::SELECTED) 
                        {
                            m_checkState = checkState::NONE;
                        }
                    break;
                case fe::gui::dialogStates::DISABLED:
                    setDrawColour(m_colourOnDeactive);
                    break;
                case fe::gui::dialogStates::HIGHLIGHTED:
                    setDrawColour(m_colourOnHighlight);
                    if (m_checkState != checkState::SELECTED) 
                        {
                            m_checkState = checkState::HIGHLIGHT;
                        }
                    break;
                case fe::gui::dialogStates::PRESSED:
                    setDrawColour(m_colourOnPress);
                    m_checkState = (m_checkState == checkState::SELECTED) ? checkState::HIGHLIGHT : checkState::SELECTED;
                    break;
                default:
                    break;
            }
    }

void fe::gui::checkBox::drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix)
    {
        //drawPolygon(getControlPolygon(), target, drawMatrix, getDrawColour());
        drawPolygon(m_outline, target, drawMatrix, getDrawColour());

        if (m_checkState > checkState::NONE)
            {
                fe::matrix3d checkMatrix = drawMatrix;
                checkMatrix.translate(fe::lightVector2d(m_boxOutlineWidth, m_boxOutlineWidth));

                sf::Color drawColour = getDrawColour();
                if (m_checkState == checkState::HIGHLIGHT)
                    {
                        drawColour.a = 150;
                    }

                drawPolygon(m_checkMark, target, checkMatrix, drawColour);
            }
    }

fe::gui::checkBox::checkBox(float radius, float boxOutlineWidth, float checkMarkWidthFromSide) :
    m_checkRadius(radius),
    m_boxOutlineWidth(boxOutlineWidth),
    m_distanceFromSide(checkMarkWidthFromSide),
    m_checkState(NONE),
    m_colourOnActive(sf::Color::White),
    m_colourOnDeactive(sf::Color::White),
    m_colourOnHighlight(sf::Color::White),
    m_colourOnPress(sf::Color::White)
    {
        createCheckBox(radius, boxOutlineWidth, checkMarkWidthFromSide);
    }

void fe::gui::checkBox::createCheckBox(float radius, float boxOutlineWidth, float checkMarkWidthFromSide)
    {
        m_checkRadius = radius;
        m_boxOutlineWidth = boxOutlineWidth;
        m_distanceFromSide = checkMarkWidthFromSide;

        float increment = (m_checkRadius - (checkMarkWidthFromSide * 2.f)) / 4.f;
        fe::lightVector2d sideBuffer(checkMarkWidthFromSide, checkMarkWidthFromSide);
        float widthModifier = (m_checkRadius / 8.f); // radius / (4.f * 2.f)

        m_checkMark.clear();
        m_checkMark.addPoint(fe::lightVector2d((1 * increment) - widthModifier, (0 * increment))                    + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d((2 * increment),                 (1 * increment) + widthModifier)    + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d((3 * increment) + widthModifier, (0 * increment))                    + sideBuffer);

        m_checkMark.addPoint(fe::lightVector2d((4 * increment),                 (1 * increment) - widthModifier)    + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d((3 * increment) - widthModifier, (2 * increment))                    + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d((4 * increment),                 (3 * increment) + widthModifier)    + sideBuffer);

        m_checkMark.addPoint(fe::lightVector2d((3 * increment) + widthModifier, (4 * increment))                    + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d((2 * increment),                 (3 * increment) - widthModifier)    + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d((1 * increment) - widthModifier, (4 * increment))                    + sideBuffer);

        m_checkMark.addPoint(fe::lightVector2d((0 * increment),                 (3 * increment) + widthModifier)    + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d((1 * increment) + widthModifier, (2 * increment))                    + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d((0 * increment),                 (1 * increment) - widthModifier)    + sideBuffer);

        m_checkMark.createPolygon();

        boxOutlineWidth = radius - boxOutlineWidth;

        m_outline.clear();
        m_outline.addPoint({ (0.f),                                    (0.f) });
        m_outline.addPoint({ (3.f * radius) - (2.f * boxOutlineWidth), (0.f) });
        m_outline.addPoint({ (3.f * radius) - (2.f * boxOutlineWidth), (3.f * radius) - (2.f * boxOutlineWidth) });
        m_outline.addPoint({ (0.f),                                    (3.f * radius) - (2.f * boxOutlineWidth) });
        m_outline.addPoint({ (0.f),                                    (2.f * radius) - (boxOutlineWidth) });
        m_outline.addPoint({ (2.f * radius) - (boxOutlineWidth),       (2.f * radius) - (boxOutlineWidth) });
        m_outline.addPoint({ (2.f * radius) - (boxOutlineWidth),       (radius - boxOutlineWidth) });
        m_outline.addPoint({ (radius - boxOutlineWidth),               (radius - boxOutlineWidth) });
        m_outline.addPoint({ (radius - boxOutlineWidth),               (2.f * radius) - (boxOutlineWidth) });
        m_outline.addPoint({ (0.f),                                    (2.f * radius) - (boxOutlineWidth) });
        
        m_outline.createPolygon();

        getControlPolygon().addPoint({ (0.f),                                       (0.f) });
        getControlPolygon().addPoint({ (3.f * radius) - (2.f * boxOutlineWidth),    (0.f) });
        getControlPolygon().addPoint({ (3.f * radius) - (2.f * boxOutlineWidth),    (3.f * radius) - (2.f * boxOutlineWidth) });
        getControlPolygon().addPoint({ (0.f),                                       (3.f * radius) - (2.f * boxOutlineWidth) });
        getControlPolygon().createPolygon();
    }

void fe::gui::checkBox::setColourOnActive(sf::Color colour)
    {
        m_colourOnActive = colour;
    }

void fe::gui::checkBox::setColourOnDeactive(sf::Color colour)
    {
        m_colourOnDeactive = colour;
    }

void fe::gui::checkBox::setColourHighlight(sf::Color colour)
    {
        m_colourOnHighlight = colour;
    }

void fe::gui::checkBox::setColourOnPress(sf::Color colour)
    {
        m_colourOnPress = colour;
    }

void fe::gui::checkBox::setSelected(bool value)
    {
        m_checkState = static_cast<checkState>(value);
    }

bool fe::gui::checkBox::isSelected() const
    {
        return m_checkState == checkState::SELECTED;
    }
