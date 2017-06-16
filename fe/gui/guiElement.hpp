// guiElement.hpp
// base class for all GUI elements
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../entity/transformable.hpp"
#include <SFML/Graphics/VertexArray.hpp>

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
                            sf::VertexArray m_shape;
                            sf::Color m_colour;

                            const panel *m_parentPanel; // the panel this gui element is attached to.
                            guiElement *m_parentElement;
                            fe::Vector2d m_size;

                            virtual void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix) = 0;

                        public:
                            FLAT_ENGINE_API guiElement();

                            FLAT_ENGINE_API fe::matrix3d getParentTransform();

                            FLAT_ENGINE_API void setParent(const panel *attached);
                            FLAT_ENGINE_API void setParent(guiElement *attached);

                            FLAT_ENGINE_API const panel *getParentPanel() const;
                            FLAT_ENGINE_API guiElement *getParentElement() const;

                            FLAT_ENGINE_API void setSize(const fe::Vector2d &size);
                            FLAT_ENGINE_API const fe::Vector2d &getSize();

                            FLAT_ENGINE_API void setColour(sf::Color colour);
                            FLAT_ENGINE_API sf::Color getColour() const;

                            virtual void handleEvent(const sf::Event &event) {}
                            virtual void update() {}
                            virtual void draw(sf::RenderTarget &target);
                            
                    };
            }
    }