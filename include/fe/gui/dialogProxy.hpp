// dialogProxy.hpp
// FOR INTERNAL USE ONLY -- Is a dialog object without any method defined. Used in guiGraph
#pragma once
#include "dialog.hpp"

namespace fe
    {
        namespace gui
            {
                namespace imp
                    {
                        class dialogProxy : public fe::gui::dialog
                            {
                                private:
                                    void drawDialogElements(sf::RenderTarget &target, const fe::transformable &drawMatrix) {}
                                    void drawDialogText(sf::RenderTarget &target, const fe::transformable &drawMatrix) {}

                                    void onStateChange(dialogStates previous, dialogStates next) {}

                                public:
                                    void init(fe::gui::guiGraph &graph, int node) {}
                                    void deinit(fe::gui::guiGraph &graph, int node) {}

                                    void handleEvent(const gameEvent &event) {}
                                    void handleEvent(const sf::Event &event) {}
                            };
                    }
            }
    }
