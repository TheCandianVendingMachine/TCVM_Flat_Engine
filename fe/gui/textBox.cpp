#include "textBox.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Glyph.hpp>

#include "panel.hpp"

void fe::gui::textBox::checkAddChar(sf::Uint32 ascii, const sf::Glyph &glyph)
    {
        if ((m_allowAlpha && m_allowNumerics) ||
            (m_allowNumerics && (ascii >= '0' && ascii <= '9') || (ascii == '-' || ascii == '.')) ||
            (m_allowAlpha && !(ascii >= '0' && ascii <= '9')))
            {
                m_inputText += (char)ascii;
                m_lastCharPos.x += glyph.advance;
            }
    }

void fe::gui::textBox::addChar(sf::Uint32 ascii)
    {
        if (ascii == '\b')
            {
                if (m_inputText.size() > 0)
                    {
                        sf::Uint32 ascii = m_inputText[m_inputText.size() - 1];
                        m_inputText.erase(m_inputText.size() - 1, 1);
                        sf::Glyph glyph = m_drawText.getText().getFont()->getGlyph(ascii, m_drawText.getCharacterSize(), false);
                        m_lastCharPos.x -= glyph.advance;
                        m_textSize.x -= glyph.advance;
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
        else if (ascii < 128 && m_inputText.size() < m_maxChars)
            {
                sf::Glyph glyph = m_drawText.getText().getFont()->getGlyph(ascii, m_drawText.getCharacterSize(), false);
                float lineSpace = m_drawText.getText().getFont()->getLineSpacing(m_drawText.getCharacterSize());
                if (!m_wordWrap)
                    {
                        if (m_lastCharPos.x + glyph.advance <= m_size.x - m_paddingX || m_sizeToText)
                            {
                                checkAddChar(ascii, glyph);
                                m_textSize.x += glyph.advance;
                            }
                    }
                else
                    {
                        if (ascii == '\n')
                            {
                                m_lastCharPos = { m_paddingX, m_lastCharPos.y + lineSpace };
                            }

                        if (m_lastCharPos.x + glyph.advance <= m_size.x - m_paddingX && m_lastCharPos.y + lineSpace <= m_size.y - m_paddingY)
                            {
                                checkAddChar(ascii, glyph);
                            }
                        else if (m_lastCharPos.y + lineSpace <= m_size.y - m_paddingY)
                            {
                                checkAddChar('\n', glyph);
                                checkAddChar(ascii, glyph);

                                m_lastCharPos = { m_paddingX + glyph.advance, m_lastCharPos.y + lineSpace };
                            }
                    }
            }
        m_drawText.setString(m_inputText.c_str());

        if (m_sizeToText) 
            {
                float sizeX = (m_paddingX * 2.f) + m_textSize.x;
                setSize({ sizeX, getSize().y });
            }
    }

void fe::gui::textBox::drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix)
    {
        m_shape[0].position = matrix.transformPoint({ 0.f, 0.f }).convertToSfVec2();
        m_shape[1].position = matrix.transformPoint({ m_size.x, 0.f }).convertToSfVec2();
        m_shape[2].position = matrix.transformPoint({ m_size.x, m_size.y }).convertToSfVec2();
        m_shape[3].position = matrix.transformPoint({ 0.f, m_size.y }).convertToSfVec2();
        m_shape[4].position = matrix.transformPoint({ 0.f, 0.f }).convertToSfVec2();

        target.draw(m_shape);
        m_drawText.draw(target);
    }

fe::gui::textBox::textBox(fe::Vector2d size, const sf::Font &font, options opt, float textStrSize, unsigned int maxChars, float padX, float padY, const char *text) : m_drawText(font, ""), m_paddingX(padX), m_paddingY(padY)
    {
        m_allowAlpha = false;
        m_allowNumerics = false;
        m_input = false;
        m_allowInput = true;
        m_wordWrap = false;
        m_sizeToText = false;

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

        setActiveColour(sf::Color(255, 255, 255, 255));
        setInactiveColour(sf::Color(255, 255, 255, 220));
    }

void fe::gui::textBox::setOptions(options opt)
    {
        m_allowAlpha =     !(opt & options::DISABLE_ALPHABET);
        m_allowNumerics =  !(opt & options::DISABLE_NUMERICS);
        m_allowInput =     !(opt & options::DISABLE_INPUT);
        m_wordWrap =        (opt & options::WORD_WRAP);
        m_sizeToText =      (opt & options::SIZE_TO_TEXT);
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
                        if (m_parentElement)
                            {
                                m_input = m_parentPanel->mouseHover(m_parentElement->getParentTransform().transformPoint(getPosition()), m_size);
                            }
                        else
                            {
                                m_input = m_parentPanel->mouseHover(getPosition(), m_size);
                            }
                    }
            }
        else if (m_input && event.type == sf::Event::TextEntered)
            {
                addChar(event.text.unicode);
            }
    }

void fe::gui::textBox::update()
    {
        if (m_allowInput && ((m_parentElement && m_parentPanel->mouseHover(m_parentElement->getParentTransform().transformPoint(getPosition()), m_size)) || m_parentPanel->mouseHover(getPosition(), m_size) || m_input))
            {
                setActive(true);
            }
        else
            {
                setActive(false);
            }
    }

void fe::gui::textBox::setString(const char *str)
    {
        m_textSize = fe::Vector2d(0, 0);
        m_inputText = "";
        for (unsigned int i = 0; i < std::strlen(str); i++)
            {
                addChar(str[i]);
            }
    }

std::string fe::gui::textBox::getString() const
    {
        return m_drawText.getString();
    }

bool fe::gui::textBox::active()
    {
        return m_input;
    }
