#include "gameWorld.hpp"
#include "../../entity/baseEntity.hpp"
#include "../physics/collision/collisionWorld.hpp"
#include "../physics/collision/broadphaseAbstract.hpp"
#include "../../debug/profiler.hpp"

void fe::gameWorld::onAdd(fe::baseEntity *object, fe::Handle objectHandle)
    {
        object->initialize();
        object->setHandle(objectHandle);
        if (object->m_collisionBody)
            {
                m_broadphase->add(object->m_collisionBody);
            }
    }

void fe::gameWorld::onRemove(fe::baseEntity * object, fe::Handle objectHandle)
    {
        object->deinitialize();
    }

void fe::gameWorld::startUp()
    {
        m_sceneGraph.startUp();
    }

void fe::gameWorld::shutDown()
    {
        m_sceneGraph.shutDown();
    }

void fe::gameWorld::setBroadphase(fe::broadphaseAbstract *broadphase)
    {
        m_broadphase = broadphase;
        m_broadphase->startUp();
    }

fe::broadphaseAbstract *fe::gameWorld::getBroadphase() const
    {
        return m_broadphase;
    }

fe::sceneGraph &fe::gameWorld::getSceneGraph()
    {
        return m_sceneGraph;
    }

void fe::gameWorld::preUpdate()
    {
        auto objects = getObjects();
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (objects[i] && objects[i]->m_enabled)
                    {
                        objects[i]->updateModules();
                    }
            }
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

                        if (objects[i]->m_moved && objects[i]->m_collisionBody)
                            {
                                m_broadphase->update(objects[i]->m_collisionBody);
                            }
                    }
            }
        FE_END_PROFILE;
    }

void fe::gameWorld::postUpdate()
    {
        auto objects = getObjects();
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (objects[i] && objects[i]->m_enabled)
                    {
                        objects[i]->postUpdate();
                    }
            }
    }

void fe::gameWorld::preDraw()
    {
        m_sceneGraph.preDraw();
    }

void fe::gameWorld::draw(sf::RenderTarget &app)
    {
        m_sceneGraph.draw(app);
        if (m_broadphase) m_broadphase->debugDraw();
    }
