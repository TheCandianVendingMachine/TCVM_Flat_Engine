#include "fe/subsystems/scripting/userTypeHandler.hpp"

fe::userTypeHandler::userTypeHandler(sol::state &state) :
    m_luaState(state)
    {
    }
