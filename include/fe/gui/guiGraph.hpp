// guiGraph.hpp
// A scene graph for GUI elements
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/graphic/renderGraph.hpp"
#include "guiBatch.hpp"

namespace fe
    {
        namespace gui
            {
                class guiGraph : public fe::renderGraph
                    {
                        private:
                            fe::gui::guiBatch m_batch;

                            FLAT_ENGINE_API fe::transformable *getNodeTransform(fe::priv::node *node) final;
                            FLAT_ENGINE_API fe::transformable *getNodeTempTransform(fe::priv::node *node) final;
                            FLAT_ENGINE_API const sf::Texture *getRenderTexture() const final;
                            FLAT_ENGINE_API bool renderNode(fe::priv::node *node) const final;
                            FLAT_ENGINE_API void drawNode(fe::priv::node *node) final;

                            FLAT_ENGINE_API void drawToScreen(sf::RenderTarget &target, sf::RenderStates states) final;

                        public:
                    };
            }
    }
