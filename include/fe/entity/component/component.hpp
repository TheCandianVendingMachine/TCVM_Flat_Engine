// component.hpp
// A component base class for the entity. Allows users to implement functionality without having to modify the base class
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/scripting/scriptManager.hpp"
#include "fe/subsystems/scripting/luaFunctionReference.hpp"
#include "fe/engine.hpp"
#include <sol.hpp>
#include <utility>

namespace fe
    {
        class baseEntity;
        class component
            {
                private:
                    fe::luaFunctionReference *m_onAdd;
                    fe::luaFunctionReference *m_onRemove;
                    fe::luaFunctionReference *m_update;
                    fe::luaFunctionReference *m_fixedUpdate;

                protected:
                    virtual void onAdd(fe::baseEntity *ent) {}
                    virtual void onRemove(fe::baseEntity *ent) {}
                    virtual void update(fe::baseEntity *ent) {}
                    virtual void fixedUpdate(fe::baseEntity *ent, float deltaTime) {}

                    virtual void initLuaValues(sol::table table, const char *componentName) {}

                public:
                    template<typename TObj, typename ...Constructors, typename ...Args>
                    component(const std::string &name, Args &&...args);

                    FLAT_ENGINE_API void engineOnAdd(fe::baseEntity *ent);
                    FLAT_ENGINE_API void engineOnRemove(fe::baseEntity *ent);
                    FLAT_ENGINE_API void engineUpdate(fe::baseEntity *ent);
                    FLAT_ENGINE_API void engineFixedUpdate(fe::baseEntity *ent, float deltaTime);

                    FLAT_ENGINE_API void engineInitLuaValues(sol::table table, const char *componentName);

            };
        template<typename TObj, typename ...Constructors, typename ...Args>
        component::component(const std::string &name, Args && ...args) :
            m_onAdd(nullptr),
            m_onRemove(nullptr),
            m_update(nullptr),
            m_fixedUpdate(nullptr)
            {
                // we assume that this class has already been defined in-engine
                fe::engine::get().getScriptManager().getUserTypeHandler().addCustomType<TObj, Constructors...>(name, sol::base_classes, sol::bases<component>(), std::forward<Args>(args)...);
            }
    }

