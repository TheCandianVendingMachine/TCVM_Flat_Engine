#include "gameWorld.hpp"
#include "../../entity/baseEntity.hpp"
#include "../physics/collision/collisionWorld.hpp"
#include "../physics/collision/broadphaseAbstract.hpp"
#include "../../debug/profiler.hpp"

void fe::gameWorld::onAdd(fe::baseEntity *object, fe::Handle objectHandle)
    {
        object->setHandle(objectHandle);
    }

void fe::gameWorld::onRemove(fe::baseEntity *object, fe::Handle objectHandle)
    {
        object->deinitialize();
    }

void fe::gameWorld::startUp()
    {
        m_sceneGraph.startUp();
        m_dynamicBroadphase = nullptr;
        m_staticBroadphase = nullptr;
    }

void fe::gameWorld::shutDown()
    {
        m_sceneGraph.shutDown();
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

fe::sceneGraph &fe::gameWorld::getSceneGraph()
    {
        return m_sceneGraph;
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
        m_sceneGraph.draw(app);
        if (m_dynamicBroadphase) m_dynamicBroadphase->debugDraw();
        if (m_staticBroadphase) m_staticBroadphase->debugDraw();
    }

void fe::gameWorld::removeGameObject(Handle handle)
    {
        removeObject(handle);
    }

fe::baseEntity *fe::gameWorld::getObject(Handle handle) const
    {
        return fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>::getObject(handle);
    }
