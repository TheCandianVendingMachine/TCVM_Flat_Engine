#include "entityWorld.hpp"
#include "../../entity/baseEntity.hpp"
#include "../physics/collision/broadphaseAbstract.hpp"
#include "../physics/collision/collisionWorld.hpp"

void fe::entityWorld::onAdd(fe::baseEntity **object, fe::Handle objectHandle)
    {
        (*object)->setHandle(objectHandle);
    }

void fe::entityWorld::onRemove(fe::baseEntity **object, fe::Handle objectHandle)
    {
        (*object)->deinitialize(m_gameWorld);
    }

void fe::entityWorld::onSave(fe::serializerID &serial) const
    {
        return void();
    }

void fe::entityWorld::onLoad(fe::serializerID &serial)
    {
        std::vector<fe::baseEntity*> objCopy;
        std::copy(std::begin(m_objects), std::end(m_objects), std::back_inserter(objCopy));
        clearAllObjects();

        for (auto &obj : objCopy)
            {
                if (!obj) break;

                obj->m_enabledModulesEnum = fe::entityModules(obj->m_enabledModulesNum);

                fe::sceneGraphObject *renderObj = obj->getRenderObject();
                if (renderObj)
                    {
                        fe::lightVector2<unsigned int> texCoords(renderObj->m_texCoords[0], renderObj->m_texCoords[1]);
                        if (renderObj->m_type == fe::RENDER_OBJECT_TYPE::TEXT)
                            {
                                m_gameWorld.addGameObject(obj, texCoords, renderObj->m_zPosition, -1, static_cast<fe::renderText*>(renderObj)->m_fontData);
                            }
                        else
                            {
                                m_gameWorld.addGameObject(obj, texCoords, renderObj->m_zPosition, -1);
                            }
                    }
            }
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
