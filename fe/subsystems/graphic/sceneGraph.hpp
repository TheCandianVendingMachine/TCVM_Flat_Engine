// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <vector>

namespace sf
    {
        class RenderWindow;
    }

namespace fe
    {
        class baseEntity;

        class sceneGraph
            {
                private:
                    std::vector<fe::baseEntity*> m_entities;

                public:
                    FLAT_ENGINE_API void update(float deltaTime);

                    FLAT_ENGINE_API void addEntity(fe::baseEntity *ent);
                    FLAT_ENGINE_API void removeEntity(fe::baseEntity *ent);

                    FLAT_ENGINE_API void draw(sf::RenderWindow &app);

            };
    }