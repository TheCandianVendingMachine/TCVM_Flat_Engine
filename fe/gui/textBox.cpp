#include "textBox.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Glyph.hpp>

#include "panel.hpp"

void fe::gui::textBox::addChar(sf::Uint32 ascii, const sf::Glyph &glyph)
    {
        if ((m_allowAlpha && m_allowNumerics) ||
            (m_allowNumerics && ascii >= '0' && ascii <= '9') ||
            (m_allowAlpha && !(ascii >= '0' && ascii <= '9')))
            {
                m_inputText += (char)ascii;
                m_lastCharPos.x += glyph.advance;
            }
    }

void fe::gui::textBox::drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix)
    {
        auto localMatrix = getMatrix() * matrix;
        m_shape[0].position = localMatrix.transformPoint({ 0.f, 0.f }).convertToSfVec2();
        m_shape[1].position = localMatrix.transformPoint({ m_size.x, 0.f }).convertToSfVec2();
        m_shape[2].position = localMatrix.transformPoint({ m_size.x, m_size.y }).convertToSfVec2();
        m_shape[3].position = localMatrix.transformPoint({ 0.f, m_size.y }).convertToSfVec2();
        m_shape[4].position = localMatrix.transformPoint({ 0.f, 0.f }).convertToSfVec2();

        target.draw(m_shape);
        m_drawText.draw(target);
    }

fe::gui::textBox::textBox(fe::Vector2d size, const sf::Font &font, options opt, float textStrSize, float padX, float padY, unsigned int maxChars, const char *text) : m_drawText(font, ""), m_paddingX(padX), m_paddingY(padY)
    {
        m_allowAlpha = false;
        m_allowNumerics = false;
        m_input = false;
        m_allowInput = false;
        m_wordWrap = false;

        m_maxChars = maxChars;

        setOptions(opt);

        m_drawText.setParent(this);
        m_drawText.setPosition({ m_paddingX, m_paddingY });
        m_lastCharPos = { m_paddingX, m_paddingY + m_drawText.getText().getFont()->getLineSpacing(m_drawText.getCharacterSize()) };
        if (textStrSize < 0.f) 
            {
                m_drawText.setPixelSize(size.y - (m_paddingY * 2));
            }
        else
            {
                m_drawText.setPixelSize(textStrSize);
            }
        m_inputText = text;
        setString(text);

        m_shape.setPrimitiveType(sf::PrimitiveType::LinesStrip);
        m_shape.resize(5);

        m_size = size;
    }

void fe::gui::textBox::setOptions(options opt)
    {
        m_allowAlpha =      (opt & options::ALPHABET);
        m_allowNumerics =   (opt & options::NUMERICS);
        m_allowInput =     !(opt & options::DISABLE_INPUT);
        m_wordWrap =        (opt & options::WORD_WRAP);
    }

void fe::gui::textBox::handleEvent(const sf::Event &event)
    {
        if (!m_allowInput) return;

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
                                sf::Uint32 ascii = m_inputText[m_inputText.size() - 1];
                                m_inputText.erase(m_inputText.size() - 1, 1);
                                auto glyph = m_drawText.getText().getFont()->getGlyph(ascii, m_drawText.getCharacterSize(), false);
                                m_lastCharPos.x -= glyph.advance;
                                if (m_lastCharPos.x < m_paddingX && !m_inputText.empty())
                                    {
                                        auto lineSpace = m_drawText.getText().getFont()->getLineSpacing(m_drawText.getCharacterSize());
                                        m_lastCharPos.y -= lineSpace;

                                        auto glyphNew = m_drawText.getText().getFont()->getGlyph(m_inputText[m_inputText.size() - 1], m_drawText.getCharacterSize(), false);
                                        m_lastCharPos.x = m_size.x - m_paddingX - glyphNew.advance;
                                    }
                                else if (m_inputText.empty())
                                    {
                                        m_lastCharPos.x = m_paddingX;
                                        m_lastCharPos.y = m_paddingY;
                                    }
                            }
                    }
                else if (event.text.unicode < 128 && m_inputText.size() < m_maxChars)
                    {
                        auto glyph = m_drawText.getText().getFont()->getGlyph(event.text.unicode, m_drawText.getCharacterSize(), false);
                        auto lineSpace = m_drawText.getText().getFont()->getLineSpacing(m_drawText.getCharacterSize());
                        if (!m_wordWrap)
                            {
                                if (m_lastCharPos.x + glyph.advance <= m_size.x - m_paddingX)
                                    {
                                         addChar(event.text.unicode, glyph);
                                    }
                            }
                        else
                            {
                                if (m_lastCharPos.x + glyph.advance <= m_size.x - m_paddingX)
                                    {
                                         addChar(event.text.unicode, glyph);
                                    }
                                else if (m_lastCharPos.y + lineSpace < m_size.y - m_paddingY)
                                    {
                                        addChar('\n', glyph);
                                        addChar(event.text.unicode, glyph);

                                        m_lastCharPos = { m_paddingX + glyph.advance, m_lastCharPos.y + lineSpace };
                                    }
                            }
                    }
                m_drawText.setString(m_inputText.c_str());
            }
    }

void fe::gui::textBox::update()
    {
    
    }

void fe::gui::textBox::setString(const char *str)
    {
        m_inputText = str;
    }

std::string fe::gui::textBox::getString() const
    {
        return m_drawText.getString();
    }

bool fe::gui::textBox::active()
    {
        return m_input;
    }
