// logger.hpp
// a logger to log events that happen in game. It is a singleton so any file in-game can access it
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineInclude.hpp"
#include "../subsystems/filesystem/fileUtilities.hpp"
#include "../time/clock.hpp"

#include <sstream>
#include <iostream>
#include <utility>

namespace fe
    {
        class FLAT_ENGINE_API logger
            {
                private:
                    std::ofstream m_output;
                    static logger *m_instance;

                public:
                    // start the logger at the directory
                    void startUp(const char *directory);
                    // close the ofstream and make sure everything logged
                    void shutDown();

                    static logger &get();

                    // print a log to file
                    template<typename ...Args>
                    void log(Args &&...args);

                    // print a log to console and file
                    template<typename ...Args>
                    void logToConsole(Args &&...args);

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

#ifndef FE_CONSOLE_LOG
    // Log values to the console
    #define FE_CONSOLE_LOG(...) fe::logger::get().logToConsole(__VA_ARGS__)
#endif