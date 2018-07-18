#include "fe/subsystems/entitySpawner/prefabObject.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/scripting/scriptManager.hpp"

fe::prefabObject &fe::prefabObject::addUserFunction(const char *luaName, const char *funcName)
    {
        m_userFunctions[FE_STR(funcName)] = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction(luaName, funcName);
        return *this;
    }
