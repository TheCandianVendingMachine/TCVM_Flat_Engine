// listButton.hpp
// A generic list button. Can set and toggle state
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "control.hpp"
#include "fe/typeDefines.hpp"
#include "fe/math/polygon2.hpp"

namespace fe
    {
        namespace gui
            {
                class listButton : public fe::gui::control
                    {
                        protected:
                            enum buttonState
                                {
                                    NONE =      0,
                                    SELECTED =  1 << 0,
                                    HIGHLIGHT = 1 << 1
                                };

                        private:
                            fe::polygon2d m_mark;
                            fe::polygon2d m_outline;

                            sf::Color m_colourOnActive;
                            sf::Color m_colourOnDeactive;
                            sf::Color m_colourOnHighlight;
                            sf::Color m_colourOnPress;

                            float m_checkRadius;
                            float m_distanceFromSide;
                            float m_outlineWidth;

                            buttonState m_buttonState;

                        protected:
                            FLAT_ENGINE_API void onStateChange(dialogStates previous, dialogStates next) final;
                            FLAT_ENGINE_API void drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix) final;

                            FLAT_ENGINE_API void addMarkPoint(float x, float y);
                            FLAT_ENGINE_API void addOutlinePoint(float x, float y);

                            FLAT_ENGINE_API void clearMark();
                            FLAT_ENGINE_API void clearOutline();

                            FLAT_ENGINE_API void createMark();
                            FLAT_ENGINE_API void createOutline();

                            virtual void onButtonStateChange(buttonState previous, buttonState next) {};

                        public:
                            FLAT_ENGINE_API listButton(float radius, float outlineWidth, float widthFromSide);
                            virtual void createButton(float radius, float outlineWidth, float widthFromSide) = 0;

                            FLAT_ENGINE_API void setCheckRadius(float radius);
                            FLAT_ENGINE_API void setDistanceFromSide(float distance);
                            FLAT_ENGINE_API void setOutlineWidth(float width);

                            FLAT_ENGINE_API float getCheckRadius() const;
                            FLAT_ENGINE_API float getDistanceFromSide() const;
                            FLAT_ENGINE_API float getOutlineWidth() const;

                            FLAT_ENGINE_API void setColourOnActive(sf::Color colour);
                            FLAT_ENGINE_API void setColourOnDeactive(sf::Color colour);
                            FLAT_ENGINE_API void setColourHighlight(sf::Color colour);
                            FLAT_ENGINE_API void setColourOnPress(sf::Color colour);

                            FLAT_ENGINE_API void setSelected(bool value);
                            FLAT_ENGINE_API bool isSelected() const;

                            using fe::gui::control::handleEvent;
                            virtual void handleEvent(const gameEvent &event) override {}
                    };
            }
    }
