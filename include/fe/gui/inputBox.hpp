// inputBox.hpp
// A box that when selected the user can type into
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/gui/control.hpp"
#include "fe/gui/text.hpp"
#include "fe/math/Vector2.hpp"
#include "inputBoxModes.hpp"
#include <string>

namespace sf
    {
        class Font;
        class RenderTarget;
    }

namespace fe
    {
        class transformable;

        namespace gui
            {
                class inputBox : public fe::gui::control
                    {
                        private:
                            fe::polygon2d m_textContainer;
                            fe::Vector2d m_containerOutlineWidth;

                            fe::gui::text m_text;
                            const sf::Font *const m_font;

                            fe::Vector2d m_bounds;
                            fe::Vector2d m_textBounds;

                            // entire text buffer
                            std::string m_inputText;
                            // text that is set. Used when SCROLL_WHEN_FULL flag is set
                            std::string m_inputTextDisplayed;

                            float m_textDistanceFromEdge;

                            fe::gui::inputBoxModes m_modes;

                            bool m_allowAlpha;
                            bool m_allowNumeric;
                            bool m_allowDecimal;
                            bool m_allowNegative;
                            bool m_scrollWhenFull;

                            bool m_active;

                            // Forces a bound update by iterating through each visible text character and adding it to the bound size
                            FLAT_ENGINE_API void forceBoundUpdate();
                            FLAT_ENGINE_API void updateText();
                            FLAT_ENGINE_API void fitText();
                            FLAT_ENGINE_API fe::Vector2d getCharacterSize(char c);

                            FLAT_ENGINE_API void handleOperator(char c);
                            FLAT_ENGINE_API void handleCharacter(char c);
                            FLAT_ENGINE_API void handleASCII(char c);
                            FLAT_ENGINE_API void handleWindowEvent(const sf::Event &event) override;
                            FLAT_ENGINE_API void onStateChange(dialogStates previous, dialogStates next) override;
                            FLAT_ENGINE_API void drawDialogElements(sf::RenderTarget &target, const fe::transformable &drawMatrix) override;

                        public:
                            FLAT_ENGINE_API inputBox(const sf::Font *const font, fe::gui::inputBoxModes mode = fe::gui::inputBoxModes::NONE);
                            FLAT_ENGINE_API void setSize(float x, float y);
                            FLAT_ENGINE_API void setSize(fe::Vector2d size);

                            FLAT_ENGINE_API void setOutlineSize(float size);
                            FLAT_ENGINE_API void setOutlineSize(fe::Vector2d size);
                            FLAT_ENGINE_API void setOutlineSize(float x, float y);

                            FLAT_ENGINE_API void init(fe::gui::guiGraph &graph, int node) override;

                            // Returns the entire string in the input box
                            FLAT_ENGINE_API std::string getInput() const;
                            // Returns the input that is currently displayed in the box. Any extra text is ignored
                            FLAT_ENGINE_API std::string getDisplayedInput() const;

                            FLAT_ENGINE_API void setInput(const std::string &input);
                            FLAT_ENGINE_API void setCharacterSize(unsigned int size);

                    };
        }
    }
