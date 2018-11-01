// tableHandler.hpp
// Handles all Lua tables
#pragma once
#include "fe/flatEngineExport.hpp"
#include <sol.hpp>
#include <string>

namespace fe
    {
        class tableHandler
            {
                private:
                    sol::state &m_state;

                public:
                    FLAT_ENGINE_API tableHandler(sol::state &state);
                    FLAT_ENGINE_API sol::table getTable(const char *path);

            };
    }
