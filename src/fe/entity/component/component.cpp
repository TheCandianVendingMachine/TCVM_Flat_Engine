#include "fe/entity/component/component.hpp"

bool fe::componentBase::isInitialized() const
    {
        return m_initialized;
    }

void fe::componentBase::engineInitLuaValues(sol::table table, const char *componentName)
    {
        m_initialized = true;
        if (table["onAdd"].get_type() == sol::type::function)
            {
                m_onAdd = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(componentName, "onAdd");
            }

        if (table["onRemove"].get_type() == sol::type::function)
            {
                m_onRemove = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(componentName, "onRemove");
            }

        if (table["update"].get_type() == sol::type::function)
            {
                m_update = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(componentName, "update");
            }

        if (table["fixedUpdate"].get_type() == sol::type::function)
            {
                m_fixedUpdate = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(componentName, "fixedUpdate");
            }

        initLuaValues(table, componentName);
    }
