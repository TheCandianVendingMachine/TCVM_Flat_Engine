// toggleButton.hpp
// a button that toggles. Callback is called on press
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "button.hpp"

namespace fe
    {
        namespace gui
            {
                class toggleButton : public fe::gui::button
                    {
                        private:
                            bool m_toggle;

                        public:
                            FLAT_ENGINE_API toggleButton(const fe::Vector2d &size, const std::function<void()> &callback);

                            FLAT_ENGINE_API void update();
                            FLAT_ENGINE_API void setToggle(bool value);

                    };
            }
    }