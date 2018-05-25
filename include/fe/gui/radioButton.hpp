// radioButton.hpp
// a radio button. Attaches to radioLists
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "listButton.hpp"
#include "fe/typeDefines.hpp"

namespace fe
    {
        namespace gui
            {
                class radioButton : public fe::gui::listButton
                    {
                        private:
                            unsigned int m_points;
                            unsigned int m_id;
                            unsigned int m_idToSendTo;

                            FLAT_ENGINE_API fe::lightVector2d getCirclePoint(unsigned int index, float radius) const;
                            FLAT_ENGINE_API void onButtonStateChange(buttonState previous, buttonState next);

                        public:
                            FLAT_ENGINE_API radioButton(unsigned int id, unsigned int idToSendTo, unsigned int points, float radius, float outlineWidth = 10.f, float markWidthFromSide = 5.f);
                            FLAT_ENGINE_API void createButton(float radius, float outlineWidth, float markWidthFromSide);

                    };
            }
    }
