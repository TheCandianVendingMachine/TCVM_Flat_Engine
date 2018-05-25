#include "fe/gui/dialog.hpp"
#include "fe/debug/profiler.hpp"
#include <algorithm>

fe::gui::dialog::dialog() :
    m_dialogState(dialogStates::NONE),
    m_killed(false),
    m_draw(true)
    {
    }

void fe::gui::dialog::kill()
    {
        m_killed = true;
    }

bool fe::gui::dialog::isKilled() const
    {
        return m_killed;
    }

void fe::gui::dialog::setDraw(bool draw)
    {
        m_draw = draw;
    }

bool fe::gui::dialog::isDrawn() const
    {
        return m_draw;
    }

void fe::gui::dialog::setState(dialogStates state)
    {
        onStateChange(m_dialogState, state);
        m_dialogState = state;
    }

fe::gui::dialogStates fe::gui::dialog::getState() const
    {
        return m_dialogState;
    }

void fe::gui::dialog::draw(sf::RenderTarget &target)
    {
        FE_ENGINE_PROFILE("gui_dialog", "draw_elements");
        drawDialogElements(target, m_drawMatrix.getMatrix());
        FE_END_PROFILE;

        FE_ENGINE_PROFILE("gui_dialog", "draw_text");
        drawDialogText(target, m_drawMatrix);
        FE_END_PROFILE;
    }

fe::transformable &fe::gui::dialog::getDrawMatrix()
    {
        return m_drawMatrix;
    }

fe::gui::dialog &fe::gui::dialog::operator=(dialog &rhs)
    {
        if (&rhs != this)
            {
                m_killed = rhs.m_killed;
            }
        return *this;
    }
