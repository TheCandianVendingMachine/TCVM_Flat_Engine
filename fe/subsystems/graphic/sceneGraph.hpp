// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "spriteBatch.hpp"
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
                public:
                    typedef unsigned int EntityHandle;

                private:
                    std::vector<fe::baseEntity*> m_entities;
                    std::vector<EntityHandle> m_handles; // a front end so handles will always point to the right entity

                    spriteBatch m_batch;

                public:
                    FLAT_ENGINE_API void update(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API fe::sceneGraph::EntityHandle addEntity(fe::baseEntity *ent);
                    FLAT_ENGINE_API void removeEntity(fe::sceneGraph::EntityHandle handle);
                    FLAT_ENGINE_API fe::baseEntity *getEntity(fe::sceneGraph::EntityHandle handle);

                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);

            };
    }