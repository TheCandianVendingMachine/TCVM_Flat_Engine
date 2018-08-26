// fileUtilities.hpp
// Utilities to help with navigating directories
#pragma once
#include "../../flatEngineExport.hpp"
#include <vector>
#include <string>

namespace fe
    {
        // Get the last time since epoch when a file was modified
        // Returns -1 if file does not exist
        FLAT_ENGINE_API __time64_t lastTimeModified(const char *filename);

        // Returns all the files in the specified directory and subdirectory
        FLAT_ENGINE_API void getAllFilesInDirectory(const std::string &directory, std::vector<std::string> &fileReturn);
        FLAT_ENGINE_API void getAllFilesInDirectory(const char *directory, std::vector<std::string> &fileReturn);
        // Returns all the files in the specified directory and subdirectory with the specified extension
        FLAT_ENGINE_API void getAllFilesInDirectory(const std::string &directory, const std::vector<std::string> &&extension, std::vector<std::string> &fileReturn);
        FLAT_ENGINE_API void getAllFilesInDirectory(const char *directory, const std::vector<std::string> &&extension, std::vector<std::string> &fileReturn);

        // Returns all the files in the specified directory
        FLAT_ENGINE_API void getFilesInDirectory(const std::string &directory, std::vector<std::string> &fileReturn);
        FLAT_ENGINE_API void getFilesInDirectory(const char *directory, std::vector<std::string> &fileReturn);
        // Returns all files in the specifiecd directory with the specified extension
        FLAT_ENGINE_API void getFilesInDirectory(const std::string &directory, const std::vector<std::string> &&extension, std::vector<std::string> &fileReturn);
        FLAT_ENGINE_API void getFilesInDirectory(const char *directory, const std::vector<std::string> &&extension, std::vector<std::string> &fileReturn);

        // Returns the directory name from a directory string
        FLAT_ENGINE_API std::string getFileFromDirectory(const std::string &directory);
        FLAT_ENGINE_API std::string getFileFromDirectory(const char *directory);

        // Returns the extension for the file
        FLAT_ENGINE_API std::string getFileExtension(const std::string &directory);
        FLAT_ENGINE_API std::string getFileExtension(const char *directory);
    }