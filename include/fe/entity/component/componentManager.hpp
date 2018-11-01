// componentManager.hpp
// Loops through all components and calls their relavent functions
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/memory/poolAllocater.hpp"
#include <memory>
#include <type_traits>
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

                    FLAT_ENGINE_API fe::componentBase *getComponentFromScriptObject(fe::scriptObject *ent, const char *compName) const;

                public:
                    FLAT_ENGINE_API void startUp();

                    template<typename T>
                    void setComponentProxy(const char *name);

                    FLAT_ENGINE_API void addComponentToObject(fe::baseEntity *ent, const char *entName, const char *compName, const char *compLuaPath, sol::table table);
                    FLAT_ENGINE_API fe::componentBase *getComponentFromObject(fe::baseEntity *ent, const char *compName) const;
                    FLAT_ENGINE_API void removeComponentFromObject(fe::baseEntity *ent, fe::Handle handle);

                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void fixedUpdate(float deltaTime);
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void shutDown();

            };

        template<typename T>
        void componentManager::setComponentProxy(const char *name)
            {
                static_assert(std::is_base_of<component<T>, T>::value, "Component added does not have base of fe::component");
                FE_ASSERT(m_componentLookupTable.find(FE_STR(name)) == m_componentLookupTable.end(), "Component already exists in table");

                m_componentLookupTable[FE_STR(name)] = new componentProxy<T>;
            }
    }
