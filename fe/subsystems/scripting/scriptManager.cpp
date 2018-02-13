#include "scriptManager.hpp"

void fe::scriptManager::startUp()
    {
        m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);
    }

void fe::scriptManager::shutDown()
    {
    }
