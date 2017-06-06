// label.hpp
// A text-label that can be added to the world
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"

#include "guiElement.hpp"
#include <SFML/Graphics/Text.hpp>

namespace sf
    {
        class Font;
    }

namespace fe
    {
        namespace gui
            {
                class label : public guiElement
                    {
                        private:
                            sf::Text m_text;
                            fe::Vector2d m_textPosition;

                            FLAT_ENGINE_API void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix);

                        public:
                            FLAT_ENGINE_API label(const sf::Font &font, const char *text);
                            
                            FLAT_ENGINE_API void setPosition(fe::Vector2d pos);
                            FLAT_ENGINE_API fe::Vector2d getPosition() const;

                            FLAT_ENGINE_API void setCharacterSize(float point);
                            FLAT_ENGINE_API float getCharacterSize() const;

                            FLAT_ENGINE_API void setPixelSize(float pixel);
                            FLAT_ENGINE_API float getPixelSize() const;

                            FLAT_ENGINE_API void setString(const char *str);
                            FLAT_ENGINE_API std::string getString() const;

                            FLAT_ENGINE_API void setColour(sf::Color colour);
                            FLAT_ENGINE_API sf::Color getColour();

                            FLAT_ENGINE_API void handleEvent(const sf::Event &event) {}
                            FLAT_ENGINE_API void update() {}
                    };
            }
    }