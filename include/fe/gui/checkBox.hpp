// checkBox.hpp
// A check box. Attaches to checkList to emulate check boxes
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "button.hpp"
#include "fe/typeDefines.hpp"
#include "fe/math/polygon2.hpp"

namespace fe
    {
        namespace gui
            {
                class checkBox : public fe::gui::button
                    {
                        private:
                            fe::polygon2d m_checkMark;

                        protected:
                            FLAT_ENGINE_API void onStateChange(dialogStates previous, dialogStates next) final;
                            FLAT_ENGINE_API void drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix) final;

                        public:
                            FLAT_ENGINE_API checkBox(float radius, float checkMarkWidthFromSide = 5.f);

                    };
            }
    }
