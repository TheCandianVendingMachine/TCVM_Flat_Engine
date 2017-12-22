#include "guiElement.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include "../objectManagement/str.hpp"
#include <SFML/Graphics/Texture.hpp>

void fe::gui::guiElement::fitToParent()
    {
        float x = getSize().x;
        float y = getSize().y;
        if (getSize().x > m_parentElement->getSize().x)
            {
                x = m_parentElement->getSize().x;
            }
        if (getSize().y > m_parentElement->getSize().y)
            {
                y = m_parentElement->getSize().x;
            }
        setSize({ x, y });
    }

fe::matrix3d fe::gui::guiElement::getParentTransform()
    {
        if (m_parentElement)
            {
                return getMatrix() * m_parentElement->getParentTransform();
            }
        return getMatrix();
    }

fe::gui::guiElement::guiElement() : m_parentPanel(nullptr), m_parentElement(nullptr), m_event(FE_STR("NO_EVENT")), m_id(0), m_extraEvent(FE_STR("NO_EVENT"))
    {
        m_active = false;
        m_colourUpdate = false;
        setActive(false);
    }

void fe::gui::guiElement::setPositionRelative(fe::Vector2d position)
    {
        setPositionRelative(position.x, position.y);
    }

void fe::gui::guiElement::setPositionRelative(float x, float y)
    {
        setPosition((m_parentElement->getSize().x * x) - (m_size.x / 2.f), (m_parentElement->getSize().y * y) - (m_size.y / 2.f));
    }

void fe::gui::guiElement::setEvent(const char *event)
    {
        setEvent(FE_STR(event));
    }

void fe::gui::guiElement::setEvent(fe::str event)
    {
        m_event = event;
    }

fe::str fe::gui::guiElement::getEvent() const
    {
        return m_event;
    }

void fe::gui::guiElement::setExtraEvent(const char *event)
    {
        setExtraEvent(FE_STR(event));
    }

void fe::gui::guiElement::setExtraEvent(fe::str event)
    {
        m_extraEvent = event;
    }

fe::str fe::gui::guiElement::getExtraEvent() const
    {
        return m_extraEvent;
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

const fe::Vector2d &fe::gui::guiElement::getSize() const
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

void fe::gui::guiElement::setTexture(sf::Texture *texture, fe::Vector2<unsigned int> texCoords, fe::Vector2<unsigned int> size)
    {
        m_texture = texture;

        if (size.x == 0)
            {
                size.x = texture->getSize().x;
            }
        if (size.y == 0)
            {
                size.y = texture->getSize().y;
            }

        m_shape[0].texCoords = sf::Vector2f(texCoords.convertToSfVec2()) + sf::Vector2f(0,      0);
        m_shape[1].texCoords = sf::Vector2f(texCoords.convertToSfVec2()) + sf::Vector2f(size.x, 0);
        m_shape[2].texCoords = sf::Vector2f(texCoords.convertToSfVec2()) + sf::Vector2f(size.x, size.y);
        m_shape[3].texCoords = sf::Vector2f(texCoords.convertToSfVec2()) + sf::Vector2f(0,      size.y);

        m_size = size;
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

void fe::gui::guiElement::setID(fe::str id)
    {
        m_id = id;
    }

fe::str fe::gui::guiElement::id()
    {
        return m_id;
    }
