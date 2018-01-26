// fontData.hpp
// Data regarding bitmap fonts
#pragma once
#include "../serializer/serializerID.hpp"

namespace sf
    {
        class Texture;
        class Font;
    }

namespace fe
    {
        struct fontData
            {
                fe::Vector2<unsigned int> m_positions[FE_CHAR_END - FE_CHAR_START];
                const sf::Texture *m_texture = nullptr;
                const sf::Font *m_font = nullptr; // The font which corresponds to this bitmap
                unsigned int m_charSize = 0;

                SERIALIZE_ID(   "fontData",
                                "charSize", m_charSize);
            };
    }