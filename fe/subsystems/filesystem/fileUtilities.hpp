// fileUtilities.hpp
// Utilities to help with navigating directories
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <vector>
#include <string>

namespace fe
    {
        // Get the last time since epoch when a file was modified
        // Returns -1 if file does not exist
        FLAT_ENGINE_API __time64_t lastTimeModified(const char *filename);

        // Returns all the files in the specified directory and subdirectory
        FLAT_ENGINE_API std::vector<std::string> getAllFilesInDirectory(const char *directory);
        // Returns all the files in the specified directory and subdirectory with the specified extension
        FLAT_ENGINE_API std::vector<std::string> getAllFilesInDirectory(const char *directory, const char *extension);

        // Returns all the files in the specified directory
        FLAT_ENGINE_API std::vector<std::string> getFilesInDirectory(const char *directory);
        // Returns all files in the specifiecd directory with the specified extension
        FLAT_ENGINE_API std::vector<std::string> getFilesInDirectory(const char *directory, const char *extension);
    }