// guiElement.hpp
// base class for all GUI elements
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../subsystems/physics/transformable.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include "../typeDefines.hpp"

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
                            sf::Color m_activeColour;
                            sf::Color m_inactiveColour;

                            fe::guid m_event;
                            fe::guid m_extraEvent;
                            fe::guid m_id;

                            const panel *m_parentPanel; // the panel this gui element is attached to.
                            guiElement *m_parentElement;
                            fe::Vector2d m_size;

                            sf::Texture *m_texture;

                            bool m_active;
                            bool m_colourUpdate;

                            virtual void drawElement(sf::RenderTarget &target, const fe::matrix3d &matrix) {}

                        public:
                            FLAT_ENGINE_API guiElement();

                            FLAT_ENGINE_API virtual void fitToParent();
                            FLAT_ENGINE_API fe::matrix3d getParentTransform();

                            // set the position relative to the parent element
                            FLAT_ENGINE_API virtual void setPositionRelative(fe::Vector2d position);
                            FLAT_ENGINE_API virtual void setPositionRelative(float x, float y);

                            FLAT_ENGINE_API void setEvent(const char *event);
                            FLAT_ENGINE_API void setEvent(fe::guid event);
                            FLAT_ENGINE_API fe::guid getEvent() const;

                            FLAT_ENGINE_API void setExtraEvent(const char *event);
                            FLAT_ENGINE_API void setExtraEvent(fe::guid event);
                            FLAT_ENGINE_API fe::guid getExtraEvent() const;

                            FLAT_ENGINE_API void setParent(const panel *attached);
                            FLAT_ENGINE_API void setParent(guiElement *attached);

                            FLAT_ENGINE_API const panel *getParentPanel() const;
                            FLAT_ENGINE_API guiElement *getParentElement() const;

                            FLAT_ENGINE_API virtual void setSize(const fe::Vector2d &size);
                            FLAT_ENGINE_API const fe::Vector2d &getSize() const;

                            FLAT_ENGINE_API void setActive(bool value);
                            FLAT_ENGINE_API bool getActive() const;

                            FLAT_ENGINE_API void setActiveColour(sf::Color colour);
                            FLAT_ENGINE_API void setInactiveColour(sf::Color colour);

                            FLAT_ENGINE_API sf::Color getActiveColour() const;
                            FLAT_ENGINE_API sf::Color getInactiveColour() const;

                            FLAT_ENGINE_API virtual void setTexture(sf::Texture *texture, fe::Vector2<unsigned int> texCoords = fe::Vector2<unsigned int>(0, 0), fe::Vector2<unsigned int> size = fe::Vector2<unsigned int>(0, 0));

                            virtual void handleEvent(const sf::Event &event) {}
                            virtual void update() {}
                            FLAT_ENGINE_API virtual void draw(sf::RenderTarget &target);

                            FLAT_ENGINE_API void setID(fe::guid id);
                            FLAT_ENGINE_API fe::guid id();
                            
                    };
            }
    }