// gameWorld.hpp
// The game world. Contains all relavent classes that are required for a world to function. Pathfinding graph, scene graph, etc
#pragma once
#include "../graphic/renderObject/sceneGraph.hpp"
#include "../../objectManagement/handleManager.hpp"

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class baseEntity;
        class collisionWorld;
        class broadphaseAbstract;

        class gameWorld : public fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>
            {
                private:
                    fe::sceneGraph m_sceneGraph;
                    fe::broadphaseAbstract *m_broadphase;

                    FLAT_ENGINE_API void onAdd(fe::baseEntity *object, fe::Handle objectHandle);

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void setBroadphase(fe::broadphaseAbstract *broadphase);
                    FLAT_ENGINE_API fe::broadphaseAbstract *getBroadphase() const;

                    FLAT_ENGINE_API fe::sceneGraph &getSceneGraph();

                    FLAT_ENGINE_API void preUpdate();
                    FLAT_ENGINE_API void update(collisionWorld *collisionWorld);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void preDraw();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);

            };
    }