#include "fe/entity/component/componentManager.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/scripting/scriptManager.hpp"

fe::componentBase *fe::componentManager::getComponentFromScriptObject(fe::scriptObject *ent, const char *compName) const
    {
        return getComponentFromObject(ent->getBaseEntity(), compName);
    }

void fe::componentManager::startUp()
    {
        fe::engine::get().getScriptManager().getFunctionHandler().registerCPPObjectFunction("getComponentFromEntity", this, &fe::componentManager::getComponentFromScriptObject);
    }

void fe::componentManager::addComponentToObject(fe::baseEntity *ent, const char *entName, const char *compName, const char *compLuaPath, sol::table table)
    {
        fe::str str = FE_STR(compName);
        if (m_componentLookupTable.find(str) != m_componentLookupTable.end())
            {
                fe::Handle compHandle = m_components.addObject(m_componentLookupTable[str]->create());
                
                m_components.getObject(compHandle)->engineInitLuaValues(table, compLuaPath, entName);
                m_components.getObject(compHandle)->engineOnAdd(ent);

                ent->addComponent(compHandle);
                FE_LOG_DEBUG("Added component [", compName, "] to entity [", ent->getName(), "]");
            }
        else
            {
                FE_LOG_ERROR("Component with name [", compName, "] does not exist");
            }
    }

fe::componentBase *fe::componentManager::getComponentFromObject(fe::baseEntity *ent, const char *compName) const
    {
        fe::str lookingStr = FE_STR(compName);
        for (auto &compPair : ent->getAllComponents())
            {
                if (m_components.getObject(compPair.first)->getID() == lookingStr)
                    {
                        return m_components.getObject(compPair.first);
                    }
            }

        return nullptr;
    }

void fe::componentManager::removeComponentFromObject(fe::baseEntity *ent, fe::Handle handle)
    {
        m_components.getObject(handle)->engineOnRemove(ent);
        ent->removeComponent(handle);
        FE_LOG_DEBUG("Removed component with handle [", handle, "] from entity [", ent->getName(), "]");
    }

void fe::componentManager::update()
    {
        for (auto &component : m_components.getObjects())
            {
                component->engineUpdate();
            }
    }

void fe::componentManager::fixedUpdate(float deltaTime)
    {
        for (auto &component : m_components.getObjects())
            {
                component->engineFixedUpdate(deltaTime);
            }
    }

void fe::componentManager::postUpdate()
    {
        for (unsigned int i = 0; i < m_components.objectCount(); i++)
            {
                if (m_components.handleActive(i))
                    {
                        fe::baseEntity *entOwner = m_components.getObject(i)->getOwner()->getBaseEntity();
                        if (entOwner->isKilled())
                            {
                                removeComponentFromObject(entOwner, i);
                                m_components.removeObject(i);
                            }
                    }
            }
    }

void fe::componentManager::shutDown()
    {
        m_components.clearAllObjects([](componentBase **c) { delete *c; *c = nullptr; });
        for (auto &proxy : m_componentLookupTable)
            {
                delete proxy.second;
                proxy.second = nullptr;
            }
    }
