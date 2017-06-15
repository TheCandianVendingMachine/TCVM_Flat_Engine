// textBox.hpp
// allows user to get input
#pragma once
#include "label.hpp"
#include "guiElement.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

namespace fe
    {
        namespace gui
            {
                class textBox : public fe::gui::guiElement
                    {
                        protected:
                            fe::gui::label m_drawText;
                            std::string m_inputText;

                            float m_paddingX;
                            float m_paddingY;
                            bool m_input;

                            FLAT_ENGINE_API void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix);

                        public:
                            FLAT_ENGINE_API textBox(fe::Vector2d size, const sf::Font &font, const char *text = "");

                            FLAT_ENGINE_API void handleEvent(const sf::Event &event);
                            FLAT_ENGINE_API void update();

                    };
            }
    }