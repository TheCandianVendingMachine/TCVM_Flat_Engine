#include "fe/entity/component/componentManager.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"

void fe::componentManager::startUp(fe::baseGameState *state)
    {
        m_state = state;
    }

fe::baseGameState *fe::componentManager::getState() const
    {
        return m_state;
    }

fe::componentBase *fe::componentManager::getComponent(fe::Handle handle) const
    {
        return m_components.getObject(handle);
    }

sol::object fe::componentManager::getLuaComponent(const char *id, fe::scriptObject *entity, sol::this_state state) const
    {
        fe::str idStr = FE_STR(id);
        for (auto &compPair : entity->getBaseEntity()->getAllComponents())
            {
                fe::componentBase *comp = m_components.getObject(compPair.first);
                if (comp->getID() == idStr)
                    {
                        return comp->makeIntoLuaObject(sol::state_view(state.L));
                    }
            }

        return sol::object();
    }

void fe::componentManager::addComponentToObject(fe::baseEntity *ent, const std::string &entName, const std::string &compName, const std::string &compLuaPath, sol::table table)
    {
        fe::str str = FE_STR(compName.c_str());
        if (m_componentLookupTable.find(str) != m_componentLookupTable.end())
            {
                fe::Handle compHandle = m_components.addObject(m_componentLookupTable[str]->create());
                
                m_components.getObject(compHandle)->setState(m_state);
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
        for (unsigned int i = 0; i < m_components.objectCount(); i++)
            {
                fe::componentBase *component = m_components.getObject(i);
                if (component->getOwner()->getBaseEntity()->isComponentEnabled(i))
                    {
                        component->engineUpdate();
                    }
            }
    }

void fe::componentManager::fixedUpdate(float deltaTime)
    {
        for (unsigned int i = 0; i < m_components.objectCount(); i++)
            {
                fe::componentBase *component = m_components.getObject(i);
                if (component->getOwner()->getBaseEntity()->isComponentEnabled(i))
                    {
                        component->engineFixedUpdate(deltaTime);
                    }
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
        clearAllComponents();
        for (auto &proxy : m_componentLookupTable)
            {
                delete proxy.second;
                proxy.second = nullptr;
            }
    }

void fe::componentManager::clearAllComponents()
    {
        m_components.clearAllObjects([this](componentBase **c) {
            (*c)->engineOnRemove((*c)->getOwner()->getBaseEntity());
            delete *c;
            *c = nullptr;
        });
    }
