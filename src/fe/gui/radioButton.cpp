#include "fe/gui/radioButton.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include "fe/objectManagement/str.hpp"

fe::lightVector2d fe::gui::radioButton::getCirclePoint(unsigned int index, float radius) const
    {
        constexpr float pi = 3.14159f;

        float angle = (index * 2.f) * (pi / m_points) - (pi / 2.f);

        float x = std::cos(angle) * radius;
        float y = std::sin(angle) * radius;

        return fe::lightVector2d(x, y);
    }

void fe::gui::radioButton::onButtonStateChange(buttonState previous, buttonState next)
    {
        if (previous != next && next == buttonState::SELECTED)
            {
                fe::gameEvent event(FE_STR("radioButtonSelect"), 2);
                event.args[0].arg.TYPE_VOIDP = this;
                event.args[0].argType = fe::gameEventArgument::type::TYPE_VOIDP;

                event.args[1].arg.TYPE_UINTEGER = m_id;
                event.args[1].argType = fe::gameEventArgument::type::TYPE_UINT;

                fe::engine::get().getEventSender().sendTo(event, m_idToSendTo);
            }
    }

fe::gui::radioButton::radioButton(unsigned int id, unsigned int idToSendTo, unsigned int points, float radius, float outlineWidth, float markWidthFromSide) :
    m_points(points),
    m_id(id),
    m_idToSendTo(idToSendTo),
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
                addMarkPoint(coord.x + radius, coord.y + radius);
            }
        createMark();


        getControlPolygon().clear();
        for (unsigned int i = 0; i <= m_points; i++)
            {
                fe::lightVector2d coord = getCirclePoint(i, radius);
                addPoint(coord.x + radius, coord.y + radius);
            }
        getControlPolygon().createPolygon();
    }
