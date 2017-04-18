// button.hpp
// Can be clicked, and when clicked a callback is called
#pragma once
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
                            sf::VertexArray m_shape;

                            void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix);

                        public:
                            button(const fe::Vector2d &size, const std::function<void()> &callback);

                            void handleEvent(const sf::Event &event);
                            void update();

                    };
            }
    }