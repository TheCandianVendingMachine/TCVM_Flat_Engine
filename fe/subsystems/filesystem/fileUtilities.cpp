#include "fileUtilities.hpp"
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#ifndef _WIN32
    #include <unistd.h>
#else
    #define stat _stat
#endif // !WIN32

__time64_t fe::lastTimeModified(const char *filename)
    {
        struct stat result;
        if (stat(filename, &result) == 0)
            {
                return result.st_mtime;
            }

        return -1;
    }

std::vector<std::string> fe::getAllFilesInDirectory(const char *directory)
    {
        std::vector<std::string> returnVector;
        for (auto &file : std::experimental::filesystem::directory_iterator(directory))
            {
                auto path = file.path().filename().string();

                // if there is a dot in the name, then we can assume that it isnt a folder
                // if it is a folder, well that sucks for the user
                // dont be a jackass
                auto extension = (int)path.find_last_of('.');
                if (extension < 0)
                    {   
                        auto ret = getAllFilesInDirectory((directory + path + "/").c_str());
                        returnVector.insert(returnVector.end(), ret.begin(), ret.end());
                    }
                else
                    {
                        returnVector.push_back(directory + path);
                    }
            }

        return returnVector;
    }

std::vector<std::string> fe::getAllFilesInDirectory(const char *directory, const char *extension)
    {
        std::vector<std::string> returnVector;
        for (auto &file : std::experimental::filesystem::directory_iterator(directory))
            {
                auto path = file.path().filename().string();

                // if there is a dot in the name, then we can assume that it isnt a folder
                // if it is a folder, well that sucks for the user
                // dont be a jackass
                auto fileExtension = (int)path.find(extension);
                if (fileExtension < 0)
                    {   
                        auto ret = getAllFilesInDirectory((directory + path + "/").c_str(), extension);
                        returnVector.insert(returnVector.end(), ret.begin(), ret.end());
                    }
                else
                    {
                        returnVector.push_back(directory + path);
                    }
            }

        return returnVector;
    }

std::vector<std::string> fe::getFilesInDirectory(const char *directory)
    {
        std::vector<std::string> returnVector;
        for (auto &file : std::experimental::filesystem::directory_iterator(directory))
            {
                auto path = file.path().filename().string();

                // if there is a dot in the name, then we can assume that it isnt a folder
                // if it is a folder, well that sucks for the user
                // dont be a jackass
                auto fileExtension = (int)path.find('.');
                if (fileExtension > 0)
                    {   
                        returnVector.push_back(directory + path);
                    }
            }

        return returnVector;
    }

std::vector<std::string> fe::getFilesInDirectory(const char *directory, const char *extension)
    {
        std::vector<std::string> returnVector;
        for (auto &file : std::experimental::filesystem::directory_iterator(directory))
            {
                auto path = file.path().filename().string();

                // if there is a dot in the name, then we can assume that it isnt a folder
                // if it is a folder, well that sucks for the user
                // dont be a jackass
                auto fileExtension = (int)path.find(extension);
                if (fileExtension > 0)
                    {   
                        returnVector.push_back(directory + path);
                    }
            }

        return returnVector;
    }
