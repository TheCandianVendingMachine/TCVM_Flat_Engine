// dialog.hpp
// a dialog is a GUI container. A dialog can have something attached to it, and is the very base GUI class
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/messaging/eventHandler.hpp"
#include "fe/subsystems/physics/transformable.hpp"
#include "dialogStates.hpp"
#include "alignment.hpp"
#include <vector>

namespace sf
    {
        class Event;
        class RenderTarget;
    }

namespace fe 
    {
        struct matrix3d;
        namespace gui
            {
                class guiGraph;
                class dialog : public fe::eventHandler, public fe::transformable
                    {
                        private:
                            fe::transformable m_drawMatrix;

                            dialogStates m_dialogState;
                            int m_graphNode;
                            bool m_killed;
                            bool m_draw;

                        protected:
                            virtual void drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix) = 0;
                            virtual void drawDialogText(sf::RenderTarget &target, const fe::transformable &drawMatrix) = 0;

                            virtual void onStateChange(dialogStates previous, dialogStates next) = 0;

                        public:
                            FLAT_ENGINE_API dialog();

                            virtual void init(fe::gui::guiGraph &graph, int node) = 0;
                            virtual void deinit(fe::gui::guiGraph &graph, int node) = 0;

                            // Marks GUI dialog for cleanup
                            FLAT_ENGINE_API void kill();
                            FLAT_ENGINE_API bool isKilled() const;

                            FLAT_ENGINE_API void setDraw(bool draw);
                            FLAT_ENGINE_API bool isDrawn() const;

                            FLAT_ENGINE_API void setState(dialogStates state);
                            FLAT_ENGINE_API dialogStates getState() const;

                            virtual void handleEvent(const gameEvent &event) = 0;
                            virtual void handleEvent(const sf::Event &event) = 0;

                            FLAT_ENGINE_API void draw(sf::RenderTarget &target, const fe::transformable &matrix);

                            FLAT_ENGINE_API fe::transformable &getDrawMatrix();

                            FLAT_ENGINE_API fe::gui::dialog &operator=(dialog &rhs);

                            FLAT_ENGINE_API void setPosition(float x, float y, fe::gui::align alignment = align::MIDDLE);
                            FLAT_ENGINE_API void setPosition(fe::Vector2d pos, fe::gui::align alignment = align::MIDDLE);
                            // Set the window position in X/Y% across the attached dialog
                            FLAT_ENGINE_API void setWindowPosition(float x, float y, fe::gui::align alignment = align::MIDDLE);
                            FLAT_ENGINE_API void setWindowPosition(fe::Vector2d pos, fe::gui::align alignment = align::MIDDLE);

                            FLAT_ENGINE_API virtual fe::lightVector2d getSize() const;

                            FLAT_ENGINE_API void setGraphNode(int node);
                            FLAT_ENGINE_API int getGraphNode();

                    };
            }
    }
