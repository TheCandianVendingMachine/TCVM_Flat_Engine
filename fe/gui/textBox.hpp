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
                        public:
                            enum options
                                {
                                    ALPHA = 1 << 0,
                                    NUM = 1 << 1,
                                };

                        protected:
                            fe::gui::label m_drawText;
                            std::string m_inputText;

                            unsigned int m_maxChars;

                            bool m_allowAlpha;
                            bool m_allowNumerics;

                            float m_paddingX;
                            float m_paddingY;
                            bool m_input;

                            FLAT_ENGINE_API void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix);

                        public:
                            // intentionally underflowing maxChars to allow for a max amount of characters
                            FLAT_ENGINE_API textBox(fe::Vector2d size, const sf::Font &font, options opt = options(options::ALPHA | options::NUM), float padX = 10.f, float padY = 5.f, unsigned int maxChars = -1, const char *text = "");

                            FLAT_ENGINE_API void setOptions(options opt);

                            FLAT_ENGINE_API void handleEvent(const sf::Event &event);
                            FLAT_ENGINE_API void update();

                            FLAT_ENGINE_API void setString(const char *str);
                            FLAT_ENGINE_API std::string getString() const;

                            FLAT_ENGINE_API bool active();

                    };
            }
    }