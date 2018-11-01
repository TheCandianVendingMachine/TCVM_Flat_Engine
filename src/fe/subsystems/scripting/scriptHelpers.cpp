#include "fe/subsystems/scripting/scriptHelpers.hpp"
#include "fe/utility/splitString.hpp"
#include "fe/debug/logger.hpp"
#include "fe/engine.hpp"

sol::table fe::imp::getTableFromPath(const std::string &path, sol::state &state)
    {
        std::vector<std::string> luaPathVector;
        fe::splitString(path.c_str(), '/', std::move(luaPathVector));
        std::reverse(luaPathVector.begin(), luaPathVector.end());

        sol::table foundTable = state.globals();
        while (!luaPathVector.empty())
            {
                try 
                    {
                        foundTable = foundTable[luaPathVector.back()];
                    }
                catch (std::exception &e)
                    {
                        FE_LOG_ERROR("Is the Lua path correct? [", path, "]");
                        fe::engine::crash(e.what());
                    }
                luaPathVector.pop_back();
            }

        return foundTable;
    }
