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
                                    DISABLE_ALPHABET    = 1 << 0,
                                    DISABLE_NUMERICS    = 1 << 1,
                                    DISABLE_INPUT       = 1 << 2,
                                    WORD_WRAP           = 1 << 3,
                                    SIZE_TO_TEXT        = 1 << 4,
                                };

                            friend options operator|(const options &lhs, const options &rhs)
                                {
                                    return (options)((int)lhs | (int)rhs);
                                }

                        protected:
                            fe::gui::label m_drawText;
                            std::string m_inputText;
                            std::string m_inputTextShown;

                            const fe::Vector2d m_maxSize;
                            fe::Vector2d m_characterBoundingBox;
                            fe::Vector2d m_lastCharPos;

                            unsigned int m_maxChars;
                            int m_maxLineHeight; // the max height of a glyph in pixels

                            bool m_allowAlpha;
                            bool m_allowNumerics;
                            bool m_allowInput;
                            bool m_wordWrap;
                            bool m_sizeToText;

                            float m_paddingX;
                            float m_paddingY;
                            bool m_input;
                            bool m_updateSize;

                            FLAT_ENGINE_API bool checkAddChar(sf::Uint32 ascii, const sf::Glyph &glyph);
                            FLAT_ENGINE_API bool addChar(sf::Uint32 ascii);

                            FLAT_ENGINE_API void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix);

                        public:
                            // intentionally underflowing maxChars to allow for a max amount of characters
                            FLAT_ENGINE_API textBox(fe::Vector2d size, const sf::Font &font, options opt, float textStrSize = -1.f, unsigned int maxChars = -1, float padX = 10.f, float padY = 5.f, const char *text = "");
                            FLAT_ENGINE_API textBox(fe::Vector2d size, const sf::Font &font, options opt, float textStrSize = -1.f, unsigned int maxChars = -1, float padX = 10.f, float padY = 5.f, const std::string &text = "");

                            FLAT_ENGINE_API void setOptions(options opt);

                            FLAT_ENGINE_API void handleEvent(const sf::Event &event);
                            FLAT_ENGINE_API void update();

                            FLAT_ENGINE_API void setString(const char *str);
                            FLAT_ENGINE_API void setString(const std::string &str);
                            FLAT_ENGINE_API const std::string &getString() const;
                            FLAT_ENGINE_API const std::string &getVisibleString() const;

                            FLAT_ENGINE_API virtual void setSize(const fe::Vector2d &size);

                            FLAT_ENGINE_API bool active();

                    };
            }
    }