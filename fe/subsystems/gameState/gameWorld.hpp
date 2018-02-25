// gameWorld.hpp
// The game world. Contains all relavent classes that are required for a world to function. Pathfinding graph, scene graph, etc
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include "entityWorld.hpp"
#include "../graphic/renderObject/sceneGraph.hpp"
#include "../graphic/tileMap.hpp"
#include "../ai/graph.hpp"
#include "../../entity/baseEntity.hpp"
#include "../resourceManager/fontData.hpp"

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class collisionWorld;
        class broadphaseAbstract;
        class serializerID;

        class gameWorld
            {
                private:
                    fe::sceneGraph m_sceneGraph;
                    fe::tileMap m_tileMap;
                    fe::graph m_aiGraph;
                    fe::entityWorld m_entityWorld;
                    fe::broadphaseAbstract *m_dynamicBroadphase;
                    fe::broadphaseAbstract *m_staticBroadphase;
                    fe::serializerID *m_serializer;

                    sf::RenderStates m_staticRenderStates;

                public:
                    FLAT_ENGINE_API gameWorld();

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

                    FLAT_ENGINE_API void save(std::ofstream &out);
                    FLAT_ENGINE_API void save();
                    FLAT_ENGINE_API void load(std::ifstream &in);
                    FLAT_ENGINE_API void load();

                    FLAT_ENGINE_API void loadTilePrefabs(const char *filepath);
                    
                    FLAT_ENGINE_API fe::Handle addGameObject(fe::baseEntity *entity, int connected = -1, const fe::fontData &data = fe::fontData());
                    FLAT_ENGINE_API void removeGameObject(Handle handle);

                    FLAT_ENGINE_API fe::baseEntity *getObject(Handle handle) const;

            };
    }
