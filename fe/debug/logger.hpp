// logger.hpp
// a logger to log events that happen in game. It is a singleton so any file in-game can access it
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../time/clock.hpp"

#include <sstream>
#include <iostream>
#include <utility>
#include <fstream>
#include <assert.h>

namespace fe
    {
        class logger
            {
                private:
                    std::ofstream m_output;
                    static logger *m_instance;

                public:
                    // start the logger at the directory
                    FLAT_ENGINE_API void startUp(const char *directory);
                    // close the ofstream and make sure everything logged
                    FLAT_ENGINE_API void shutDown();

                    FLAT_ENGINE_API static logger &get();

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
                m_output << fe::clock::getFormattedTime("%b %Y %H:%M:%S %p") << " - ";

                using expanded = int[];
                expanded { 0, (m_output << std::forward<Args>(args) << " ", 0)... };

                m_output << std::endl;
                logToConsole(args...);
            }

        template<typename ...Args>
        void logger::logToConsole(Args &&...args)
            {
            #if _DEBUG
                std::cout << "<CNSL> " << fe::clock::getFormattedTime("%b %Y %H:%M:%S %p") << " - ";

                using expanded = int[];
                expanded { 0, (std::cout << std::forward<Args>(args) << " ", 0)... };

                std::cout << std::endl;
            #endif
            }
    }

#ifndef FE_LOG
    // Log values to the output file
    #define FE_LOG(...) fe::logger::get().log(__VA_ARGS__)
#endif

#ifndef FE_LOG_ERROR
    // Log values to the output file with an error prefix
    #define FE_LOG_ERROR(...) fe::logger::get().log("Error: ", __VA_ARGS__)
#endif

#ifndef FE_LOG_WARNING
    // Log values to the output file with a warning prefix
    #define FE_LOG_WARNING(...) fe::logger::get().log("Warning: ", __VA_ARGS__)
#endif

#ifndef FE_CONSOLE_LOG
    // Log values to the console
    #define FE_CONSOLE_LOG(...) fe::logger::get().logToConsole(__VA_ARGS__)
#endif