// dialog.hpp
// a dialog is a GUI container. A dialog can have something attached to it, and is the very base GUI class
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/messaging/eventHandler.hpp"
#include "fe/subsystems/physics/transformable.hpp"
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

                            dialogStates m_dialogState;
                            int m_drawOrder;
                            bool m_killed;

                        protected:
                            virtual void drawDialogElements(fe::gui::guiBatch &target) = 0;
                            virtual void drawDialogText(sf::RenderTarget &target) = 0;

                            virtual void onStateChange(dialogStates previous, dialogStates next) = 0;

                        public:
                            virtual void init() = 0;
                            virtual void deinit() = 0;

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

                            virtual void handleEvent(const gameEvent &event) = 0;
                            virtual void handleEvent(const sf::Event &event) = 0;

                            FLAT_ENGINE_API void draw(sf::RenderTarget &target, fe::gui::guiBatch &guiBatch);

                            FLAT_ENGINE_API fe::gui::dialog &operator=(dialog &rhs);

                    };
            }
    }
