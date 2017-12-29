#include "gameWorld.hpp"
#include "../../entity/baseEntity.hpp"
#include "../physics/collision/collisionWorld.hpp"
#include "../physics/collision/broadphaseAbstract.hpp"
#include "../../debug/profiler.hpp"
#include "../resourceManager/resourceManager.hpp"
#include "../../engine.hpp"

void fe::gameWorld::onAdd(fe::baseEntity **object, fe::Handle objectHandle)
    {
        (*object)->setHandle(objectHandle);
    }

void fe::gameWorld::onRemove(fe::baseEntity **object, fe::Handle objectHandle)
    {
        (*object)->deinitialize(*this);
    }

void fe::gameWorld::startUp()
    {
        m_sceneGraph.startUp();
        m_staticRenderStates.texture = &fe::engine::get().getResourceManager<sf::Texture>()->get();
        m_dynamicBroadphase = nullptr;
        m_staticBroadphase = nullptr;
        m_serializer = new fe::serializerID();
    }

void fe::gameWorld::shutDown()
    {
        m_sceneGraph.shutDown();
        delete m_serializer;
        m_serializer = nullptr;
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

void fe::gameWorld::preUpdate()
    {
        FE_ENGINE_PROFILE("game_world", "entity_pre_update");
        auto objects = getObjects();
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (objects[i] && objects[i]->m_enabled)
                    {
                        objects[i]->updateModules();
                    }
            }
        FE_END_PROFILE;
    }

void fe::gameWorld::update(collisionWorld *collisionWorld)
    {
        FE_ENGINE_PROFILE("game_world", "entity_update");
        auto objects = getObjects();
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (objects[i] && objects[i]->m_enabled)
                    {
                        objects[i]->update();

                        if (objects[i]->m_collisionBody && !objects[i]->m_collisionBody->m_static)
                            {
                                m_dynamicBroadphase->update(objects[i]->m_collisionBody);
                            }
                    }
            }
        FE_END_PROFILE;
    }

void fe::gameWorld::postUpdate()
    {
        FE_ENGINE_PROFILE("game_world", "entity_post_update");
        auto objects = getObjects();
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (objects[i] && objects[i]->m_enabled)
                    {
                        objects[i]->postUpdate();
                    }
            }
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

void fe::gameWorld::save(std::ofstream &out)
    {
        save();
        m_serializer->outData(out);
    }

void fe::gameWorld::save()
    {
        m_serializer->clearData();
        FE_LOG("Saving Game World");
        FE_LOG("Saving Tilemap");
        m_tileMap.serialize(*m_serializer);
    }

void fe::gameWorld::load(std::ifstream &in)
    {
        FE_LOG("Loading Game World");
        FE_LOG("Reading File");
        m_serializer->clearData();
        m_serializer->readData(in);
        load();
    }

void fe::gameWorld::load()
    {
        FE_LOG("Loading Tilemap");
        m_tileMap.clearMap();
        m_tileMap.deserialize(*m_serializer);
        m_tileMap.rebuildTilemap();
        
    }

void fe::gameWorld::loadTilePrefabs(const char *filepath)
    {
        fe::serializerID prefabSerial;
        std::ifstream in(filepath);
        prefabSerial.readData(in);
        in.close();

        m_tileMap.deserializeFabrications(prefabSerial);
    }

void fe::gameWorld::removeGameObject(Handle handle)
    {
        removeObject(handle);
    }

fe::baseEntity *fe::gameWorld::getObject(Handle handle) const
    {
        return fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>::getObject(handle);
    }
