// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../../flatEngineExport.hpp"
#include "../../../typeDefines.hpp"
#include "../../memory/poolAllocater.hpp"
#include "spriteBatch.hpp"
#include "renderObject.hpp"

namespace fe
    {
        class RenderTarget;
    }

namespace fe
    {
        class sceneGraph
            {
                private:
                    fe::poolAllocater<renderObject> m_renderObjects;
                    fe::spriteBatch m_batch;

                public:
                    void startUp();
                    void shutDown();
                    void clear();

                    void draw(sf::RenderTarget &window);

            };
    }