// componentManager.hpp
// Loops through all components and calls their relavent functions
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/memory/poolAllocater.hpp"
#include <memory>
#include <type_traits>
#include <sol.hpp>
#include <unordered_map>
#include "fe/entity/component/component.hpp"
#include "fe/objectManagement/handleManager.hpp"

namespace fe
    {
        class componentManager
            {
                private:
                    struct componentProxyBase { virtual componentBase *create() { return nullptr; } };
                    template<typename T>
                    struct componentProxy : componentProxyBase
                        {
                            componentBase *create() { return new T; }
                        };

                    fe::handleManager<componentBase*, 0> m_components;
                    std::unordered_map<fe::str, componentProxyBase*> m_componentLookupTable;

                public:
                    FLAT_ENGINE_API void startUp();

                    template<typename T>
                    void setComponentProxy(const std::string &name);

                    template<typename T>
                    T *getComponent(const char *id, fe::baseEntity *entity) const;
                    FLAT_ENGINE_API fe::componentBase *getComponent(fe::Handle handle) const;
                    FLAT_ENGINE_API sol::object getLuaComponent(const char *id, fe::scriptObject *entity, sol::this_state state) const;

                    FLAT_ENGINE_API void addComponentToObject(fe::baseEntity *ent, const std::string &entName, const std::string &compName, const std::string &compLuaPath, sol::table table);
                    FLAT_ENGINE_API void removeComponentFromObject(fe::baseEntity *ent, fe::Handle handle);

                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void fixedUpdate(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clearAllComponents();

            };

        template<typename T>
        void componentManager::setComponentProxy(const std::string &name)
            {
                static_assert(std::is_base_of<component<T>, T>::value, "Component added does not have base of fe::component");
                FE_ASSERT(m_componentLookupTable.find(FE_STR(name.c_str())) == m_componentLookupTable.end(), "Component already exists in table");

                m_componentLookupTable[FE_STR(name.c_str())] = new componentProxy<T>;
            }

        template<typename T>
        T *componentManager::getComponent(const char *id, fe::baseEntity *entity) const
            {
                static_assert(std::is_base_of<component<T>, T>::value, "Component cast does not have base of fe::component");

                fe::str idStr = FE_STR(id);

                for (auto &compPair : entity->getAllComponents())
                    {
                        fe::componentBase *comp = m_components.getObject(compPair.first);
                        if (comp->getID() == idStr)
                            {
                                return static_cast<T*>(comp);
                            }
                    }

                return nullptr;
            }
    }
