// checkBox.hpp
// A check box. Attaches to checkList to emulate check boxes
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
                class checkBox : public fe::gui::control
                    {
                        private:
                            fe::polygon2d m_checkMark;
                            fe::polygon2d m_outline;

                            sf::Color m_colourOnActive;
                            sf::Color m_colourOnDeactive;
                            sf::Color m_colourOnHighlight;
                            sf::Color m_colourOnPress;

                            float m_checkRadius;
                            float m_distanceFromSide;
                            float m_boxOutlineWidth;

                            enum checkState
                                {
                                    NONE =      0,
                                    SELECTED =  1 << 0,
                                    HIGHLIGHT = 1 << 1
                                } m_checkState;

                        protected:
                            FLAT_ENGINE_API void onStateChange(dialogStates previous, dialogStates next) final;
                            FLAT_ENGINE_API void drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix) final;

                        public:
                            FLAT_ENGINE_API checkBox(float radius, float boxOutlineWidth = 10.f, float checkMarkWidthFromSide = 5.f);
                            FLAT_ENGINE_API void createCheckBox(float radius, float boxOutlineWidth, float checkMarkWidthFromSide);

                            FLAT_ENGINE_API void setColourOnActive(sf::Color colour);
                            FLAT_ENGINE_API void setColourOnDeactive(sf::Color colour);
                            FLAT_ENGINE_API void setColourHighlight(sf::Color colour);
                            FLAT_ENGINE_API void setColourOnPress(sf::Color colour);

                            FLAT_ENGINE_API void setSelected(bool value);
                            FLAT_ENGINE_API bool isSelected() const;

                    };
            }
    }
