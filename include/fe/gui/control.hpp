// control.hpp
// Base class for all GUI controls. A control is a GUI element which the user can interact with (i.e, not windows)
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "fe/math/polygon2.hpp"
#include "dialog.hpp"
#include "dialogStates.hpp"

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

                class control : public fe::gui::dialog
                    {
                        private:
                            fe::polygon2d m_controlPolygon;
                            bool m_polygonNeedsCreation;

                        protected:
                            virtual void drawDialogText(sf::RenderTarget &target) {}
                            virtual void onStateChange(dialogStates previous, dialogStates next) {}
                            virtual void handleWindowEvent(const sf::Event &event) {}

                            FLAT_ENGINE_API void drawDialogElements(fe::gui::guiBatch &target) final;

                            FLAT_ENGINE_API void addPoint(fe::lightVector2d point);
                            FLAT_ENGINE_API void addPoint(float x, float y);

                        public:
                            FLAT_ENGINE_API control();

                            virtual void init() {}
                            virtual void deinit() {}

                            FLAT_ENGINE_API void handleEvent(const sf::Event &event);
                            virtual void handleEvent(const gameEvent &event) {}

                    };
            }
    }
