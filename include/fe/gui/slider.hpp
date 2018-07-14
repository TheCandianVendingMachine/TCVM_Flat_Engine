// slider.hpp
// A GUI control which allows for the user to slide the control and get the percentage slid
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "control.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/math/polygon2.hpp"
#include "sliderOrientation.hpp"

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
                            fe::Vector2d m_sliderDistanceFromSide;

                            fe::Vector2d m_minSliderPos;
                            fe::Vector2d m_maxSliderPos;

                            fe::Vector2d m_sliderGrabPoint;

                            fe::gui::sliderOrientation m_orientation;

							sf::Color m_colourOnActive;
							sf::Color m_colourOnDeactive;
							sf::Color m_colourOnHighlight;
							sf::Color m_colourOnPress;

							sf::Color m_sliderColour;

                            FLAT_ENGINE_API void drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix) final;
                            FLAT_ENGINE_API void onStateChange(dialogStates previous, dialogStates next) final;
                            FLAT_ENGINE_API void handleWindowEvent(const sf::Event &event) final;

                        public:
							/*
								- orientation = Orientation of the slider
								- sliderControlSize = Width/Height of slider depending on orientation (It equals: Width for Vertical and Height for Horizontal)
								- scrollSize = What percentage of the slider it will fill
								- lengthOfOutline = Width/Height of slider depending on orientation (It equals: Width for Horizontal and Height for Vertical)
								- outlineWidth = Width of outline of slider
								- sliderControlWidthFromEdge = Pixel gap between outline and start of slider
							*/
                            FLAT_ENGINE_API slider(fe::gui::sliderOrientation orientation, float sliderControlSize, float scrollSize, float lengthOfOutline, float outlineWidth, float sliderControlWidthFromEdge = 0.f);
                            FLAT_ENGINE_API float getPercentScrolled() const;
							FLAT_ENGINE_API void setPercentScrolled(float percent);

							FLAT_ENGINE_API void setColourOnActive(sf::Color colour);
							FLAT_ENGINE_API void setColourOnDeactive(sf::Color colour);
							FLAT_ENGINE_API void setColourHighlight(sf::Color colour);
							FLAT_ENGINE_API void setColourOnPress(sf::Color colour);

                    };
            }
    }
