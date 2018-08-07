#include "fe/subsystems/scripting/scriptManager.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/subsystems/filesystem/fileUtilities.hpp"

fe::scriptManager::scriptManager() :
    m_functionHandler(m_luaState),
    m_userTypeHandler(m_luaState),
    m_enumHandler(m_luaState)
    {
    }

void fe::scriptManager::startUp()
    {
        m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);
    }

void fe::scriptManager::shutDown()
    {
        m_functionHandler.shutDown();
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

        if (std::find(m_luaFiles.begin(), m_luaFiles.end(), file) == m_luaFiles.end())
            {
                m_luaFiles.push_back(file);
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

fe::enumHandler &fe::scriptManager::getEnumHandler()
    {
        return m_enumHandler;
    }

void fe::scriptManager::update()
    {
        bool needLuaUpdate = false;
        for (auto &path : m_luaFiles)
            {
                if (fe::lastTimeModified(path.c_str()) > 10)
                    {
                    needLuaUpdate = true;
                       runFile(path);
                    }
            }

        if (needLuaUpdate)
            {
                m_functionHandler.reloadAllLuaFunctions();
            }
    }

sol::table fe::scriptManager::getTable(const char *id)
    {
        try 
            {
                return m_luaState[id];
            }
        catch (std::exception &e)
            {
                FE_LOG_ERROR(e.what());
            }
    }

sol::state &fe::scriptManager::getLuaState()
    {
        return m_luaState;
    }
