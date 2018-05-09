#include "fe/subsystems/scripting/luaFunctionReference.hpp"
#include "fe/subsystems/scripting/functionHander.hpp"

fe::luaFunctionReference::luaFunctionReference(sol::protected_function function, std::string luaPath, const std::string &functionName, functionHandler &handler) :
    m_function(function),
    m_functionName(functionName),
    m_functionHandler(handler),
    m_luaPathName(luaPath)
    {
    }

void fe::luaFunctionReference::reload()
    {
        m_functionHandler.reloadLuaFunction(*this);
    }

bool fe::luaFunctionReference::operator==(const luaFunctionReference &rhs)
    {
        return  m_function == rhs.m_function &&
                m_functionName == rhs.m_functionName &&
                m_luaPathName == rhs.m_luaPathName;
    }
