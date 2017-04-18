// guiElement.hpp
// base class for all GUI elements
#pragma once

#include "../transformable.hpp"

namespace sf
    {
        class RenderTarget;
        class Drawable;
        class Event;
    }

namespace fe
    {
        namespace gui
            {
                class panel;

                class guiElement : public fe::transformable
                    {
                        protected:
                            const panel *m_parentPanel; // the panel this gui element is attached to.
                            guiElement *m_parentElement;
                            fe::Vector2d m_size;

                            virtual void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix) = 0;

                        public:
                            guiElement();

                            void setParent(const panel *attached);
                            void setParent(guiElement *attached);

                            void setSize(const fe::Vector2d &size);
                            const fe::Vector2d &getSize() const;

                            virtual void handleEvent(const sf::Event &event) = 0;
                            virtual void update() = 0;
                            virtual void draw(sf::RenderTarget &target);
                            
                    };
            }
    }