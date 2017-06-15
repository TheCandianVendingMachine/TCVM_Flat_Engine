#include "textBox.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include "panel.hpp"

void fe::gui::textBox::drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix)
    {
        auto localMatrix = getMatrix() * matrix;
        m_shape[0].position = localMatrix.transformPoint({ 0.f, 0.f }).convertToSfVec2();
        m_shape[1].position = localMatrix.transformPoint({ m_size.x, 0.f }).convertToSfVec2();
        m_shape[2].position = localMatrix.transformPoint({ m_size.x, m_size.y }).convertToSfVec2();
        m_shape[3].position = localMatrix.transformPoint({ 0.f, m_size.y }).convertToSfVec2();

        target.draw(m_shape);
        m_drawText.draw(target);
    }

fe::gui::textBox::textBox(fe::Vector2d size, const sf::Font &font, options opt, unsigned int maxChars, const char *text) : m_drawText(font, text), m_paddingX(10.f), m_paddingY(3.f)
    {
        m_allowAlpha = false;
        m_allowNumerics = false;
        m_input = false;

        m_maxChars = maxChars;

        setOptions(opt);

        m_drawText.setParent(this);
        m_drawText.setPosition({ m_paddingX, m_paddingY });
        m_drawText.setPixelSize(size.y - (m_paddingY * 2));

        m_shape.setPrimitiveType(sf::PrimitiveType::LinesStrip);
        m_shape.resize(4);

        m_size = size;
    }

void fe::gui::textBox::setOptions(options opt)
    {
        m_allowAlpha =      (opt & options::ALPHA);
        m_allowNumerics =   (opt & options::NUM);
    }

void fe::gui::textBox::handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (m_input)
                    {
                        m_input = false;
                    }
                else
                    {
                        m_input = m_parentPanel->mouseHover(getPosition(), m_size);
                    }
            }
        else if (m_input && event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b')
                    {
                        if (m_inputText.size() > 0)
                            {
                                m_inputText.erase(m_inputText.size() - 1, 1);
                            }
                    }
                else if (event.text.unicode < 128 && m_inputText.size() < m_maxChars)
                    {
                         if ((m_allowAlpha && m_allowNumerics) ||
                             (m_allowNumerics && event.text.unicode >= '0' && event.text.unicode <= '9') ||
                             (m_allowAlpha && !(event.text.unicode >= '0' && event.text.unicode <= '9')))
                            {
                                m_inputText += (char)event.text.unicode;
                            }
                    }
                m_drawText.setString(m_inputText.c_str());

                while (m_drawText.getSize().x + m_drawText.getPosition().x + m_paddingX > m_size.x - m_paddingX)
                    {
                        auto str = m_drawText.getString();
                        str.erase(str.begin());
                        m_drawText.setString(str.c_str());
                    }
            }
    }

void fe::gui::textBox::update()
    {
    
    }

void fe::gui::textBox::setString(const char *str)
    {
        m_drawText.setString(str);
    }

std::string fe::gui::textBox::getString() const
    {
        return m_drawText.getString();
    }

bool fe::gui::textBox::active()
    {
        return m_input;
    }
