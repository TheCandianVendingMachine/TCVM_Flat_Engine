// label.hpp
// A text-label that can be added to the world
#pragma once
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

                            void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix);

                        public:
                            label(sf::Font &font, const char *text);
                            
                            void setPosition(fe::Vector2d pos);
                            fe::Vector2d getPosition() const;

                            void setCharacterSize(float point);
                            float getCharacterSize() const;

                            void setPixelSize(float pixel);
                            float getPixelSize() const;

                            void setString(const char *str);
                            std::string getString() const;

                            void setColour(sf::Color colour);
                            sf::Color getColour();

                            void handleEvent(const sf::Event &event) {}
                            void update() {}
                    };
            }
    }