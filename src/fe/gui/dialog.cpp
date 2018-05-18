#include "fe/gui/dialog.hpp"
#include <algorithm>

void fe::gui::dialog::attach(dialog *element)
    {
        m_containedDialogs.push_back(element);
    }

void fe::gui::dialog::detach(dialog *element)
    {
        m_containedDialogs.erase(std::remove(m_containedDialogs.begin(), m_containedDialogs.end(), element), m_containedDialogs.end());
    }

bool fe::gui::dialog::isElementAttached(const dialog *element) const
    {
        return std::find(m_containedDialogs.begin(), m_containedDialogs.end(), element) != m_containedDialogs.end();
    }

bool fe::gui::dialog::isAttachedTo(const dialog *element) const
    {
        return element->isElementAttached(this);
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

void fe::gui::dialog::draw(sf::RenderTarget &target, fe::gui::guiBatch &guiBatch)
    {
        std::sort(m_containedDialogs.begin(), m_containedDialogs.end(), [] (fe::gui::dialog *a, fe::gui::dialog *b) { return a->getDrawOrder() < b->getDrawOrder(); });

        for (auto &child : m_containedDialogs)
            {
                child->draw(target, guiBatch);
            }

        drawDialogElements(guiBatch);
        drawDialogText(target);
    }

fe::gui::dialog &fe::gui::dialog::operator=(dialog &rhs)
    {
        if (&rhs != this)
            {
                m_containedDialogs = rhs.m_containedDialogs;
                m_killed = rhs.m_killed;
            }
        return *this;
    }
