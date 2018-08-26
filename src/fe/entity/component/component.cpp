#include "fe/entity/component/component.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/subsystems/scripting/scriptManager.hpp"

void fe::component::engineOnAdd(fe::baseEntity *ent)
    {
        if (m_onAdd)
            {
                m_onAdd->call(this, static_cast<fe::scriptObject*>(ent));
            }
        onAdd(ent);
    }

void fe::component::engineOnRemove(fe::baseEntity *ent)
    {
        if (m_onRemove)
            {
                m_onRemove->call(this, static_cast<fe::scriptObject*>(ent));
            }
        onRemove(ent);
    }

void fe::component::engineUpdate(fe::baseEntity *ent)
    {
        if (m_update)
            {
                m_update->call(this, static_cast<fe::scriptObject*>(ent));
            }
        update(ent);
    }

void fe::component::engineFixedUpdate(fe::baseEntity *ent, float deltaTime)
    {
        if (m_fixedUpdate)
            {
                m_fixedUpdate->call(this, static_cast<fe::scriptObject*>(ent), deltaTime);
            }
        fixedUpdate(ent, deltaTime);
    }

void fe::component::engineInitLuaValues(sol::table table, const char *componentName)
    {
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
                m_fixedUpdate = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(componentName, "onRemove");
            }

        initLuaValues(table, componentName);
    }
