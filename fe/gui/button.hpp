// <NAME>button.hpp</NAME>
// <DESCRIPTION>Can be clicked, and when clicked a callback is called</DESCRIPTION>
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"

#include "guiElement.hpp"
#include <functional>
#include <SFML/Graphics/VertexArray.hpp>

namespace fe
    {
        namespace gui
            {
                class panel;
                class button : public guiElement
                    {
                        private:
                            std::function<void()> m_callback;
                            bool m_pressed;

                            FLAT_ENGINE_API void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix);

                        public:
                            FLAT_ENGINE_API button(const fe::Vector2d &size, const std::function<void()> &callback);

                            FLAT_ENGINE_API virtual void handleEvent(const sf::Event &event);
                            FLAT_ENGINE_API virtual void update();

                    };
            }
    }