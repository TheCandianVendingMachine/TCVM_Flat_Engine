// gameWorld.hpp
// The game world. Contains all relavent classes that are required for a world to function. Pathfinding graph, scene graph, etc
#pragma once
#include "fe/flatEngineExport.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include "fe/math/Vector2.hpp"
#include "entityWorld.hpp"
#include "fe/subsystems/graphic/renderObject/sceneGraph.hpp"
#include "fe/subsystems/graphic/tileMap.hpp"
#include "fe/subsystems/ai/graph.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/subsystems/resourceManager/fontData.hpp"
#include "fe/entity/entityModules.hpp"
#include "fe/entity/component/componentManager.hpp"
#include <functional>
#include <vector>

namespace sf
    {
        class RenderTarget;
    }

namespace fe
    {
        class collisionWorld;
        class broadphaseAbstract;
        class serializerID;
        class baseGameState;
        class userEntityObject;
        class baseEntity;

        class gameWorld
            {
                private:
                    fe::sceneGraph m_sceneGraph;
                    fe::tileMap m_tileMap;
                    fe::graph m_aiGraph;
                    fe::entityWorld m_entityWorld;
                    fe::componentManager m_componentManager;
                    fe::broadphaseAbstract *m_dynamicBroadphase;
                    fe::broadphaseAbstract *m_staticBroadphase;
                    fe::serializerID *m_serializer;
                    fe::baseGameState *m_gameState;

                    sf::RenderStates m_staticRenderStates;

                public:
                    FLAT_ENGINE_API gameWorld(baseGameState *gameState);

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API fe::baseGameState &getGameState() const;
                    FLAT_ENGINE_API fe::entityWorld &getEntityWorld();

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

                    FLAT_ENGINE_API const fe::componentManager &getComponentManager() const;
                    FLAT_ENGINE_API fe::componentManager &getComponentManager();

                    FLAT_ENGINE_API void preUpdate();
                    FLAT_ENGINE_API void update(collisionWorld *collisionWorld);
                    FLAT_ENGINE_API void fixedUpdate(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void preDraw();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &app);

                    FLAT_ENGINE_API void save(const std::string &save);
                    FLAT_ENGINE_API void save(std::ofstream &out);
                    FLAT_ENGINE_API void save();
                    FLAT_ENGINE_API void load(const std::string &load);
                    FLAT_ENGINE_API void load(std::ifstream &in);
                    FLAT_ENGINE_API void load();

                    FLAT_ENGINE_API void loadTilePrefabs(const char *filepath);
                    
                    FLAT_ENGINE_API fe::Handle addGameObject(fe::entityModules modules, fe::userEntityObject *scriptObject, int connected = -1, const fe::fontData &data = fe::fontData());
                    FLAT_ENGINE_API void removeGameObject(Handle handle);

                    FLAT_ENGINE_API fe::baseEntity *getObject(Handle handle) const;
                    
                    FLAT_ENGINE_API void getEntitiesWithinArea(std::vector<fe::baseEntity*> &entities, float xPos, float yPos, float xSize, float ySize);
                    FLAT_ENGINE_API void getEntitiesWithinArea(std::vector<fe::baseEntity*> &entities, fe::Vector2d pos, fe::Vector2d size);
                    FLAT_ENGINE_API void getEntitiesWithinArea(std::vector<fe::baseEntity*> &entities, float xPos, float yPos, float xSize, float ySize, fe::str tag);
                    FLAT_ENGINE_API void getEntitiesWithinArea(std::vector<fe::baseEntity*> &entities, fe::Vector2d pos, fe::Vector2d size, fe::str tag);

                    FLAT_ENGINE_API void getEntitiesWithinRange(std::vector<fe::baseEntity*> &entities, float xPos, float yPos, float range);
                    FLAT_ENGINE_API void getEntitiesWithinRange(std::vector<fe::baseEntity*> &entities, fe::Vector2d pos, float range);
                    FLAT_ENGINE_API void getEntitiesWithinRange(std::vector<fe::baseEntity*> &entities, float xPos, float yPos, float range, fe::str tag);
                    FLAT_ENGINE_API void getEntitiesWithinRange(std::vector<fe::baseEntity*> &entities, fe::Vector2d pos, float range, fe::str tag);

                    FLAT_ENGINE_API void getEntitiesByTag(std::vector<fe::baseEntity*> &entities, fe::str tag);

            };
    }
