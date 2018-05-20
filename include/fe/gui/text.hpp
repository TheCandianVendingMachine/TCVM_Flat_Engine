// text.hpp
// A static text control. Does nothing but render text
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "control.hpp"
#include "fe/subsystems/resourceManager/fontData.hpp"
#include <SFML/Graphics/Text.hpp>
#include <string>

namespace fe
    {
        namespace gui
            {
                class text : public fe::gui::control
                    {
                        private:
                            const sf::Font *m_fontData;
                            sf::Text m_renderText;

                        protected:
                            FLAT_ENGINE_API void drawDialogText(sf::RenderTarget &target, const fe::matrix3d &drawMatrix);

                            // Updates text's origin to be in the center of the control
                            FLAT_ENGINE_API void updateOrigin();

                        public:
                            FLAT_ENGINE_API text(const sf::Font *data);

                            FLAT_ENGINE_API void setText(const char *text);
                            FLAT_ENGINE_API void setText(const std::string &text);

                            FLAT_ENGINE_API void setCharacterSize(unsigned int size);

                    };
            }
    }
