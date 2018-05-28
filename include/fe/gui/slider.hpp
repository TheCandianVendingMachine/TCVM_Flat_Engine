// slider.hpp
// A GUI control which allows for the user to slide the control and get the percentage slid
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "control.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/math/polygon2.hpp"

namespace fe
    {
        namespace gui
            {
                class slider : public fe::gui::control
                    {
                        private:
                            fe::polygon2d m_sliderContainer;
                            fe::Vector2d m_containerSize;
                            fe::Vector2d m_containerOutlineWidth;

                            fe::Vector2d m_sliderSize;
                            fe::Vector2d m_sliderPosition;

                            float m_percentageScrolled; // percentage of the scroll from 0 -> 1

                            FLAT_ENGINE_API void drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix) final;
                            FLAT_ENGINE_API void onStateChange(dialogStates previous, dialogStates next) final;

                        public:
                            enum class sliderOrientation
                                {
                                    HORIZONTAL,
                                    VERTICAL
                                };

                            FLAT_ENGINE_API slider(sliderOrientation orientation, float sliderControlSize, float scrollSize, float lengthOfOutline, float outlineWidth, float sliderControlWidthFromEdge = 0.f);

                    };
            }
    }
