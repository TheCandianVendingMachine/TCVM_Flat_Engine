// renderText.hpp
// Text that can be rendered on an entity seperate from the GUI system
#pragma once
#include <SFML/Graphics/Text.hpp>

namespace sf
    {
        class Font;
    }

namespace fe
    {
        struct renderText
            {
                sf::Text m_text;
            };
    }