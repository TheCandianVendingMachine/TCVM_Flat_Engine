#include "fe/entity/component/componentManager.hpp"
#include "fe/entity/baseEntity.hpp"

void fe::componentManager::startUp()
    {
    }

void fe::componentManager::addComponentToObject(fe::baseEntity *ent, const std::string &name)
    {
        fe::str str = FE_STR(name.c_str());
        if (m_componentLookupTable.find(str) != m_componentLookupTable.end())
            {
                ent->addComponent(m_componentLookupTable[str]);
                m_components.getObject(m_componentLookupTable[str])->engineOnAdd(ent);
                FE_LOG_DEBUG("Added component [", name, "] to entity [", ent->getName(), "]");
            }
        else
            {
                FE_LOG_ERROR("Component with name [", name, "] does not exist");
            }
    }

void fe::componentManager::removeComponentFromObject(fe::baseEntity *ent, fe::Handle handle)
    {
        m_components.getObject(handle)->engineOnRemove(ent);
        ent->removeComponent(handle);
        FE_LOG_DEBUG("Removed component with handle [", handle, "] from entity [", ent->getName(), "]");
    }

void fe::componentManager::initComponentLuaTable(const std::string &name, const std::string &path, sol::table table)
    {
        fe::str str = FE_STR(name.c_str());
        if (m_componentLookupTable.find(str) != m_componentLookupTable.end())
            {
                fe::componentBase *comp = m_components.getObject(m_componentLookupTable[str]);
                if (!comp->isInitialized()) 
                    {
                        comp->engineInitLuaValues(table, path.c_str());
                    }
            }
        else
            {
                FE_LOG_ERROR("[", name, "] does not exist in lookup table -- cannot init Lua table");
            }
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

void fe::componentManager::shutDown()
    {
        m_components.clearAllObjects([](componentBase **c) { delete *c; *c = nullptr; });
    }
