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
                    fe::handleManager<componentBase*, 0> m_components;
                    std::unordered_map<fe::str, fe::Handle> m_componentLookupTable;

                public:
                    FLAT_ENGINE_API void startUp();

                    template<typename T, typename ...Args>
                    void setComponentProxy(const std::string &name, Args &&...args);

                    FLAT_ENGINE_API void addComponentToObject(fe::baseEntity *ent, const std::string &name);
                    FLAT_ENGINE_API void removeComponentFromObject(fe::baseEntity *ent, fe::Handle handle);
                    FLAT_ENGINE_API void initComponentLuaTable(const std::string &name, const std::string &path, sol::table table);

                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void fixedUpdate(float deltaTime);

                    FLAT_ENGINE_API void shutDown();

            };

        template<typename T, typename ...Args>
        void componentManager::setComponentProxy(const std::string &name, Args &&...args)
            {
                static_assert(std::is_base_of<component<T>, T>::value, "Component added does not have base of fe::component");
                FE_ASSERT(m_componentLookupTable.find(FE_STR(name.c_str())) == m_componentLookupTable.end(), "Component already exists in table");

                m_componentLookupTable[FE_STR(name.c_str())] = m_components.addObject(new T(std::forward<Args>(args)...));
            }
    }
