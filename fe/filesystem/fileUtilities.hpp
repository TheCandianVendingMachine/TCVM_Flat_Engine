// fileUtilities.hpp
// include std::filesystem and has a custom function to get the last time a file was modified
#pragma once
#include <experimental/filesystem>
#include <fstream>

#define FLAT_ENGINE_EXPORTS
#include "../flatEngineExports.hpp"

namespace fe
    {
        long long FLAT_ENGINE_API lastTimeModified(const char *filename);
    }