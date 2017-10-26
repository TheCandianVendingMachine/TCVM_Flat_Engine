#include "renderObject.hpp"
#include "../../../debug/logger.hpp"
#include <cstring>

fe::renderText::renderText() : 
    m_strPos(0),
    m_strSize(0),
    m_charSize(0.f),
    m_font(nullptr),
    m_verticies(sf::PrimitiveType::Triangles)
    {
        std::memset(m_string, 0, FE_MAX_STRING_RENDER_SIZE);
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
    }

void fe::renderText::computeVerticies(const fe::matrix3d &matrix)
    {
        m_verticies.clear();

        float header = static_cast<float>(m_font->getGlyph(' ', m_charSize, 0.f).advance);
        float lineSpace = static_cast<float>(m_font->getLineSpacing(m_charSize));
        float x = matrix.transformPoint({ 0.f, m_charSize }).x;
        float y = matrix.transformPoint({ 0.f, m_charSize }).y;

        sf::Color textColour(m_vertColour[0], m_vertColour[1], m_vertColour[2], m_vertColour[3]);

        char prevChar = '\0';
        for (unsigned int i = 0; i < m_strSize; i++)
            {
                char curChar = m_string[i];
                const sf::Glyph &glyph = m_font->getGlyph(curChar, m_charSize, 0.f);

                x += m_font->getKerning(prevChar, curChar, m_charSize);
                

                float left      = glyph.bounds.left;
                float top       = glyph.bounds.top;
                float right     = glyph.bounds.left + glyph.bounds.width;
                float bottom    = glyph.bounds.top + glyph.bounds.height;

                float texturePX = m_texCoords[0] + glyph.textureRect.left;
                float texturePY = m_texCoords[1] + glyph.textureRect.top;
                float textureW = m_texCoords[0] + glyph.textureRect.left + glyph.textureRect.width;
                float textureH = m_texCoords[1] + glyph.textureRect.top + glyph.textureRect.height;

                m_verticies.append(sf::Vertex(sf::Vector2f(x + left,    y + top),       textColour, sf::Vector2f(texturePX, texturePY)));
                m_verticies.append(sf::Vertex(sf::Vector2f(x + right,   y + top),       textColour, sf::Vector2f(textureW, texturePY)));
                m_verticies.append(sf::Vertex(sf::Vector2f(x + left,    y + bottom),    textColour, sf::Vector2f(texturePX, textureH)));

                m_verticies.append(sf::Vertex(sf::Vector2f(x + left,    y + bottom),    textColour, sf::Vector2f(texturePX, textureH)));
                m_verticies.append(sf::Vertex(sf::Vector2f(x + right,   y + top),       textColour, sf::Vector2f(textureW, texturePY)));
                m_verticies.append(sf::Vertex(sf::Vector2f(x + right,   y + bottom),    textColour, sf::Vector2f(textureW, textureH)));

                x += glyph.advance;
                prevChar = curChar;
            }
    }
