#include "fe/subsystems/scripting/functionHander.hpp"

fe::functionHandler::functionHandler(sol::state &state) : m_state(state)
    {
    }

sol::protected_function fe::functionHandler::getFunction(const std::string &functionName)
    {
        return m_state[functionName];
    }

