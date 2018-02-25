#include "entityWorld.hpp"
#include "../../entity/baseEntity.hpp"
#include "../physics/collision/broadphaseAbstract.hpp"
#include "../physics/collision/collisionWorld.hpp"
#include "../serializer/serializerID.hpp"
#include "gameState.hpp"
#include "../entitySpawner/entitySpawner.hpp"

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
                        if (m_objects[i]->m_collisionBody && !m_objects[i]->m_collisionBody->m_static)
                            {
                                dynamicBroadphase->update(m_objects[i]->m_collisionBody);
                            }
                        m_objects[i]->update();
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

fe::baseEntity *fe::entityWorld::addGameObject(fe::baseEntity *entity, int connected, const fe::fontData &data)
    {
        fe::Handle objHandle = addObject(entity);
        fe::baseEntity *object = getObject(objHandle);

        object->createModules();

        if (object->getRenderObject())
            {
                object->getRenderObject()->initialize(fe::lightVector2<unsigned int>(), 0);
            }

        object->initialize(m_gameWorld, connected, data);
        object->onAdd(m_gameWorld);

        return object;
    }

void fe::entityWorld::clearAllObjects()
    {
        fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>::clearAllObjects();
    }

void fe::entityWorld::removeObject(fe::Handle handle)
    {
        getObject(handle)->onRemove(m_gameWorld);
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
                entityRepresentation entity;
                entity.m_enabled = m_objects[i]->getEnabled();
                entity.m_handle = m_objects[i]->getHandle();
                entity.m_name = m_objects[i]->getName();
                entity.m_positionX = m_objects[i]->getPosition().x;
                entity.m_positionY = m_objects[i]->getPosition().y;

                serializer.writeObjectList("entities", entity);
            }
    }

void fe::entityWorld::deserialize(fe::serializerID &serializer)
    {
        while (serializer.listHasItems("entities"))
            {
                entityRepresentation entity;
                serializer.readObjectList("entities", entity);

                fe::baseEntity *ent = m_gameWorld.getObject(m_gameWorld.getGameState().addObject(entity.m_name.c_str()));
                ent->setPosition(entity.m_positionX, entity.m_positionY);
                ent->enable(entity.m_enabled);
            }
    }

void fe::entityWorld::entityRepresentation::serialize(fe::serializerID &serializer) const
    {
        serializer.write("enabled", m_enabled);
        serializer.write("handle", int(m_handle));
        serializer.write("name", m_name);
        serializer.write("positionX", m_positionX);
        serializer.write("positionY", m_positionY);
    }

void fe::entityWorld::entityRepresentation::deserialize(fe::serializerID &serializer)
    {
        m_enabled = serializer.read<bool>("enabled");
        m_handle = serializer.read<int>("handle");
        m_name = serializer.read<std::string>("name");
        m_positionX = serializer.read<float>("positionX");
        m_positionY = serializer.read<float>("positionY");
    }
