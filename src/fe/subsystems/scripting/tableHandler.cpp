#include "fe/subsystems/scripting/tableHandler.hpp"
#include "fe/subsystems/scripting/scriptHelpers.hpp"

fe::tableHandler::tableHandler(sol::state &state) : m_state(state)
    {
    }

sol::table fe::tableHandler::getTable(const char *path)
    {
        return fe::imp::getTableFromPath(path, m_state);
    }
