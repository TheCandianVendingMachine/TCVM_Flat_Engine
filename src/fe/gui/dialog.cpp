#include "fe/gui/dialog.hpp"
#include <algorithm>

fe::gui::dialog::dialog() :
    m_parent(nullptr),
    m_dialogState(dialogStates::NONE),
    m_drawOrder(0),
    m_killed(false)
    {
    }

void fe::gui::dialog::attach(dialog *element)
    {
        element->m_parent = this;
        m_containedDialogs.push_back(element);
    }

void fe::gui::dialog::detach(dialog *element)
    {
        element->m_parent = nullptr;
        m_containedDialogs.erase(std::remove(m_containedDialogs.begin(), m_containedDialogs.end(), element), m_containedDialogs.end());
    }

bool fe::gui::dialog::isElementAttached(const dialog *element) const
    {
        return element->m_parent != nullptr;
    }

bool fe::gui::dialog::isAttachedTo(const dialog *element) const
    {
        return element->m_parent == element;
    }

void fe::gui::dialog::kill()
    {
        m_killed = true;
    }

bool fe::gui::dialog::isKilled() const
    {
        return m_killed;
    }

void fe::gui::dialog::setDrawOrder(int focus)
    {
        m_drawOrder = focus;
    }

int fe::gui::dialog::getDrawOrder() const
    {
        return m_drawOrder;
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
        drawDialogText(target, m_drawMatrix.getMatrix());
    }

void fe::gui::dialog::draw(fe::gui::guiBatch &guiBatch)
    {
        drawDialogElements(guiBatch, m_drawMatrix.getMatrix());
        m_drawMatrix = fe::transformable();
    }

fe::transformable &fe::gui::dialog::getDrawMatrix()
    {
        return m_drawMatrix;
    }

fe::gui::dialog &fe::gui::dialog::operator=(dialog &rhs)
    {
        if (&rhs != this)
            {
                m_containedDialogs = rhs.m_containedDialogs;
                m_killed = rhs.m_killed;
                m_drawOrder = rhs.m_drawOrder;
                m_parent = rhs.m_parent;
            }
        return *this;
    }
