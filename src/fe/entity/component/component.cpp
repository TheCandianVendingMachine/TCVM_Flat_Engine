#include "fe/entity/component/component.hpp"

fe::componentBase::componentBase(const char *str) :
    m_id(FE_STR(str))
    {
    }

bool fe::componentBase::isInitialized() const
    {
        return m_initialized;
    }

void fe::componentBase::engineInitLuaValues(sol::table table, const char *componentPath, const char *entName)
    {
        m_initialized = true;
        if (table["onAdd"].get_type() == sol::type::function)
            {
                m_onAdd = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(componentPath, "onAdd");
            }

        if (table["onRemove"].get_type() == sol::type::function)
            {
                m_onRemove = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(componentPath, "onRemove");
            }

        if (table["update"].get_type() == sol::type::function)
            {
                m_update = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(componentPath, "update");
            }

        if (table["fixedUpdate"].get_type() == sol::type::function)
            {
                m_fixedUpdate = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(componentPath, "fixedUpdate");
            }

        initLuaValues(table, componentPath, entName);
    }

fe::scriptObject *fe::componentBase::getOwner()
    {
        return m_owner;
    }

fe::str fe::componentBase::getID() const
    {
        return m_id;
    }
