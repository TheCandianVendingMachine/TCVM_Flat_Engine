#include "fe/gui/radioButton.hpp"

fe::lightVector2d fe::gui::radioButton::getCirclePoint(unsigned int index, float radius) const
    {
        static constexpr float pi = 3.14159f;

        float angle = (index * 2.f) * (pi / m_points) - (pi / 2.f);

        float x = std::cos(angle) * radius;
        float y = std::sin(angle) * radius;

        return fe::lightVector2d(x, y);
    }

fe::gui::radioButton::radioButton(unsigned int points, float radius, float outlineWidth, float markWidthFromSide) :
    m_points(points),
    fe::gui::listButton(radius, outlineWidth, markWidthFromSide)
    {
        createButton(radius, outlineWidth, markWidthFromSide);
    }

void fe::gui::radioButton::createButton(float radius, float outlineWidth, float markWidthFromSide)
    {
        clearOutline();
        for (unsigned int i = 0; i <= m_points; i++)
            {
                fe::lightVector2d coord = getCirclePoint(i, radius);
                addOutlinePoint(coord.x + radius, coord.y + radius);
            }

        for (int i = m_points; i >= 0; i--)
            {
                fe::lightVector2d coord = getCirclePoint(i, radius - outlineWidth);
                addOutlinePoint(coord.x + radius, coord.y + radius);
            }
        createOutline();


        clearMark();
        for (unsigned int i = 0; i <= m_points; i++)
            {
                fe::lightVector2d coord = getCirclePoint(i, radius - outlineWidth - markWidthFromSide);
                addMarkPoint(coord.x + radius - markWidthFromSide, coord.y + radius - markWidthFromSide);
            }
        createMark();
    }
