// renderObject.hpp
// an object that represents the coordinates for verticies to be rendered
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"

#include "../../physics/transformable.hpp"
#include "../../../typeDefines.hpp"
#include "../../resourceManager/fontData.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Font.hpp>

namespace sf
    {
        class Font;
    }

namespace fe
    {
        enum RENDER_OBJECT_TYPE
            {
                NONE = 0,
                OBJECT = 1 << 0,
                TEXT = 1 << 1,
            };

        struct sceneGraphObject
            {
                fe::transformable m_transform;
                fe::transformable m_tempTransform;
                RENDER_OBJECT_TYPE m_type;
                int m_vertColour[4];  // r, g, b, a
                float m_texCoords[2]; // x, y
                int m_graphNode = -1;
                bool m_draw = true;
                bool m_static = false;

                sceneGraphObject() : m_type(NONE) {}
                virtual void setSize(float x, float y) {}
            };

        struct renderObject : public sceneGraphObject
            {
                float m_verticies[4]; // x, y, w, h

                renderObject() { m_type = OBJECT; }
                void setSize(float x, float y) { m_verticies[2] = x; m_verticies[3] = y; }
            };

        struct renderText : public sceneGraphObject
            {
                float m_verticies[FE_MAX_STRING_RENDER_SIZE * 4]; // x, y, w, h
                float m_charTexCoords[FE_MAX_STRING_RENDER_SIZE * 4]; // x, y, w, h for each text
                char m_string[FE_MAX_STRING_RENDER_SIZE];
                fe::fontData m_fontData;
                float m_size[2];
                float m_fontSize;
                unsigned int m_strSize;
                unsigned int m_strPos;

                bool m_update;

                FLAT_ENGINE_API renderText();

                FLAT_ENGINE_API void setString(const char *str);
                FLAT_ENGINE_API void appendChar(char chr);

                FLAT_ENGINE_API void computeVerticies();
                FLAT_ENGINE_API void setSize(float x, float y);
            };
    }