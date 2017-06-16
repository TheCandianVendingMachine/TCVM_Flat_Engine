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

void fe::gui::guiElement::setColour(sf::Color colour)
    {
        m_colour = colour;
        for (unsigned int i = 0; i < m_shape.getVertexCount(); i++)
            {
                m_shape[i].color = colour;
            }
    }

sf::Color fe::gui::guiElement::getColour() const
    {
        return m_colour;
    }

void fe::gui::guiElement::draw(sf::RenderTarget &target)
    {
        drawElement(target, getParentTransform());
    }
