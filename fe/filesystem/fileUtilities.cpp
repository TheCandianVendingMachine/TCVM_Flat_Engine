#include "fileUtilities.hpp"
#include <sys/types.h>
#include <sys/stat.h>

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