#include "guiElement.hpp"

fe::gui::guiElement::guiElement() : m_parentPanel(nullptr), m_parentElement(nullptr)
    {
    }

void fe::gui::guiElement::setParent(const panel *attached)
    {
        m_parentPanel = attached;
    }

void fe::gui::guiElement::setParent(guiElement *attached)
    {
        m_parentElement = attached;
    }

const fe::gui::panel *fe::gui::guiElement::getParentPanel() const
    {
        return m_parentPanel;
    }

fe::gui::guiElement *fe::gui::guiElement::getParentElement() const
    {
        return m_parentElement;
    }

void fe::gui::guiElement::setSize(const fe::Vector2d &size)
    {
        scale({ size.x / m_size.x, size.y / m_size.y });
        m_size = size;
    }

const fe::Vector2d &fe::gui::guiElement::getSize() const
    {
        return m_size;
    }

void fe::gui::guiElement::draw(sf::RenderTarget &target)
    {
        if (m_parentElement)
            {
                drawElement(target, m_parentElement->getMatrix());
            }
        else
            {
                drawElement(target, fe::matrix3d());
            }
    }
