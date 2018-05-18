// dialog.hpp
// a dialog is a GUI container. A dialog can have something attached to it, and is the very base GUI class
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/messaging/eventHandler.hpp"
#include "fe/subsystems/physics/transformable.hpp"
#include "fe/math/polygon2.hpp"
#include "dialogStates.hpp"
#include <vector>

namespace sf
    {
        struct Event;
        class RenderTarget;
    }

namespace fe 
    {
        namespace gui
            {
                class guiBatch;
                class dialog : public fe::eventHandler, public fe::transformable
                    {
                        private:
                            std::vector<dialog*> m_containedDialogs;
                            fe::polygon2d m_dialogPolygon;

                            dialogStates m_dialogState;
                            int m_drawOrder;
                            bool m_killed;
                            bool m_polygonNeedsCreation;

                        protected:
                            virtual void drawDialogElements(fe::gui::guiBatch &target) {}
                            virtual void drawDialogText(sf::RenderTarget &target) {}

                            virtual void handleWindowEvent(const sf::Event &event) {}

                            virtual void onStateChange(dialogStates previous, dialogStates next) {}

                            FLAT_ENGINE_API void addPoint(fe::lightVector2d point);
                            FLAT_ENGINE_API void addPoint(float x, float y);

                        public:
                            FLAT_ENGINE_API virtual void init() {}
                            FLAT_ENGINE_API virtual void deinit() {}

                            FLAT_ENGINE_API void attach(dialog *element);
                            FLAT_ENGINE_API void detach(dialog *element);
                            FLAT_ENGINE_API bool isElementAttached(const dialog *element) const;
                            FLAT_ENGINE_API bool isAttachedTo(const dialog *element) const;

                            // Marks GUI dialog for cleanup
                            FLAT_ENGINE_API void kill();
                            FLAT_ENGINE_API bool isKilled() const;

                            // Set the draw order of the GUI element (Higher = Closer to the top)
                            FLAT_ENGINE_API void setDrawOrder(int drawOrder);
                            FLAT_ENGINE_API int getDrawOrder() const;

                            FLAT_ENGINE_API void setState(dialogStates state);
                            FLAT_ENGINE_API dialogStates getState() const;

                            virtual void handleEvent(const gameEvent &event) {}
                            FLAT_ENGINE_API void handleEvent(const sf::Event &event);

                            FLAT_ENGINE_API void draw(sf::RenderTarget &target, fe::gui::guiBatch &guiBatch);

                            FLAT_ENGINE_API fe::gui::dialog &operator=(dialog &rhs);

                    };
            }
    }
