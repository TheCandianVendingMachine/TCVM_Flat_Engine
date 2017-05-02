// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "spriteBatch.hpp"
#include <vector>

namespace sf
    {
        class RenderWindow;
    }

namespace fe
    {
        class baseEntity;

        class sceneGraph : public handleManager<fe::baseEntity*>
            {
                private:
                    spriteBatch m_batch;

                public:
                    FLAT_ENGINE_API void update(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);

                    FLAT_ENGINE_API ~sceneGraph();

            };
    }