// gameWorld.hpp
// The game world. Contains all relavent classes that are required for a world to function. Pathfinding graph, scene graph, etc
#pragma once
#include "../graphic/renderObject/sceneGraph.hpp"
#include "../graphic/tileMap.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "../ai/graph.hpp"

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class baseEntity;
        class collisionWorld;
        class broadphaseAbstract;

        class gameWorld : protected fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>
            {
                private:
                    fe::sceneGraph m_sceneGraph;
                    fe::tileMap m_tileMap;
                    fe::graph m_aiGraph;
                    fe::broadphaseAbstract *m_dynamicBroadphase;
                    fe::broadphaseAbstract *m_staticBroadphase;

                    sf::RenderStates m_staticRenderStates;

                    FLAT_ENGINE_API void onAdd(fe::baseEntity *object, fe::Handle objectHandle);
                    FLAT_ENGINE_API void onRemove(fe::baseEntity *object, fe::Handle objectHandle);

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void setDynamicBroadphase(fe::broadphaseAbstract *broadphase);
                    FLAT_ENGINE_API void setStaticBroadphase(fe::broadphaseAbstract *broadphase);
                    FLAT_ENGINE_API fe::broadphaseAbstract *getDynamicBroadphase() const;
                    FLAT_ENGINE_API fe::broadphaseAbstract *getStaticBroadphase() const;

                    FLAT_ENGINE_API const fe::sceneGraph &getSceneGraph() const;
                    FLAT_ENGINE_API fe::sceneGraph &getSceneGraph();

                    FLAT_ENGINE_API const fe::tileMap &getTileMap() const;
                    FLAT_ENGINE_API fe::tileMap &getTileMap();

                    FLAT_ENGINE_API const fe::graph &getAIGraph() const;
                    FLAT_ENGINE_API fe::graph &getAIGraph();

                    FLAT_ENGINE_API void preUpdate();
                    FLAT_ENGINE_API void update(collisionWorld *collisionWorld);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void preDraw();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);
                    
                    template<typename ...Args>
                    FLAT_ENGINE_API fe::Handle addGameObject(fe::baseEntity *entity, Args &&...args);
                    FLAT_ENGINE_API void removeGameObject(Handle handle);

                    FLAT_ENGINE_API fe::baseEntity *getObject(Handle handle) const;

            };

        template<typename ...Args>
        fe::Handle gameWorld::addGameObject(fe::baseEntity *entity, Args &&...args)
            {
                fe::Handle objHandle = addObject(entity);
                fe::baseEntity *object = getObject(objHandle);

                object->initialize(*this, std::forward<Args>(args)...);
                if (object->m_collisionBody)
                    {
                        if (object->m_collisionBody->m_static && m_staticBroadphase)
                            {
                                m_staticBroadphase->add(object->m_collisionBody);
                            }
                        else
                            {
                                m_dynamicBroadphase->add(object->m_collisionBody);
                            }
                    }

                return objHandle;
            }
}