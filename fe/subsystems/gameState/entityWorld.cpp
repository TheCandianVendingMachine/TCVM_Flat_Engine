#include "entityWorld.hpp"
#include "../../entity/baseEntity.hpp"
#include "../physics/collision/broadphaseAbstract.hpp"
#include "../physics/collision/collisionWorld.hpp"
#include "../serializer/serializerID.hpp"

void fe::entityWorld::onAdd(fe::baseEntity **object, fe::Handle objectHandle)
    {
        (*object)->setHandle(objectHandle);
    }

void fe::entityWorld::onRemove(fe::baseEntity **object, fe::Handle objectHandle)
    {
        (*object)->deinitialize(m_gameWorld);
    }

fe::entityWorld::entityWorld(fe::gameWorld &gameWorld) : m_gameWorld(gameWorld)
    {
    }

void fe::entityWorld::preUpdate()
    {
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (m_objects[i] && m_objects[i]->m_enabled)
                    {
                        m_objects[i]->updateModules();
                    }
            }
    }

void fe::entityWorld::update(collisionWorld *collisionWorld, broadphaseAbstract *dynamicBroadphase)
    {
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (m_objects[i] && m_objects[i]->m_enabled)
                    {
                        m_objects[i]->update();

                        if (m_objects[i]->m_collisionBody && !m_objects[i]->m_collisionBody->m_static)
                            {
                                dynamicBroadphase->update(m_objects[i]->m_collisionBody);
                            }
                    }
            }
    }

void fe::entityWorld::postUpdate()
    {
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (m_objects[i] && m_objects[i]->m_enabled)
                    {
                        m_objects[i]->postUpdate();
                    }
            }
    }

fe::baseEntity *fe::entityWorld::addGameObject(fe::baseEntity *entity, int connected, fe::fontData &data)
    {
        fe::Handle objHandle = addObject(entity);
        fe::baseEntity *object = getObject(objHandle);

        object->createModules();

        if (object->getRenderObject())
            {
                object->getRenderObject()->initialize(fe::lightVector2<unsigned int>(), 0);
            }

        object->initialize(m_gameWorld, connected, data);

        return object;
    }

void fe::entityWorld::clearAllObjects()
    {
        fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>::clearAllObjects();
    }

void fe::entityWorld::removeObject(fe::Handle handle)
    {
        fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>::removeObject(handle);
    }

fe::baseEntity *fe::entityWorld::getObject(fe::Handle handle) const
    {
        return fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>::getObject(handle);
    }

void fe::entityWorld::serialize(fe::serializerID &serializer)
    {
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                serializer.writeObjectList("entities", *m_objects[i]);
            }
    }

void fe::entityWorld::deserialize(fe::serializerID &serializer)
    {
        while (serializer.listHasItems("entities"))
            {
                fe::baseEntity *ent = new fe::baseEntity();
                serializer.readObjectList("entities", *ent);

                fe::sceneGraphObject *renderObj = ent->getRenderObject();
                // To DRY we call fe::gameWorld::addGameObject to handle the addition of the game object. It calls fe::entityWorld::addGameObject which handles init
                // Kinda backwards, but it avoids code duplication
                if (renderObj)
                    {
                        fe::lightVector2<unsigned int> texCoords(renderObj->m_texCoords[0], renderObj->m_texCoords[1]);
                        if (renderObj->m_type == fe::RENDER_OBJECT_TYPE::TEXT)
                            {
                                m_gameWorld.addGameObject(ent, -1, static_cast<fe::renderText*>(renderObj)->m_fontData);
                            }
                        else
                            {
                                m_gameWorld.addGameObject(ent, -1);
                            }
                    }
                else
                    {
                        m_gameWorld.addGameObject(ent, -1);
                    }
            }
    }
