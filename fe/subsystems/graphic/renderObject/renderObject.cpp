#include "renderObject.hpp"
#include "../../../debug/logger.hpp"
#include "../../../feAssert.hpp"
#include <cstring>

fe::renderText::renderText() : 
    m_strPos(0),
    m_strSize(0),
    m_fontSize(16),
    m_update(true)
    {
        std::memset(m_string, 0, FE_MAX_STRING_RENDER_SIZE);
        m_size[0] = 0.f;
        m_size[1] = 0.f;
        m_type = TEXT;
    }

void fe::renderText::setString(const char *str)
    {
        std::memset(m_string, 0, FE_MAX_STRING_RENDER_SIZE);

        m_strSize = 0;
        m_strPos = 0;

        int i = 0;
        char current = str[i++];
        while (current != '\0')
            {
                appendChar(current);
                current = str[i++];
            }
    }

void fe::renderText::appendChar(char chr)
    {
        m_strSize++;
        if (m_strSize == FE_MAX_STRING_RENDER_SIZE)
            {
                FE_LOG_WARNING("String size exceeds FE_MAX_STRING_RENDER_SIZE");
                return;
            }
        m_string[m_strPos++] = chr;
        m_update = true;
    }

void fe::renderText::computeVerticies()
    {
        FE_ASSERT(m_fontData.m_font && m_fontData.m_texture, "Font Data not set");

        if (!m_update) return;
        m_update = false;

        float x = 0.f;
        float y = m_fontData.m_charSize;

        unsigned int index = 0;

        char prevChar = '\0';
        for (unsigned int i = 0; i < m_strSize; i++)
            {
                char curChar = m_string[i];
                const sf::Glyph &glyph = m_fontData.m_font->getGlyph(curChar, m_fontData.m_charSize, 0.f);
                const sf::Glyph &posGlyh = m_fontData.m_font->getGlyph(curChar, m_fontSize, 0.f);
                x += m_fontData.m_font->getKerning(prevChar, curChar, m_fontSize);

                m_verticies[index + 0] = std::ceil(x + posGlyh.bounds.left);
                m_verticies[index + 1] = std::ceil(y + posGlyh.bounds.top);
                m_verticies[index + 2] = std::ceil(posGlyh.bounds.width);
                m_verticies[index + 3] = std::ceil(posGlyh.bounds.height);

                m_charTexCoords[index + 0] = std::ceil(m_fontData.m_positions[curChar - FE_CHAR_START].x);
                m_charTexCoords[index + 1] = std::ceil(m_fontData.m_positions[curChar - FE_CHAR_START].y);
                m_charTexCoords[index + 2] = std::ceil(glyph.textureRect.width);
                m_charTexCoords[index + 3] = std::ceil(glyph.textureRect.height);

                x += posGlyh.advance;
                prevChar = curChar;
                index += 4;
            }
        m_size[0] = x;
        m_size[1] = (m_fontSize * 96.f) / 72.f;
    }

void fe::renderText::setSize(float x, float y)
    {
        m_fontSize = (y * (72.f / 96.f));
        m_update = true;
    }
