#include "fe/entity/component/componentManager.hpp"
#include "fe/entity/baseEntity.hpp"

void fe::componentManager::startUp()
    {
    }

void fe::componentManager::addComponentToObject(fe::baseEntity *ent, const std::string &entName, const std::string &compName, const std::string &compLuaPath, sol::table table)
    {
        fe::str str = FE_STR(compName.c_str());
        if (m_componentLookupTable.find(str) != m_componentLookupTable.end())
            {
                fe::Handle compHandle = m_components.addObject(m_componentLookupTable[str]->create());
                
                m_components.getObject(compHandle)->engineInitLuaValues(table, compLuaPath.c_str(), entName.c_str());
                m_components.getObject(compHandle)->engineOnAdd(ent);

                ent->addComponent(compHandle);
                FE_LOG_DEBUG("Added component [", compName, "] to entity [", ent->getName(), "]");
            }
        else
            {
                FE_LOG_ERROR("Component with name [", compName, "] does not exist");
            }
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

void fe::componentManager::shutDown()
    {
        m_components.clearAllObjects([](componentBase **c) { delete *c; *c = nullptr; });
        for (auto &proxy : m_componentLookupTable)
            {
                delete proxy.second;
                proxy.second = nullptr;
            }
    }
