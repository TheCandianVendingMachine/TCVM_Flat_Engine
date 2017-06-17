#include "guiElement.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

fe::matrix3d fe::gui::guiElement::getParentTransform()
    {
        if (m_parentElement)
            {
                return getMatrix() * m_parentElement->getParentTransform();
            }
        return getMatrix();
    }

fe::gui::guiElement::guiElement() : m_parentPanel(nullptr), m_parentElement(nullptr)
    {
        m_active = false;
        m_colourUpdate = false;
        setActive(false);
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
        m_size = size;
    }

const fe::Vector2d &fe::gui::guiElement::getSize()
    {
        return m_size;
    }

void fe::gui::guiElement::setActive(bool value)
    {
        m_active = value;
        m_colourUpdate = true;
    }

bool fe::gui::guiElement::getActive() const
    {
        return m_active;
    }

void fe::gui::guiElement::setActiveColour(sf::Color colour)
    {
        m_activeColour = colour;
    }

void fe::gui::guiElement::setInactiveColour(sf::Color colour)
    {
        m_inactiveColour = colour;
    }

sf::Color fe::gui::guiElement::getActiveColour() const
    {
        return m_activeColour;
    }

sf::Color fe::gui::guiElement::getInactiveColour() const
    {
        return m_inactiveColour;
    }

void fe::gui::guiElement::draw(sf::RenderTarget &target)
    {
        if (m_active && m_colourUpdate)
            {
                for (unsigned int i = 0; i < m_shape.getVertexCount(); i++)
                    {
                        m_shape[i].color = m_activeColour;
                    }
                m_colourUpdate = false;
            }
        else if (m_colourUpdate)
            {
                for (unsigned int i = 0; i < m_shape.getVertexCount(); i++)
                    {
                        m_shape[i].color = m_inactiveColour;
                    }
                m_colourUpdate = false;
            }
        drawElement(target, getParentTransform());
    }
