#include "fe/subsystems/gameState/entityWorld.hpp"
#include "fe/subsystems/gameState/gameState.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/subsystems/collision/broadphaseAbstract.hpp"
#include "fe/subsystems/collision/collisionWorld.hpp"
#include "fe/subsystems/serializer/serializerID.hpp"
#include "fe/subsystems/entitySpawner/entitySpawner.hpp"

void fe::entityWorld::onAdd(fe::baseEntity **object, fe::Handle objectHandle)
    {
        if (objectHandle < 0) return;
        (*object)->setHandle(objectHandle);
    }

void fe::entityWorld::onRemove(fe::baseEntity **object, fe::Handle objectHandle)
    {
        if (objectHandle < 0) return;
        if (m_gameWorld.getDynamicBroadphase())
            {
                m_gameWorld.getDynamicBroadphase()->remove((*object)->getCollider());
            }
        (*object)->deinitialize(m_gameWorld);
    }

fe::entityWorld::entityWorld(fe::gameWorld &gameWorld) : m_gameWorld(gameWorld)
    {
    }

void fe::entityWorld::startUp()
    {
        m_entityAllocater.startUp(FE_MAX_GAME_OBJECTS);
        m_animator.startUp();
    }

void fe::entityWorld::preUpdate()
    {
        while (!m_newEntities.empty())
            {
                m_newEntities.top()->onAdd(m_gameWorld);
                m_newEntities.pop();
            }
    }

void fe::entityWorld::update(collisionWorld *collisionWorld, broadphaseAbstract *dynamicBroadphase)
    {
        m_animator.updateTextures();
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

void fe::entityWorld::fixedUpdate(float deltaTime)
    {
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (m_objects[i] && m_objects[i]->m_enabled)
                    {
                        m_objects[i]->fixedUpdate(deltaTime);
                    }
            }
    }

void fe::entityWorld::postUpdate()
    {
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (m_objects[i])
                    {
                        if (m_objects[i]->m_enabled)
                            {
                                m_objects[i]->postUpdate();
                                m_objects[i]->updateModules();
                            }

                        if (m_objects[i]->isKilled())
                            {
                                removeObject(m_objects[i]->getHandle());
                                i--;
                            }
                    }
            }
    }

fe::baseEntity *fe::entityWorld::addGameObject(fe::entityModules modules, fe::userEntityObject *scriptObject, int connected, const fe::fontData &data)
    {
        fe::baseEntity *entity = m_entityAllocater.alloc(modules, scriptObject, false);
        fe::Handle objHandle = addObject(entity);
        fe::baseEntity *object = getObject(objHandle);

        object->createModules(m_gameWorld.getGameState());

        if (object->getRenderObject())
            {
                object->getRenderObject()->initialize(fe::lightVector2<unsigned int>(), 0);
            }

        object->initialize(m_gameWorld, connected, data);

        m_newEntities.push(object);

        return object;
    }

void fe::entityWorld::clearAllObjects()
    {
        fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>::clearAllObjects();
        m_entityAllocater.clear();
    }

void fe::entityWorld::removeObject(fe::Handle handle)
    {
        getObject(handle)->onRemove(m_gameWorld);
        m_gameWorld.getGameState().m_entitySpawner.despawn(handle);
        fe::baseEntity *entAtHandle = getObject(handle);
        fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>::removeObject(handle);
        m_entityAllocater.free(entAtHandle);
    }

fe::baseEntity *fe::entityWorld::getObject(fe::Handle handle) const
    {
        return fe::handleManager<fe::baseEntity*, FE_MAX_GAME_OBJECTS>::getObject(handle);
    }

void fe::entityWorld::getAllObjects(std::vector<fe::baseEntity*> &objects)
    {
        for (unsigned int i = 0; i < m_entityAllocater.getObjectAllocCount(); i++)
            {
                if (m_entityAllocater.allocatedAt(i))
                    {
                        objects.push_back(m_entityAllocater.at(i));
                    }
            }
    }

void fe::entityWorld::getAllObjectsByTag(std::vector<fe::baseEntity*> &objects, const char *tag)
    {
        getAllObjectsByTag(objects, FE_STR(tag));
    }

void fe::entityWorld::getAllObjectsByTag(std::vector<fe::baseEntity*> &objects, fe::str tag)
    {
        for (unsigned int i = 0; i < m_entityAllocater.getObjectAllocCount(); i++)
                {
                    if (m_entityAllocater.allocatedAt(i) && m_entityAllocater.at(i)->getTag() == tag)
                        {
                            objects.push_back(m_entityAllocater.at(i));
                        }
                }
    }

void fe::entityWorld::serialize(fe::serializerID &serializer)
    {
        for (unsigned int i = 0; i < objectCount(); i++)
            {
                if (m_objects[i] && m_objects[i]->isSerializable())
                    {
                        entityRepresentation entity(m_gameWorld);
                        entity.m_enabled = m_objects[i]->getEnabled();
                        entity.m_handle = m_objects[i]->getHandle();
                        entity.m_name = m_objects[i]->getName();
                        entity.m_positionX = m_objects[i]->getPosition().x;
                        entity.m_positionY = m_objects[i]->getPosition().y;
                        entity.m_components = m_objects[i]->getAllComponents();

                        serializer.writeObjectList("entities", entity);
                    }
            }
    }

void fe::entityWorld::deserialize(fe::serializerID &serializer)
    {
        clearAllObjects();
        while (serializer.listHasItems("entities"))
            {
                entityRepresentation entity(m_gameWorld);
                serializer.readObjectList("entities", entity);
            }
    }

fe::animator &fe::entityWorld::getAnimator()
    {
        return m_animator;
    }

bool fe::entityWorld::isEntity(void *ptr) const
    {
        return m_entityAllocater.inRange(ptr);
    }

void fe::entityWorld::entityRepresentation::serialize(fe::serializerID &serializer) const
    {
        serializer.write("enabled", m_enabled);
        serializer.write("handle", int(m_handle));
        serializer.write("name", m_name);
        serializer.write("positionX", m_positionX);
        serializer.write("positionY", m_positionY);

        for (unsigned int i = 0; i < m_components.size(); i++)
            {
                fe::componentBase *comp = m_gameWorld.getComponentManager().getComponent(m_components[i].first);
                serializer.writeObjectList("components", *comp);
            }
    }

void fe::entityWorld::entityRepresentation::deserialize(fe::serializerID &serializer)
    {
        m_enabled = serializer.read<bool>("enabled");
        m_handle = serializer.read<int>("handle");
        m_name = serializer.read<std::string>("name");
        m_positionX = serializer.read<float>("positionX");
        m_positionY = serializer.read<float>("positionY");

        fe::baseEntity *ent = m_gameWorld.getObject(m_gameWorld.getGameState().addObject(m_name.c_str()));
        ent->setPosition(m_positionX, m_positionY);
        ent->enable(m_enabled);

        ent->enablePhysics(!m_gameWorld.getGameState().isPaused());
        ent->enableCollision(!m_gameWorld.getGameState().isPaused());

        for (unsigned int i = 0; i < ent->getAllComponents().size(); i++)
            {
                fe::componentBase *comp = m_gameWorld.getComponentManager().getComponent(ent->getAllComponents()[i].first);
                serializer.readObjectList("components", *comp);
            }
    }
