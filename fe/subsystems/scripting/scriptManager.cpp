#include "scriptManager.hpp"
#include "../../math/Vector2.hpp"

fe::scriptManager::scriptManager() :
    m_functionHandler(m_luaState),
    m_userTypeHandler(m_luaState)
    {
    }

void fe::scriptManager::startUp()
    {
        m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);
    }

void fe::scriptManager::shutDown()
    {
    }

void fe::scriptManager::runLua(const std::string &lua)
    {
        m_luaState.script(lua);
    }

void fe::scriptManager::runFile(const std::string &file)
    {
        try 
            {
                m_luaState.script_file(file);
            }
        catch (std::exception &e)
            {
                FE_LOG_ERROR(e.what());
            }
    }

fe::functionHandler &fe::scriptManager::getFunctionHandler()
    {
        return m_functionHandler;
    }

fe::userTypeHandler &fe::scriptManager::getUserTypeHandler()
    {
        return m_userTypeHandler;
    }

sol::state &fe::scriptManager::getLuaState()
    {
        return m_luaState;
    }
