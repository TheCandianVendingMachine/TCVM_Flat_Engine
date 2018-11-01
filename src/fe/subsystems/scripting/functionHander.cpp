#include "fe/subsystems/scripting/functionHander.hpp"
#include "fe/engine.hpp"


fe::functionHandler::functionHandler(sol::state &state) : m_state(state)
    {
    }

void fe::functionHandler::reloadAllLuaFunctions()
    {
        for (auto &func : m_registeredFunctions)
            {
                reloadLuaFunction(*func);
            }
    }

void fe::functionHandler::reloadLuaFunction(fe::luaFunctionReference &function)
    {
        function.m_function = fe::imp::getTableFromPath(function.m_luaPathName, m_state)[function.m_functionName];
    }

fe::luaFunctionReference &fe::functionHandler::getLuaFunction(const std::string &functionName)
    {
        return getLuaFunction("", functionName);
    }

fe::luaFunctionReference &fe::functionHandler::getLuaFunction(const std::string &luaPathName, const std::string &functionName)
    {
        auto func = std::find_if(m_registeredFunctions.begin(), m_registeredFunctions.end(), [&functionName, &luaPathName](const fe::luaFunctionReference *func) {
            return func->m_functionName == functionName && func->m_luaPathName == luaPathName;
        });

        if (func == m_registeredFunctions.end())
            {
                m_registeredFunctions.emplace_back(new fe::luaFunctionReference(fe::imp::getTableFromPath(luaPathName, m_state)[functionName], luaPathName, functionName, *this));
            }

        reloadLuaFunction(*m_registeredFunctions.back());
        return *m_registeredFunctions.back();
    }

void fe::functionHandler::shutDown()
    {
        for (auto &element : m_registeredFunctions)
            {
                delete element;
                element = nullptr;
            }
    }

