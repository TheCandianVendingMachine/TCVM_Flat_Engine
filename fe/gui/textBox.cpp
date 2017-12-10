#include "textBox.hpp"
#include "../engine.hpp"
#include "../subsystems/messaging/eventSender.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Glyph.hpp>

#include "panel.hpp"

bool fe::gui::textBox::checkAddChar(sf::Uint32 ascii, const sf::Glyph &glyph)
    {
        if ((m_allowAlpha && m_allowNumerics) ||
            (m_allowNumerics && (ascii >= '0' && ascii <= '9') || (ascii == '-' || ascii == '.')) ||
            (m_allowAlpha && !(ascii >= '0' && ascii <= '9')))
            {
                m_inputText += (char)ascii;
                m_lastCharPos.x += glyph.advance;
                return true;
            }
        return false;
    }

bool fe::gui::textBox::addChar(sf::Uint32 ascii)
    {
        bool added = false;
        if (ascii == '\b')
            {
                if (m_inputText.size() > 0)
                    {
                        sf::Uint32 ascii = m_inputText[m_inputText.size() - 1];
                        m_inputText.erase(m_inputText.size() - 1, 1);
                        sf::Glyph glyph = m_drawText.getText().getFont()->getGlyph(ascii, m_drawText.getCharacterSize(), false);
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
        else if (ascii < 128 && m_inputText.size() < m_maxChars)
            {
                sf::Glyph glyph = m_drawText.getText().getFont()->getGlyph(ascii, m_drawText.getCharacterSize(), false);
                float lineSpace = m_drawText.getText().getFont()->getLineSpacing(m_drawText.getCharacterSize());
                if (!m_wordWrap)
                    {
                        if (m_lastCharPos.x + glyph.advance <= m_size.x - m_paddingX)
                            {
                                added |= checkAddChar(ascii, glyph);
                            }
                    }
                else
                    {
                        if (ascii == '\n')
                            {
                                m_lastCharPos = { m_paddingX, m_lastCharPos.y + lineSpace };
                            }

                        if (m_lastCharPos.y <= m_size.y - m_paddingY)
                            {
                                if (!m_sizeToText)
                                    {
                                        if (m_lastCharPos.x + glyph.advance <= m_size.x - m_paddingX)
                                            {
                                                added |= checkAddChar(ascii, glyph);
                                                if (m_inputText.size() == 1)
                                                    {
                                                        m_lastCharPos.y = m_lastCharPos.y + lineSpace;
                                                    }
                                            }
                                        else if (m_lastCharPos.y + lineSpace <= m_size.y - m_paddingY)
                                            {
                                                added |= checkAddChar('\n', glyph);
                                                added |= checkAddChar(ascii, glyph);

                                                m_lastCharPos = { m_paddingX + glyph.advance, m_lastCharPos.y + lineSpace };
                                            }
                                    }
                                else
                                    {
                                        if (m_lastCharPos.x + glyph.advance <= m_maxSize.x - m_paddingX) // we limit in the X direction for style choice
                                            {
                                                added |= checkAddChar(ascii, glyph);
                                                if (m_inputText.size() == 1)
                                                    {
                                                        m_lastCharPos.y = m_lastCharPos.y + lineSpace;
                                                    }
                                            }
                                        else
                                            {
                                                added |= checkAddChar('\n', glyph);
                                                added |= checkAddChar(ascii, glyph);

                                                m_lastCharPos = { m_paddingX + glyph.advance, m_lastCharPos.y + lineSpace };
                                            }
                                    }
                            }
                    }
            }
        m_drawText.setString(m_inputText.c_str());

        m_characterBoundingBox.x = std::max(m_characterBoundingBox.x, m_lastCharPos.x);
        m_characterBoundingBox.y = m_lastCharPos.y;
        if (m_sizeToText)
            {
                setSize({ m_characterBoundingBox.x + (m_paddingX * 2.f), m_characterBoundingBox.y + (m_paddingY * 2.f) });
            }

        return added;
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

fe::gui::textBox::textBox(fe::Vector2d size, const sf::Font &font, options opt, float textStrSize, unsigned int maxChars, float padX, float padY, const char *text) : 
    m_drawText(font, ""),
    m_paddingX(padX),
    m_paddingY(padY),
    m_allowAlpha(false),
    m_allowNumerics(false),
    m_input(false),
    m_allowInput(false),
    m_wordWrap(false),
    m_sizeToText(false),
    m_maxChars(maxChars),
    m_inputText(text),
    m_maxSize(size),
    m_characterBoundingBox(m_paddingX * 2.f, m_paddingY * 2.f)
    {
        m_size = size;
        setOptions(opt);
        m_drawText.setParent(this);
        m_drawText.setPosition({ m_paddingX, m_paddingY });
        
        if (textStrSize < 0.f) 
            {
                m_drawText.setPixelSize(size.y - (m_paddingY * 2));
            }
        else
            {
                m_drawText.setPixelSize(textStrSize);
            }
        m_lastCharPos = { m_paddingX, m_paddingY + font.getLineSpacing(m_drawText.getCharacterSize()) };
        setString(text);

        m_shape.setPrimitiveType(sf::PrimitiveType::LinesStrip);
        m_shape.resize(5);

        setActiveColour(sf::Color(255, 255, 255, 255));
        setInactiveColour(sf::Color(255, 255, 255, 220));
    }

fe::gui::textBox::textBox(fe::Vector2d size, const sf::Font &font, options opt, float textStrSize, unsigned int maxChars, float padX, float padY, const std::string &text) : 
    textBox(size, font, opt, textStrSize, maxChars, padX, padY, text.c_str())
    {}

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
        else if (m_input && event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Return)
                    {
                        fe::gameEvent event(m_event, 1);
                        event.eventType = m_event;
                        event.args[0].arg.TYPE_VOIDP = &m_inputText;
                        event.args[0].argType = gameEventArgument::type::TYPE_VOIDP;
                        fe::engine::get().getEventSender().sendEngineEvent(event, m_event);
                        m_input = false;
                    }
            }
    }

void fe::gui::textBox::update()
    {
        if (m_updateSize)
            {
                setSize({ m_characterBoundingBox.x + (m_paddingX * 2.f), m_characterBoundingBox.y + (m_paddingY * 2.f) });
                m_updateSize = false;
            }

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
        m_inputText = "";
        for (unsigned int i = 0; i < std::strlen(str); i++)
            {
                if (!addChar(str[i]))
                    {
                        break;
                    }
            }
    }

std::string fe::gui::textBox::getString() const
    {
        return m_drawText.getString();
    }

void fe::gui::textBox::setSize(const fe::Vector2d &size)
    {
        if (m_sizeToText)
            {
                m_updateSize = true;
            }
        fe::gui::guiElement::setSize(size);
    }

bool fe::gui::textBox::active()
    {
        return m_input;
    }
