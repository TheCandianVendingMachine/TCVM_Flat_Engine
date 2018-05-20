#include "fe/gui/dialog.hpp"
#include <algorithm>

fe::gui::dialog::dialog() :
    m_dialogState(dialogStates::NONE),
    m_killed(false)
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
        drawDialogElements(target, m_drawMatrix.getMatrix());
        drawDialogText(target, m_drawMatrix);
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
