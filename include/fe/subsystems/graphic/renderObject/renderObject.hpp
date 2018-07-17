// renderObject.hpp
// an object that represents the coordinates for verticies to be rendered
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"

#include "../../physics/transformable.hpp"
#include "../../../typeDefines.hpp"
#include "../../resourceManager/fontData.hpp"
#include "../../serializer/serializable.hpp"
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

        struct sceneGraphObject : public fe::serializable
            {
                fe::transformable m_transform;
                fe::transformable m_tempTransform;
                RENDER_OBJECT_TYPE m_type;
                int m_vertColour[4];  // r, g, b, a
                float m_texCoords[4]; // x, y, w, h
                int m_graphNode = -1;
                int m_zPosition;
                bool m_draw = true;
                bool m_static = false;

                sceneGraphObject() : m_type(NONE) {}
                virtual void setSize(float x, float y) {}

                FLAT_ENGINE_API void initialize(fe::lightVector2<unsigned int> texturePos, int z);

                FLAT_ENGINE_API virtual void serialize(fe::serializerID &serializer) const;
                FLAT_ENGINE_API virtual void deserialize(fe::serializerID &serializer);
            };

        struct renderObject : public sceneGraphObject
            {
                float m_verticies[4]; // x, y, w, h

#if _DEBUG
				fe::uInt16 m_debugHeader = 0xDEAD;
#endif

                renderObject() : m_verticies{ 0.f, 0.f, 0.f, 0.f } { m_type = OBJECT; }
                void setSize(float x, float y) { m_verticies[2] = x; m_verticies[3] = y; }

                FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);
            };

        struct renderText : public sceneGraphObject
            {
                float m_verticies[FE_MAX_STRING_RENDER_SIZE * 4]; // x, y, w, h
                float m_charTexCoords[FE_MAX_STRING_RENDER_SIZE * 4]; // x, y, w, h for each text
                char m_string[FE_MAX_STRING_RENDER_SIZE];
                fe::fontData m_fontData;
                float m_size[2];
                unsigned int m_fontSize;
                unsigned int m_strSize;
                unsigned int m_strPos;

#if _DEBUG
				fe::uInt16 m_debugHeader = 0xBEEF;
#endif

                bool m_update;

                FLAT_ENGINE_API renderText();

                FLAT_ENGINE_API void setString(const char *str);
                FLAT_ENGINE_API void appendChar(char chr);

                FLAT_ENGINE_API void computeVerticies();
                FLAT_ENGINE_API void setSize(float x, float y);

                //FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                //FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);

                SERIALIZE_ID(   "renderText",
                                "type", m_type,
                                "vertColour", m_vertColour,
                                "texCoords", m_texCoords,
                                "graphNode", m_graphNode,
                                "zPosition", m_zPosition,
                                "draw", m_draw,
                                "static", m_static,
                                "verticies", m_verticies,
                                "charTexCoords", m_charTexCoords,
                                "string", m_string,
                                "fontData", m_fontData,
                                "size", m_size,
                                "fontSize", m_fontSize,
                                "stringSize", m_strSize,
                                "strPos", m_strPos);
            };
    }