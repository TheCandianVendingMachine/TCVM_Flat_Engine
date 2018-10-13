// scriptManager.hpp
// Manages the scripting subsystem. Handles addition of script values, getting of script values, running scripts and loading scripts
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/debug/logger.hpp"
#include "functionHander.hpp"
#include "userTypeHandler.hpp"
#include "enumHandler.hpp"
#include <sol.hpp>
#include <string>
#include <vector>

namespace fe
    {
        class scriptManager
            {
                private:
                    sol::state m_luaState;

                    fe::functionHandler m_functionHandler;
                    fe::userTypeHandler m_userTypeHandler;
                    fe::enumHandler m_enumHandler;

                    std::vector<std::string> m_luaFiles;

                public:
                    FLAT_ENGINE_API scriptManager();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API void runLua(const std::string &lua);
                    FLAT_ENGINE_API void runFile(const std::string &file);

                    FLAT_ENGINE_API fe::functionHandler &getFunctionHandler();
                    FLAT_ENGINE_API fe::userTypeHandler &getUserTypeHandler();
                    FLAT_ENGINE_API fe::enumHandler &getEnumHandler();

                    FLAT_ENGINE_API void update();

                    inline decltype(auto) getValueFromTable(sol::table &table, const char *id);
                    
                    FLAT_ENGINE_API sol::table getTable(const char *id);
                    FLAT_ENGINE_API sol::state &getLuaState();

            };

        decltype(auto) fe::scriptManager::getValueFromTable(sol::table &table, const char *id)
            {
                try
                    {
                        return table[id];
                    }
                catch (std::exception &e)
                    {
                        FE_LOG_ERROR(e.what());
                    }
            }
    }