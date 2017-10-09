// renderObject.hpp
// an object that represents the coordinates for verticies to be rendered
#pragma once
#include "../../physics/transformable.hpp"
#include <SFML/Graphics/Text.hpp>

namespace sf
    {
        class Font;
    }

namespace fe
    {
        struct sceneGraphObject
            {
                fe::transformable m_transform;
                int m_vertColour[4];  // r, g, b, a
                int m_graphNode = -1;
                bool m_draw = true;
                bool m_static = false;

                virtual void setSize(float x, float y) {}
            };

        struct renderObject : public sceneGraphObject
            {
                float m_verticies[4]; // x, y, w, h
                float m_texCoords[2]; // x, y

                void setSize(float x, float y) { m_verticies[2] = x; m_verticies[3] = y; }
            };

        struct renderText : public sceneGraphObject
            {
                sf::Text m_text;
                void setSize(float x, float y) { }
            };
    }