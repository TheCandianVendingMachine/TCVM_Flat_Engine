// logger.hpp
// a logger to log events that happen in game. It is a singleton so any file in-game can access it
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../subsystems/threading/threadJob.hpp"
#include "../time/clock.hpp"

#include <sstream>
#include <iostream>
#include <utility>
#include <fstream>
#include <mutex>
#include <assert.h>

namespace fe
    {
        class logger
            {
                private:
                    std::ofstream m_output;
                    static logger *m_instance;

                    std::stringstream m_fileBuffer[2];
                    std::stringstream m_consoleBuffer[2];

                    std::stringstream *m_currentFileBuffer; // Current Buffer Being Written To
                    std::stringstream *m_nextFileBuffer;    // Current Buffer Being Printed

                    std::stringstream *m_currentConsoleBuffer; // Current Buffer Being Written To
                    std::stringstream *m_nextConsoleBuffer;    // Current Buffer Being Printed

                    fe::threadFunction m_printJob;

                    std::mutex m_consoleBufferLock;
                    std::mutex m_fileBufferLock;

                    bool m_running;
                    
                    FLAT_ENGINE_API std::stringstream &getCurrentFileBuffer();
                    FLAT_ENGINE_API std::stringstream &getCurrentConsoleBuffer();

                    // Threaded functions that do as they say on the tin
                    FLAT_ENGINE_API bool printLogs();

                public:
                    FLAT_ENGINE_API logger();

                    // start the logger at the directory
                    FLAT_ENGINE_API void startUp(const char *directory);
                    // close the ofstream and make sure everything logged
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API static logger &get();

                    FLAT_ENGINE_API void swapFileBuffer();
                    FLAT_ENGINE_API void swapConsoleBuffer();

                    // print a log to file
                    template<typename ...Args>
                    FLAT_ENGINE_API void log(Args &&...args);

                    // print a log to console and file
                    template<typename ...Args>
                    FLAT_ENGINE_API void logToConsole(Args &&...args);

                    FLAT_ENGINE_API virtual ~logger() {}

            };

        template<typename ...Args>
        void logger::log(Args &&...args)
            {
                getCurrentFileBuffer() << fe::clock::getFormattedTime("%b %Y %H:%M:%S %p") << " - ";

                using expanded = int[];
                expanded { 0, (getCurrentFileBuffer() << std::forward<Args>(args) << " ", 0)... };

                getCurrentFileBuffer() << "\n";
                logToConsole(args...);
            }

        template<typename ...Args>
        void logger::logToConsole(Args &&...args)
            {
            #if _DEBUG
                getCurrentConsoleBuffer() << "<CNSL> " << fe::clock::getFormattedTime("%b %Y %H:%M:%S %p") << " - ";

                using expanded = int[];
                expanded { 0, (getCurrentConsoleBuffer() << std::forward<Args>(args) << " ", 0)... };

                getCurrentConsoleBuffer() << "\n";
            #endif
            }
    }

#ifdef _DEBUG
    // Log values to the output file
    #define FE_LOG(...) fe::logger::get().log(__VA_ARGS__, "    File:", __FILE__, "Line:", __LINE__)

    // Log values to the output file with an error prefix
    #define FE_LOG_ERROR(...) fe::logger::get().log("ERROR: ", __VA_ARGS__, "    File:", __FILE__, "Line:", __LINE__)
    // Log values to the output file with a warning prefix
    #define FE_LOG_WARNING(...) fe::logger::get().log("WARNING: ", __VA_ARGS__, "    File:", __FILE__, "Line:", __LINE__)
    // Log values taht will only happe in debug mode
    #define FE_LOG_DEBUG(...) fe::logger::get().log("DEBUG: ", __VA_ARGS__, "    File:", __FILE__, "Line:", __LINE__)
    // Log values to the console
    #define FE_CONSOLE_LOG(...) fe::logger::get().logToConsole(__VA_ARGS__, "    File:", __FILE__, "Line:", __LINE__)
#else
    // Log values to the output file
    #define FE_LOG(...) fe::logger::get().log(__VA_ARGS__)

    // Log values to the output file with an error prefix
    #define FE_LOG_ERROR(...) fe::logger::get().log("Error: ", __VA_ARGS__)
    // Log values to the output file with a warning prefix
    #define FE_LOG_WARNING(...) fe::logger::get().log("Warning: ", __VA_ARGS__)

    #define FE_LOG_DEBUG(...) ;

    // Log values to the console
    #define FE_CONSOLE_LOG(...) fe::logger::get().logToConsole(__VA_ARGS__)
#endif