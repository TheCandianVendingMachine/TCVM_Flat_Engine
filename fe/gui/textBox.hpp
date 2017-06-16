// textBox.hpp
// allows user to get input
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
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
                                    ALPHABET = 1 << 0,
                                    NUMERICS = 1 << 1,
                                    DISABLE_INPUT = 1 << 2,
                                    WORD_WRAP = 1 << 3
                                };

                        protected:
                            fe::gui::label m_drawText;
                            std::string m_inputText;

                            fe::Vector2d m_lastCharPos;

                            unsigned int m_maxChars;
                            int m_maxLineHeight; // the max height of a glyph in pixels

                            bool m_allowAlpha;
                            bool m_allowNumerics;
                            bool m_allowInput;
                            bool m_wordWrap;

                            float m_paddingX;
                            float m_paddingY;
                            bool m_input;

                            FLAT_ENGINE_API void addChar(sf::Uint32 ascii, const sf::Glyph &glyph);
                            FLAT_ENGINE_API void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix);

                        public:
                            // intentionally underflowing maxChars to allow for a max amount of characters
                            FLAT_ENGINE_API textBox(fe::Vector2d size, const sf::Font &font, options opt = options(options::ALPHABET | options::NUMERICS), float textStrSize = -1.f, float padX = 10.f, float padY = 5.f, unsigned int maxChars = -1, const char *text = "");

                            FLAT_ENGINE_API void setOptions(options opt);

                            FLAT_ENGINE_API void handleEvent(const sf::Event &event);
                            FLAT_ENGINE_API void update();

                            FLAT_ENGINE_API void setString(const char *str);
                            FLAT_ENGINE_API std::string getString() const;

                            FLAT_ENGINE_API bool active();

                    };
            }
    }