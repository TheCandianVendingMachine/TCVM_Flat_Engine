// button.hpp
// A basic button. When pressed an event is sent
#pragma once
#include "fe/flatEngineExport.hpp"
#include "control.hpp"
#include "fe/typeDefines.hpp"

namespace fe
    {
        namespace gui
            {
                class button : public fe::gui::control
                    {
                        private:
                            fe::str m_eventOnPress;
                            fe::str m_eventOnRelease;

                            sf::Color m_colourOnActive;
                            sf::Color m_colourOnDeactive;
                            sf::Color m_colourOnHighlight;
                            sf::Color m_colourOnPress;

                        protected:
                            FLAT_ENGINE_API virtual void onStateChange(dialogStates previous, dialogStates next) override;
                            FLAT_ENGINE_API virtual void drawDialogElements(sf::RenderTarget &target, const fe::transformable &drawMatrix) override;

                        public:
                            FLAT_ENGINE_API button(std::initializer_list<fe::lightVector2d> shape);

                            FLAT_ENGINE_API void setEventOnPress(fe::str event);
                            FLAT_ENGINE_API void setEventOnRelease(fe::str event);

                            FLAT_ENGINE_API void setColourOnActive(sf::Color colour);
                            FLAT_ENGINE_API void setColourOnDeactive(sf::Color colour);
                            FLAT_ENGINE_API void setColourHighlight(sf::Color colour);
                            FLAT_ENGINE_API void setColourOnPress(sf::Color colour);

                    };
            }
    }
