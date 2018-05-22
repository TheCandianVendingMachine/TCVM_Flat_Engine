#include "fe/gui/checkBox.hpp"

void fe::gui::checkBox::onStateChange(dialogStates previous, dialogStates next)
    {
        return void();
    }

void fe::gui::checkBox::drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix)
    {
        drawPolygon(getControlPolygon(), target, drawMatrix, getDrawColour());

        fe::matrix3d checkMatrix = drawMatrix;
        checkMatrix.translate(fe::lightVector2d(m_checkRadius, m_checkRadius));
        drawPolygon(m_checkMark, target, checkMatrix, getDrawColour());
    }

fe::gui::checkBox::checkBox(float radius, float boxOutlineWidth, float checkMarkWidthFromSide) :
    fe::gui::button({
        { (0.f),                                    (0.f) },
        { (3.f * radius) - (2.f * boxOutlineWidth), (0.f) },
        { (3.f * radius) - (2.f * boxOutlineWidth), (3.f * radius) - (2.f * boxOutlineWidth)},
        { (0.f),                                    (3.f * radius) - (2.f * boxOutlineWidth) },
        { (0.f),                                    (2.f * radius) - (boxOutlineWidth) },
        { (2.f * radius) - (boxOutlineWidth),       (2.f * radius) - (boxOutlineWidth) },
        { (2.f * radius) - (boxOutlineWidth),       (radius - boxOutlineWidth) },
        { (radius - boxOutlineWidth),               (radius - boxOutlineWidth) },
        { (radius - boxOutlineWidth),               (2.f * radius) - (boxOutlineWidth) },
        { (0.f),                                    (2.f * radius) - (boxOutlineWidth) }
    }),
    m_checkRadius(radius)
    {
        float increment = (m_checkRadius - (checkMarkWidthFromSide * 2.f)) / 4.f;
        fe::lightVector2d sideBuffer(checkMarkWidthFromSide, checkMarkWidthFromSide);
        float widthModifier = (m_checkRadius / 8.f); // radius / (4.f * 2.f)

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


    }
