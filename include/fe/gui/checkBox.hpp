// checkBox.hpp
// A check box. Attaches to checkList to emulate check boxes
#pragma once
#include "fe/flatEngineExport.hpp"
#include "listButton.hpp"

namespace fe
    {
        namespace gui
            {
                class checkBox : public fe::gui::listButton
                    {
                        public:
                            FLAT_ENGINE_API checkBox(float radius, float boxOutlineWidth = 10.f, float checkMarkWidthFromSide = 5.f);
                            FLAT_ENGINE_API void createButton(float radius, float boxOutlineWidth, float checkMarkWidthFromSide) final;


                    };
            }
    }
