#include "fe/gui/checkBox.hpp"

void fe::gui::checkBox::onStateChange(dialogStates previous, dialogStates next)
    {
        return void();
    }

void fe::gui::checkBox::drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix)
    {
        drawPolygon(getControlPolygon(), target, drawMatrix, getDrawColour());
        drawPolygon(m_checkMark, target, drawMatrix, getDrawColour());
    }

fe::gui::checkBox::checkBox(float radius, float boxOutlineWidth, float checkMarkWidthFromSide) :
    fe::gui::button({
        { (radius - boxOutlineWidth),                       (radius - boxOutlineWidth) },
        { (3.f * radius) - (radius - boxOutlineWidth),      (radius - boxOutlineWidth) },
        { (3.f * radius) - (radius - boxOutlineWidth),      (3.f * radius) - (radius - boxOutlineWidth) },
        { (radius - boxOutlineWidth),                       (3.f * radius) - (radius - boxOutlineWidth) },
        { (radius - boxOutlineWidth),                       (2.f * radius) },
        { (2.f * radius),                                   (2.f * radius) },
        { (2.f * radius),                                   radius },
        { radius,                                           radius },
        { radius,                                           (2.f * radius) },
        { (radius - boxOutlineWidth),                       (2.f * radius) },
        { (radius - boxOutlineWidth),                       (radius - boxOutlineWidth) }
    })
    {
        float increment = (radius - (checkMarkWidthFromSide * 2.f)) / 4.f;
        fe::lightVector2d sideBuffer(checkMarkWidthFromSide, checkMarkWidthFromSide);

        m_checkMark.addPoint(fe::lightVector2d(1 * increment, 0 * increment) + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d(2 * increment, 1 * increment) + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d(3 * increment, 0 * increment) + sideBuffer);

        m_checkMark.addPoint(fe::lightVector2d(4 * increment, 1 * increment) + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d(3 * increment, 2 * increment) + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d(4 * increment, 3 * increment) + sideBuffer);

        m_checkMark.addPoint(fe::lightVector2d(3 * increment, 4 * increment) + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d(2 * increment, 3 * increment) + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d(1 * increment, 4 * increment) + sideBuffer);

        m_checkMark.addPoint(fe::lightVector2d(0 * increment, 3 * increment) + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d(1 * increment, 2 * increment) + sideBuffer);
        m_checkMark.addPoint(fe::lightVector2d(0 * increment, 1 * increment) + sideBuffer);

        m_checkMark.createPolygon();

    }
