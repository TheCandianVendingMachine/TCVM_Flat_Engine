// guiGraph.hpp
// A scene graph for GUI elements
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/graphic/renderGraph.hpp"
#include <vector>

namespace fe
    {
        namespace gui
            {
                class dialog;
                class guiGraph : public fe::renderGraph
                    {
                        private:
                            std::vector<fe::gui::dialog*> m_dialogs;

                            FLAT_ENGINE_API int getNodeFromObject(void *object) final;
                            FLAT_ENGINE_API fe::transformable *getNodeTransform(fe::priv::node *node) final;
                            FLAT_ENGINE_API fe::transformable *getNodeTempTransform(fe::priv::node *node) final;
                            FLAT_ENGINE_API const sf::Texture *getRenderTexture() const final;
                            FLAT_ENGINE_API bool renderNode(fe::priv::node *node) const final;
                            FLAT_ENGINE_API void drawNode(fe::priv::node *node, sf::RenderTarget &target) final;

                            FLAT_ENGINE_API void *createZOrderProxy() final;

                            FLAT_ENGINE_API void drawToScreen(sf::RenderTarget &target, sf::RenderStates states) final;

                            FLAT_ENGINE_API void onObjectAdd(void *object, int node) final;
                            FLAT_ENGINE_API void onObjectRemove(void *object, int node) final;

                        public:
                            FLAT_ENGINE_API guiGraph();

                            FLAT_ENGINE_API void preDraw() final;

                    };
            }
    }
