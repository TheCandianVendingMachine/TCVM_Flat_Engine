// scriptManager.hpp
// Manages the scripting subsystem. Handles addition of script values, getting of script values, running scripts and loading scripts
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <sol.hpp>

namespace fe
    {
        class scriptManager
            {
                private:
                    sol::state m_luaState;

                public:
                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();

            };
    }