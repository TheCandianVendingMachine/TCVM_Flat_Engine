// fileUtilities.hpp
// include std::filesystem and has a custom function to get the last time a file was modified
#pragma once
#include <experimental/filesystem>
#include <fstream>

#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

namespace fe
    {
        // Get the last time since epoch when a file was modified
        // Returns -1 if file does not exist
        FLAT_ENGINE_API __time64_t lastTimeModified(const char *filename);
    }