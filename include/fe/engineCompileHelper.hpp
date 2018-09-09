// engineCompileHelper.hpp
// Helper functions to avoid the compiler running out of heap space
#pragma once
#include "fe/flatEngineExport.hpp"

namespace fe
    {
        class scriptManager;

        FLAT_ENGINE_API void compileLuaKeybinds(fe::scriptManager *manager);
    }
