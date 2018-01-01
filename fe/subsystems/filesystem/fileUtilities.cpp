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

void fe::getAllFilesInDirectory(const std::string &directory, std::vector<std::string> &fileReturn)
    {
        fe::getAllFilesInDirectory(directory.c_str(), fileReturn);
    }

void fe::getAllFilesInDirectory(const char *directory, std::vector<std::string> &fileReturn)
    {
        for (auto &file : std::experimental::filesystem::directory_iterator(directory))
            {
                auto path = file.path().filename().string();

                // if there is a dot in the name, then we can assume that it isnt a folder
                // if it is a folder, well that sucks for the user
                // dont be a jackass
                auto extension = (int)path.find_last_of('.');
                if (extension < 0)
                    {   
                        getAllFilesInDirectory((directory + ("//" + path) + "/").c_str(), fileReturn);
                    }
                else
                    {
                        fileReturn.push_back(directory + ("/" + path));
                    }
            }
    }

void fe::getAllFilesInDirectory(const std::string &directory, const std::vector<std::string> &&extension, std::vector<std::string> &fileReturn)
    {
        fe::getAllFilesInDirectory(directory.c_str(), std::move(extension), fileReturn);
    }

void fe::getAllFilesInDirectory(const char *directory, const std::vector<std::string> &&extensions, std::vector<std::string> &fileReturn)
    {
        for (auto &file : std::experimental::filesystem::directory_iterator(directory))
            {
                auto path = file.path().filename().string();

                // if there is a dot in the name, then we can assume that it isnt a folder
                // if it is a folder, well that sucks for the user
                // dont be a jackass
                int fileExtension = -1;
                for (auto &possibleExtension : extensions)
                    {
                        fileExtension = (int)path.find(possibleExtension);
                        if (fileExtension >= 0)
                            {
                                break;
                            }
                    }

                if (fileExtension < 0)
                    {   
                        getAllFilesInDirectory((directory + ("/" + path) + "/"), std::move(extensions), fileReturn);
                    }
                else
                    {
                        fileReturn.push_back(directory + ("/" + path));
                    }
            }
    }

void fe::getFilesInDirectory(const std::string &directory, std::vector<std::string> &fileReturn)
    {
        fe::getFilesInDirectory(directory.c_str(), fileReturn);
    }

void fe::getFilesInDirectory(const char *directory, std::vector<std::string> &fileReturn)
    {
        for (auto &file : std::experimental::filesystem::directory_iterator(directory))
            {
                auto path = file.path().filename().string();

                // if there is a dot in the name, then we can assume that it isnt a folder
                // if it is a folder, well that sucks for the user
                // dont be a jackass
                auto fileExtension = (int)path.find('.');
                if (fileExtension > 0)
                    {   
                        fileReturn.push_back(directory + ("/" + path));
                    }
            }
    }

void fe::getFilesInDirectory(const std::string &directory, const std::vector<std::string> &&extension, std::vector<std::string> &fileReturn)
    {
        fe::getFilesInDirectory(directory.c_str(), std::move(extension), fileReturn);
    }

void fe::getFilesInDirectory(const char *directory, const std::vector<std::string> &&extensions, std::vector<std::string> &fileReturn)
    {
        for (auto &file : std::experimental::filesystem::directory_iterator(directory))
            {
                auto path = file.path().filename().string();

                // if there is a dot in the name, then we can assume that it isnt a folder
                // if it is a folder, well that sucks for the user
                // dont be a jackass
                int fileExtension = -1;
                for (auto &possibleExtension : extensions)
                    {
                        fileExtension = (int)path.find(possibleExtension);
                        if (fileExtension >= 0)
                            {
                                break;
                            }
                    }
                if (fileExtension > 0)
                    {   
                        fileReturn.push_back(directory + ("/" + path));
                    }
            }
    }

std::string fe::getFileFromDirectory(const std::string &directory)
    {
        return fe::getFileFromDirectory(directory.c_str());
    }

std::string fe::getFileFromDirectory(const char *directory)
    {
        std::string file = "";
        for (unsigned int i = std::strlen(directory) - 1; i > 0; i--)
            {
                if (directory[i] == '\\' || directory[i] == '/')
                    {
                        break;
                    }
                else
                    {
                        file += directory[i];
                    }
            }
        std::reverse(file.begin(), file.end());
        return file;
    }

std::string fe::getFileExtension(const std::string &directory)
    {
        return fe::getFileExtension(directory.c_str());
    }

std::string fe::getFileExtension(const char *directory)
    {
        std::string extension = "";
        for (unsigned int i = std::strlen(directory) - 1; i > 0; i--)
            {
                if (directory[i] == '.')
                    {
                        extension += directory[i];
                        break;
                    }
                else
                    {
                        extension += directory[i];
                    }
            }
        std::reverse(extension.begin(), extension.end());
        return extension;
    }
