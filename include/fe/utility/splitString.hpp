// splitString.hpp
// splits a string into a vector
#pragma once
#include "fe/flatEngineExport.hpp"

#include <vector>
#include <string>

namespace fe
    {
        FLAT_ENGINE_API std::vector<std::string> splitString(const char *str, char split);
        FLAT_ENGINE_API void splitString(const char *str, char split, std::vector<std::string> &&out);
    }
