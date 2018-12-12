#include "fe/subsystems/gameState/gameWorld.hpp"
#include "fe/subsystems/resourceManager/resourceManager.hpp"
#include "fe/subsystems/collision/collisionWorld.hpp"
#include "fe/subsystems/collision/broadphaseAbstract.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/collision/bounds/circle.hpp"
#include "fe/subsystems/collision/bounds/mixedTests.hpp"
#include "fe/subsystems/collision/bounds/aabbTests.hpp"
#include <fstream>

void addEntityToVector(fe::collisionBounds *bound, std::vector<fe::baseEntity*> *entities, fe::entityWorld &world, fe::collider *collider)
    {
        fe::AABB *aabb = dynamic_cast<fe::AABB*>(bound);
        fe::circle *circle = dynamic_cast<fe::circle*>(bound);

        bool intersects = false;
        if (aabb)
            {
                intersects = fe::intersects(*aabb, collider->m_aabb);
            }
        else
            {
                intersects = fe::intersects(*circle, collider->m_aabb);
            }

        if (intersects && world.isEntity(collider->m_owner))
            {
                entities->push_back(static_cast<fe::baseEntity*>(collider->m_owner));
            }
    }

void addTaggedEntityToVector(fe::collisionBounds *bound, std::vector<fe::baseEntity*> *entities, fe::entityWorld &world, fe::str tag, fe::collider *collider)
    {
        fe::AABB *aabb = dynamic_cast<fe::AABB*>(bound);
        fe::circle *circle = dynamic_cast<fe::circle*>(bound);

        bool intersects = false;
        if (aabb)
            {
                intersects = fe::intersects(*aabb, collider->m_aabb);
            }
        else
            {
                intersects = fe::intersects(*circle, collider->m_aabb);
            }

        if (intersects && world.isEntity(collider->m_owner) && static_cast<fe::baseEntity*>(collider->m_owner)->getTag() == tag)
            {
                entities->push_back(static_cast<fe::baseEntity*>(collider->m_owner));
            }
    }

fe::gameWorld::gameWorld(baseGameState *gameState) :
    m_entityWorld(*this),
    m_gameState(gameState),
    m_tileMap(*this)
    {
    }

void fe::gameWorld::startUp()
    {
        m_sceneGraph.startUp();
        m_staticRenderStates.texture = &fe::engine::get().getResourceManager<sf::Texture>()->get();
        m_entityWorld.startUp();
        m_dynamicBroadphase = nullptr;
        m_staticBroadphase = nullptr;
        m_serializer = new fe::serializerID();
        m_componentManager.startUp(m_gameState);

        fe::engine::get().getScriptManager().getFunctionHandler().registerCPPObjectFunction("getComponent", &m_componentManager, &fe::componentManager::getLuaComponent);
    }

void fe::gameWorld::shutDown()
    {
        m_componentManager.shutDown();
        m_sceneGraph.shutDown();
        delete m_serializer;
        m_serializer = nullptr;
    }

fe::baseGameState &fe::gameWorld::getGameState() const
    {
        return *m_gameState;
    }

fe::entityWorld &fe::gameWorld::getEntityWorld()
    {
        return m_entityWorld;
    }

void fe::gameWorld::setDynamicBroadphase(fe::broadphaseAbstract *broadphase)
    {
        m_dynamicBroadphase = broadphase;
        m_dynamicBroadphase->startUp();
        m_dynamicBroadphase->debugMode(false);
    }

void fe::gameWorld::setStaticBroadphase(fe::broadphaseAbstract *broadphase)
    {
        m_staticBroadphase = broadphase;
        m_staticBroadphase->startUp();
        m_staticBroadphase->debugMode(false);
    }

fe::broadphaseAbstract *fe::gameWorld::getDynamicBroadphase() const
    {
        return m_dynamicBroadphase;
    }

fe::broadphaseAbstract *fe::gameWorld::getStaticBroadphase() const
    {
        return m_staticBroadphase;
    }

const fe::sceneGraph &fe::gameWorld::getSceneGraph() const
    {
        return m_sceneGraph;
    }

fe::sceneGraph &fe::gameWorld::getSceneGraph()
    {
        return m_sceneGraph;
    }

const fe::tileMap &fe::gameWorld::getTileMap() const
    {
        return m_tileMap;
    }

fe::tileMap &fe::gameWorld::getTileMap()
    {
        return m_tileMap;
    }

const fe::graph &fe::gameWorld::getAIGraph() const
    {
        return m_aiGraph;
    }

fe::graph &fe::gameWorld::getAIGraph()
    {
        return m_aiGraph;
    }

const fe::componentManager &fe::gameWorld::getComponentManager() const
    {
        return m_componentManager;
    }

fe::componentManager &fe::gameWorld::getComponentManager()
    {
        return m_componentManager;
    }

void fe::gameWorld::preUpdate()
    {
        FE_ENGINE_PROFILE("game_world", "entity_pre_update");
        m_entityWorld.preUpdate();
        FE_END_PROFILE;
    }

void fe::gameWorld::update(collisionWorld *collisionWorld)
    {
        FE_ENGINE_PROFILE("game_world", "entity_update");
        m_entityWorld.update(collisionWorld, m_dynamicBroadphase);
        m_componentManager.update();
        FE_END_PROFILE;
    }

void fe::gameWorld::fixedUpdate(float deltaTime)
    {
        FE_ENGINE_PROFILE("game_world", "entity_fixed_update");
        m_entityWorld.fixedUpdate(deltaTime);
        m_componentManager.fixedUpdate(deltaTime);
        FE_END_PROFILE;
    }

void fe::gameWorld::postUpdate()
    {
        FE_ENGINE_PROFILE("game_world", "entity_post_update");
        m_componentManager.postUpdate();
        m_entityWorld.postUpdate();
        FE_END_PROFILE;
    }

void fe::gameWorld::preDraw()
    {
        m_sceneGraph.preDraw();
    }

void fe::gameWorld::draw(sf::RenderTarget &app)
    {
        m_tileMap.draw(app, m_staticRenderStates);
        m_sceneGraph.draw(app);
        if (m_dynamicBroadphase) m_dynamicBroadphase->debugDraw();
        if (m_staticBroadphase) m_staticBroadphase->debugDraw();
    }

void fe::gameWorld::save(const std::string &save)
    {
        std::ofstream out(save);
        this->save(out);
        out.close();
    }

void fe::gameWorld::save(std::ofstream &out)
    {
        save();
        m_serializer->outData(out);
    }

void fe::gameWorld::save()
    {
        m_serializer->clearData();
        FE_LOG("Saving Game World");
        FE_LOG("Saving Entities");
        m_entityWorld.serialize(*m_serializer);
        FE_LOG("Saving AI tilemap");
        m_aiGraph.serialize(*m_serializer);
        FE_LOG("Saving Tilemap");
        m_tileMap.serialize(*m_serializer);
    }

void fe::gameWorld::load(const std::string &load)
    {
        std::ifstream in(load);
        this->load(in);
        in.close();
    }

void fe::gameWorld::load(std::ifstream &in)
    {
        FE_LOG("Reading File");
        m_serializer->clearData();
        m_serializer->readData(in);
        load();
    }

void fe::gameWorld::load()
    {
        FE_LOG("Loading Game World");
        FE_LOG("Loading Tilemap");
        m_tileMap.clearMap();
        m_tileMap.deserialize(*m_serializer);
        m_tileMap.rebuildTilemap();
        FE_LOG("Loading AI tilemap");
        m_aiGraph.clear();
        m_aiGraph.deserialize(*m_serializer);
        FE_LOG("Loading Entities");
        m_componentManager.clearAllComponents();
        m_entityWorld.clearAllObjects();
        m_entityWorld.deserialize(*m_serializer);
    }

void fe::gameWorld::loadTilePrefabs(const char *filepath)
    {
        fe::serializerID prefabSerial;
        std::ifstream in(filepath);
        prefabSerial.readData(in);
        in.close();

        m_tileMap.deserializeFabrications(prefabSerial);
    }

fe::Handle fe::gameWorld::addGameObject(fe::entityModules modules, fe::userEntityObject *scriptObject, int connected, const fe::fontData &data)
    {
        fe::baseEntity *object = m_entityWorld.addGameObject(modules, scriptObject, connected, data);
        if (object->getCollider())
            {
                if (object->getCollider()->m_static && m_staticBroadphase)
                    {
                        m_staticBroadphase->add(object->getCollider());
                    }
                else
                    {
                        m_dynamicBroadphase->add(object->getCollider());
                    }
            }
        
        // Entity Spawner Logic

        return object->getHandle();
    }

void fe::gameWorld::removeGameObject(Handle handle)
    {
        // Entity Spawner Logic

        m_entityWorld.removeObject(handle);
    }

fe::baseEntity *fe::gameWorld::getObject(Handle handle) const
    {
        return m_entityWorld.getObject(handle);
    }

void fe::gameWorld::getEntitiesWithinArea(std::vector<fe::baseEntity*> *entities, float xPos, float yPos, float xSize, float ySize)
    {
        getEntitiesWithinArea(entities, fe::Vector2d(xPos, yPos), fe::Vector2d(xSize, ySize));
    }

void fe::gameWorld::getEntitiesWithinArea(std::vector<fe::baseEntity*> *entities, fe::Vector2d pos, fe::Vector2d size)
    {
        fe::AABB aabb(size.x, size.y);
        aabb.m_globalPositionX = pos.x;
        aabb.m_globalPositionY = pos.y;

        if (m_dynamicBroadphase) 
            {
                m_dynamicBroadphase->colliderAABB(aabb, std::bind(&addEntityToVector, &aabb, entities, m_entityWorld, std::placeholders::_1));
            }

        if (m_staticBroadphase)
            {
                m_staticBroadphase->colliderAABB(aabb, std::bind(&addEntityToVector, &aabb, entities, m_entityWorld, std::placeholders::_1));
            }
    }

void fe::gameWorld::getEntitiesWithinArea(std::vector<fe::baseEntity*> *entities, float xPos, float yPos, float xSize, float ySize, fe::str tag)
    {
        getEntitiesWithinArea(entities, fe::Vector2d(xPos, yPos), fe::Vector2d(xSize, ySize), tag);
    }

void fe::gameWorld::getEntitiesWithinArea(std::vector<fe::baseEntity*> *entities, fe::Vector2d pos, fe::Vector2d size, fe::str tag)
    {
        fe::AABB aabb(size.x, size.y);
        aabb.m_globalPositionX = pos.x;
        aabb.m_globalPositionY = pos.y;

        if (m_dynamicBroadphase) 
            {
                m_dynamicBroadphase->colliderAABB(aabb, std::bind(&addTaggedEntityToVector, &aabb, entities, m_entityWorld, tag, std::placeholders::_1));
            }

        if (m_staticBroadphase)
            {
                m_staticBroadphase->colliderAABB(aabb, std::bind(&addTaggedEntityToVector, &aabb, entities, m_entityWorld, tag, std::placeholders::_1));
            }
    }

void fe::gameWorld::getEntitiesWithinRange(std::vector<fe::baseEntity*> *entities, float xPos, float yPos, float range)
    {
        getEntitiesWithinRange(entities, fe::Vector2d(xPos, yPos), range);
    }

void fe::gameWorld::getEntitiesWithinRange(std::vector<fe::baseEntity*> *entities, fe::Vector2d pos, float range)
    {
        fe::circle circle(range);
        circle.m_globalPositionX = pos.x;
        circle.m_globalPositionY = pos.y;

        if (m_dynamicBroadphase)
            {
                m_dynamicBroadphase->colliderCircle(circle, std::bind(&addEntityToVector, &circle, entities, m_entityWorld, std::placeholders::_1));
            }

        if (m_staticBroadphase)
            {
                m_staticBroadphase->colliderCircle(circle, std::bind(&addEntityToVector, &circle, entities, m_entityWorld, std::placeholders::_1));
            }
    }

void fe::gameWorld::getEntitiesWithinRange(std::vector<fe::baseEntity*> *entities, float xPos, float yPos, float range, fe::str tag)
    {
        getEntitiesWithinRange(entities, fe::Vector2d(xPos, yPos), range, tag);
    }

void fe::gameWorld::getEntitiesWithinRange(std::vector<fe::baseEntity*> *entities, fe::Vector2d pos, float range, fe::str tag)
    {
        fe::circle circle(range);
        circle.m_globalPositionX = pos.x;
        circle.m_globalPositionY = pos.y;

        if (m_dynamicBroadphase)
            {
                m_dynamicBroadphase->colliderCircle(circle, std::bind(&addTaggedEntityToVector, &circle, entities, m_entityWorld, tag, std::placeholders::_1));
            }

        if (m_staticBroadphase)
            {
                m_staticBroadphase->colliderCircle(circle, std::bind(&addTaggedEntityToVector, &circle, entities, m_entityWorld, tag, std::placeholders::_1));
            }
    }
