#include "fe/gui/checkBox.hpp"

fe::gui::checkBox::checkBox(float radius, float boxOutlineWidth, float checkMarkWidthFromSide) :
    listButton(radius, boxOutlineWidth, checkMarkWidthFromSide)
    {
        createButton(radius, boxOutlineWidth, checkMarkWidthFromSide);
    }

void fe::gui::checkBox::createButton(float radius, float boxOutlineWidth, float checkMarkWidthFromSide)
    {
        setCheckRadius(radius);
        setOutlineWidth(boxOutlineWidth);
        setDistanceFromSide(checkMarkWidthFromSide);

        float increment = (getCheckRadius() - (checkMarkWidthFromSide * 2.f)) / 4.f;
        float sideBuffer = checkMarkWidthFromSide;
        float widthModifier = (getCheckRadius() / 8.f); // radius / (4.f * 2.f)

        clearMark();
        
        addMarkPoint((1 * increment) - widthModifier + sideBuffer, (0 * increment) + sideBuffer);
        addMarkPoint((2 * increment) + sideBuffer,                 (1 * increment) + widthModifier + sideBuffer);
        addMarkPoint((3 * increment) + widthModifier + sideBuffer, (0 * increment) + sideBuffer);

        addMarkPoint((4 * increment) + sideBuffer,                 (1 * increment) - widthModifier + sideBuffer);
        addMarkPoint((3 * increment) - widthModifier + sideBuffer, (2 * increment) + sideBuffer);
        addMarkPoint((4 * increment) + sideBuffer,                 (3 * increment) + widthModifier + sideBuffer);

        addMarkPoint((3 * increment) + widthModifier + sideBuffer, (4 * increment) + sideBuffer);
        addMarkPoint((2 * increment) + sideBuffer,                 (3 * increment) - widthModifier + sideBuffer);
        addMarkPoint((1 * increment) - widthModifier + sideBuffer, (4 * increment) + sideBuffer);

        addMarkPoint((0 * increment) + sideBuffer,                 (3 * increment) + widthModifier + sideBuffer);
        addMarkPoint((1 * increment) + widthModifier + sideBuffer, (2 * increment) + sideBuffer);
        addMarkPoint((0 * increment) + sideBuffer,                 (1 * increment) - widthModifier + sideBuffer);

        createMark();

        boxOutlineWidth = radius - boxOutlineWidth;

        clearOutline();

        addOutlinePoint((0.f),                                    (0.f));
        addOutlinePoint((3.f * radius) - (2.f * boxOutlineWidth), (0.f));
        addOutlinePoint((3.f * radius) - (2.f * boxOutlineWidth), (3.f * radius) - (2.f * boxOutlineWidth));
        addOutlinePoint((0.f),                                    (3.f * radius) - (2.f * boxOutlineWidth));
        addOutlinePoint((0.f),                                    (2.f * radius) - (boxOutlineWidth));
        addOutlinePoint((2.f * radius) - (boxOutlineWidth),       (2.f * radius) - (boxOutlineWidth));
        addOutlinePoint((2.f * radius) - (boxOutlineWidth),       (radius - boxOutlineWidth));
        addOutlinePoint((radius - boxOutlineWidth),               (radius - boxOutlineWidth));
        addOutlinePoint((radius - boxOutlineWidth),               (2.f * radius) - (boxOutlineWidth));
        addOutlinePoint((0.f),                                    (2.f * radius) - (boxOutlineWidth));
        
        createOutline();

        getControlPolygon().addPoint({ (0.f),                                       (0.f) });
        getControlPolygon().addPoint({ (3.f * radius) - (2.f * boxOutlineWidth),    (0.f) });
        getControlPolygon().addPoint({ (3.f * radius) - (2.f * boxOutlineWidth),    (3.f * radius) - (2.f * boxOutlineWidth) });
        getControlPolygon().addPoint({ (0.f),                                       (3.f * radius) - (2.f * boxOutlineWidth) });
        getControlPolygon().createPolygon();
    }
