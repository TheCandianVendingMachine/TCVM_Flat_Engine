// control.hpp
// Base class for all GUI controls. A control is a GUI element which the user can interact with (i.e, not windows)
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/math/polygon2.hpp"
#include "dialog.hpp"
#include "dialogStates.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <vector>

namespace sf
    {
        class Event;
        class RenderTarget;
    }

namespace fe
    {
        namespace gui
            {
                class control : public fe::gui::dialog
                    {
                        private:
                            std::vector<sf::Vertex> m_verticies;
                            fe::polygon2d m_controlPolygon;
                            sf::Color m_drawColour;

                            fe::lightVector2d m_controlOffset;

                            int m_vertexCount;
                            int m_activeVertexCount;

                        protected:
                            virtual void drawDialogElements(sf::RenderTarget &target, const fe::transformable &drawMatrix) {}
                            virtual void drawDialogText(sf::RenderTarget &target, const fe::transformable &drawMatrix) {}
                            virtual void onStateChange(dialogStates previous, dialogStates next) {}
                            virtual void handleWindowEvent(const sf::Event &event) {}

                            FLAT_ENGINE_API void drawToScreen(sf::RenderTarget &target, const fe::transformable &drawMatrix);
                            FLAT_ENGINE_API void drawPolygon(fe::polygon2d &poly, const fe::matrix3d &drawMatrix, sf::Color drawColour);

                            FLAT_ENGINE_API fe::polygon2d &getControlPolygon();

                            FLAT_ENGINE_API void addPoint(fe::lightVector2d point);
                            FLAT_ENGINE_API void addPoint(float x, float y);

                            FLAT_ENGINE_API void setControlOffset(fe::lightVector2d offset);

                        public:
                            FLAT_ENGINE_API control();

                            virtual void init(fe::gui::guiGraph &graph, int node) {}
                            virtual void deinit(fe::gui::guiGraph &graph, int node) {}

                            FLAT_ENGINE_API void setDrawColour(sf::Color colour);
                            FLAT_ENGINE_API sf::Color getDrawColour() const;

                            FLAT_ENGINE_API void handleEvent(const sf::Event &event);
                            virtual void handleEvent(const gameEvent &event) override {}

                            FLAT_ENGINE_API virtual fe::lightVector2d getSize() const;

                    };
            }
    }
