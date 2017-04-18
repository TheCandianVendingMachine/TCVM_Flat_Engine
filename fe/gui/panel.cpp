#include "panel.hpp"
#include "guiElement.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <cstring>

fe::gui::panel::panel(fe::Vector2d size, int modifiers, const char *title, sf::Font *font) :
    m_mousePressed(false),
    m_size(size),
    m_panelColour(12, 175, 232, 75),
    m_titleBarColour(sf::Color::White),
    m_isOpen(true),
    m_canDrag(false),
    m_canClose(false),
    m_canMinimize(false),
    m_dragging(false),
    m_isFolded(false),
    m_windowOffset(0.f),
    m_minSize(30.f * ((modifiers & panelModifiers::CAN_MINIMIZE > 0) + (modifiers & panelModifiers::CAN_CLOSE > 0)), 5.f), // if both modifiers are true, then we will multiply the amount by 2
    m_buttonSize(20.f),
    m_distanceFromEnd(5.f),
    m_distanceFromTop(5.f) 
    {
        m_canMinimize |=(modifiers & panelModifiers::CAN_MINIMIZE);
        m_canClose |=   (modifiers & panelModifiers::CAN_CLOSE);
        m_canDrag |=    (modifiers & panelModifiers::CAN_DRAG);
        m_hasTitle |=   (modifiers & panelModifiers::HAS_TITLE);

        m_window.setPrimitiveType(sf::PrimitiveType::Quads);
        m_window.resize(4);

        if (m_canMinimize || m_canClose || m_canDrag || m_hasTitle)
            {
                m_titleBar.setPrimitiveType(sf::PrimitiveType::Quads);
                m_titleBar.resize(12);
                m_windowOffset = 30.f;

                m_titleBar[0].color = m_titleBarColour;
                m_titleBar[1].color = m_titleBarColour;
                m_titleBar[2].color = m_titleBarColour;
                m_titleBar[3].color = m_titleBarColour;

                if (m_canMinimize)
                    {
                        m_titleBar[4].color = sf::Color(60, 151, 255);
                        m_titleBar[5].color = sf::Color(60, 151, 255);
                        m_titleBar[6].color = sf::Color(60, 151, 255);
                        m_titleBar[7].color = sf::Color(60, 151, 255);
                    }

                if (m_canClose)
                    {
                        m_titleBar[8].color = sf::Color::Red;
                        m_titleBar[9].color = sf::Color::Red;
                        m_titleBar[10].color = sf::Color::Red;
                        m_titleBar[11].color = sf::Color::Red;
                    }

                if (m_hasTitle)
                    {
                        char croppedTitle[31];
                        std::strncpy(croppedTitle, title, 30);
                        croppedTitle[30] = '\0';

                        m_title.setString(croppedTitle);
                        m_title.setFillColor(sf::Color::Black);
                        m_title.setFont(*font);

                        m_title.setCharacterSize((m_windowOffset - 5.f) * (72.f / 96.f));
                        m_title.setPosition(5, 5);

                        m_titlePosition = m_title.getPosition();
                    }
            }

        setSize(size);
    }

void fe::gui::panel::setPanelBarColour(sf::Color colour)
    {
        m_titleBarColour = colour;
    }

void fe::gui::panel::setPanelClearColour(sf::Color colour)
    {
        m_panelColour = colour;
    }

sf::Color fe::gui::panel::getPanelBarColour() const
    {
        return m_titleBarColour;
    }

sf::Color fe::gui::panel::getPanelClearColour() const
    {
        return m_panelColour;
    }

bool fe::gui::panel::isOpen() const
    {
        return m_isOpen;
    }

unsigned int fe::gui::panel::addElement(guiElement *element)
    {
        m_guiElements.emplace_back(element);
        m_guiHandles.push_back(m_guiElements.size() - 1);
        element->setParent(this);

        return m_guiHandles.size() - 1;
    }

fe::gui::guiElement *fe::gui::panel::getElement(unsigned int handle)
    {
        if (handle >= m_guiHandles.size())
            {
                return nullptr;
            }
        return m_guiElements[m_guiHandles[handle]];
    }

void fe::gui::panel::removeElement(unsigned int handle)
    {
        if (m_guiHandles.begin() + handle < m_guiHandles.end())
            {
                delete *(m_guiElements.begin() + m_guiHandles[handle]);
                m_guiElements.erase(m_guiElements.begin() + m_guiHandles[handle]);
                for (auto it = m_guiHandles.begin() + handle; it != m_guiHandles.end(); ++it) { (*it) -= 1; };
            }
    }

bool fe::gui::panel::getMousePressed() const
    {
        return m_mousePressed;
    }

bool fe::gui::panel::mouseHover(const fe::Vector2d &position, const fe::Vector2d &size) const
    {
        // We add the window offset so we can get the correct result if we have a title bar
        return m_mousePosition.x - getPosition().x > position.x          && m_mousePosition.y - getPosition().y > position.y + m_windowOffset &&
               m_mousePosition.x - getPosition().x < position.x + size.x && m_mousePosition.y - getPosition().y < position.y + size.y + m_windowOffset;
    }

void fe::gui::panel::setSize(fe::Vector2d size)
    {
        size.x = m_minSize.x < size.x ? size.x : m_minSize.x;
        size.y = m_minSize.y < size.y ? size.y : m_minSize.y;

        m_texture.create(size.x, size.y);

        m_window[0].texCoords = fe::Vector2d(0.f, 0.f).convertToSfVec2();
        m_window[1].texCoords = fe::Vector2d(size.x, 0.f).convertToSfVec2();
        m_window[2].texCoords = fe::Vector2d(size.x, size.y).convertToSfVec2();
        m_window[3].texCoords = fe::Vector2d(0.f, size.y).convertToSfVec2();

        m_size = size;
    }

fe::Vector2d fe::gui::panel::getSize() const
    {
        return m_size;
    }

void fe::gui::panel::handleEvent(const sf::Event &event)
    {
        switch (event.type)
            {
                case sf::Event::MouseButtonPressed:
                    m_mousePressed = true;
                    break;
                case sf::Event::MouseButtonReleased:
                    m_mousePressed = false;
                    {
                        fe::Vector2d minButtonPos(m_size.x - m_distanceFromEnd - m_buttonSize, m_distanceFromTop);
                        fe::Vector2d maxButtonPos(m_size.x - m_distanceFromEnd, m_windowOffset - m_distanceFromTop);

                        float distanceFromEnd = m_distanceFromEnd;
                        if (m_mousePosition.x - getPosition().x > minButtonPos.x && m_mousePosition.y - getPosition().y > minButtonPos.y &&
                            m_mousePosition.x - getPosition().x < maxButtonPos.x && m_mousePosition.y - getPosition().y < maxButtonPos.y && m_canClose)
                            {
                                m_isOpen = false;
                                destroy();
                            }

                        if (m_canClose)
                            {
                                distanceFromEnd += m_buttonSize + m_distanceFromEnd;
                            }

                        minButtonPos = fe::Vector2d(m_size.x - distanceFromEnd - m_buttonSize, m_distanceFromTop);
                        maxButtonPos = fe::Vector2d(m_size.x - distanceFromEnd, m_windowOffset - m_distanceFromTop);

                        if (m_mousePosition.x - getPosition().x > minButtonPos.x && m_mousePosition.y - getPosition().y > minButtonPos.y &&
                            m_mousePosition.x - getPosition().x < maxButtonPos.x && m_mousePosition.y - getPosition().y < maxButtonPos.y && m_canMinimize)
                            {
                                m_isFolded = !m_isFolded;
                            }
                    }
                    break;
                case sf::Event::MouseMoved:
                    m_mousePosition = m_texture.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y });
                    break;
                default:
                    break;
            }

        if (!m_isFolded)
            {
                for (auto &element : m_guiElements)
                    {
                        element->handleEvent(event);
                    }
            }
    }

void fe::gui::panel::update()
    {
        if ((m_canDrag && m_mousePressed &&
            m_mousePosition.x - getPosition().x > 0.f      && m_mousePosition.y - getPosition().y > 0.f &&
            m_mousePosition.x - getPosition().x < m_size.x && m_mousePosition.y - getPosition().y < m_windowOffset) || m_dragging)
            {
                if (!m_dragging)
                    {
                        m_dragging = true;
                        m_clickOffset = m_mousePosition - getPosition();
                    }
                setPosition(m_mousePosition - m_clickOffset);
            }

        if (!m_mousePressed && m_dragging)
            {
                m_dragging = false;
            }

        if (!m_isFolded)
            {
                for (auto &element : m_guiElements)
                    {
                        element->update();
                    }
            }
    }

void fe::gui::panel::draw(sf::RenderTarget &target)
    {
        auto &matrix = getMatrix();
        if (m_canClose || m_canDrag || m_canMinimize || m_hasTitle)
            {
                m_titleBar[0].position = matrix.transformPoint({ 0.f, 0.f }).convertToSfVec2();
                m_titleBar[1].position = matrix.transformPoint({ m_size.x, 0.f }).convertToSfVec2();
                m_titleBar[2].position = matrix.transformPoint({ m_size.x, m_windowOffset }).convertToSfVec2();
                m_titleBar[3].position = matrix.transformPoint({ 0.f, m_windowOffset }).convertToSfVec2();

                float distanceFromEnd = m_distanceFromEnd; // distance from the far right side of panel
                if (m_canClose)
                    {
                        m_titleBar[8].position  = matrix.transformPoint({ m_size.x - m_distanceFromEnd - m_buttonSize, m_distanceFromTop }).convertToSfVec2();
                        m_titleBar[9].position  = matrix.transformPoint({ m_size.x - m_distanceFromEnd, m_distanceFromTop }).convertToSfVec2();
                        m_titleBar[10].position = matrix.transformPoint({ m_size.x - m_distanceFromEnd, m_windowOffset - m_distanceFromTop }).convertToSfVec2();
                        m_titleBar[11].position = matrix.transformPoint({ m_size.x - m_distanceFromEnd - m_buttonSize, m_windowOffset - m_distanceFromTop }).convertToSfVec2();

                        distanceFromEnd += m_buttonSize + m_distanceFromEnd;
                    }

                if (m_canMinimize) 
                    {
                        m_titleBar[4].position = matrix.transformPoint({ m_size.x - distanceFromEnd - m_buttonSize, m_distanceFromTop }).convertToSfVec2();
                        m_titleBar[5].position = matrix.transformPoint({ m_size.x - distanceFromEnd, m_distanceFromTop }).convertToSfVec2();
                        m_titleBar[6].position = matrix.transformPoint({ m_size.x - distanceFromEnd, m_windowOffset - m_distanceFromTop }).convertToSfVec2();
                        m_titleBar[7].position = matrix.transformPoint({ m_size.x - distanceFromEnd - m_buttonSize, m_windowOffset - m_distanceFromTop }).convertToSfVec2();
                    }

                m_title.setPosition(matrix.transformPoint(m_titlePosition).convertToSfVec2());

                m_window[0].position = matrix.transformPoint({ 0.f, m_windowOffset }).convertToSfVec2();
                m_window[1].position = matrix.transformPoint({ m_size.x, m_windowOffset }).convertToSfVec2();
                m_window[2].position = matrix.transformPoint({ m_size.x, m_size.y + m_windowOffset }).convertToSfVec2();
                m_window[3].position = matrix.transformPoint({ 0.f, m_size.y + m_windowOffset }).convertToSfVec2();
            }
        else
            {
                m_window[0].position = matrix.transformPoint({ 0.f, 0.f }).convertToSfVec2();
                m_window[1].position = matrix.transformPoint({ m_size.x, 0.f }).convertToSfVec2();
                m_window[2].position = matrix.transformPoint({ m_size.x, m_size.y }).convertToSfVec2();
                m_window[3].position = matrix.transformPoint({ 0.f, m_size.y }).convertToSfVec2();
            }

        if (!m_isFolded) 
            {
                m_texture.clear(m_panelColour);
                for (auto &element : m_guiElements)
                    {
                        element->draw(m_texture);
                    }
                m_texture.display();

                sf::RenderStates state;
                state.texture = &m_texture.getTexture();

                target.draw(m_window, state);
            }
        target.draw(m_titleBar);
        target.draw(m_title);
    }

void fe::gui::panel::destroy()
    {
        while(m_guiElements.size() > 0)
            {
                delete m_guiElements.back();
                m_guiElements.pop_back();
            }
    }

fe::gui::panel::~panel()
    {
        destroy();
    }
