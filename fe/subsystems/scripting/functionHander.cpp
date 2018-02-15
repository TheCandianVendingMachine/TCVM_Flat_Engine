#include "functionHander.hpp"

fe::functionHandler::functionHandler(sol::state &state) : m_state(state)
    {
    }

void fe::functionHandler::registerLuaFunctions()
    {
        for (auto &globalObjects : m_state.globals())
            {
                if (globalObjects.second.get_type() == sol::type::function)
                    {
                        std::string first = globalObjects.first.as<std::string>();
                        if (m_registeredFunctions.find(FE_STR(first.c_str())) == m_registeredFunctions.end())
                            {
                                m_registeredFunctions[FE_STR(first.c_str())] = globalObjects.second.as<sol::protected_function>();
                            }
                    }
            }
    }

sol::protected_function &fe::functionHandler::getFunction(const std::string &functionName)
    {
        return getFunction(FE_STR(functionName.c_str()));
    }

sol::protected_function &fe::functionHandler::getFunction(fe::str functionName)
    {
        return m_registeredFunctions[functionName];
    }
