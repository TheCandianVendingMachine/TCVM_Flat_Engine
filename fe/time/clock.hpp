// clock.hpp
// a high-res clock to be able to get the current time in seconds, milliseconds, microseconds
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExports.hpp"
#include "time.hpp"

namespace fe
    {
        class FLAT_ENGINE_API clock
            {
                private:
                    fe::time m_startTime;
                    fe::time m_stopTime;
                    fe::time m_offsetTime; // the time between stops gets added to this

                    bool m_stopped;

                    fe::time m_getCurrentTime();

                public:
                    clock();

                    // Restarts the timer to zero
                    void restart();

                    // Stop or start the clock based on the value
                    // Arguments:
                    // True -> Stop the clock
                    // False-> Start the clock
                    void stop(bool value);

                    // returns the elapsed time as a fe::Time value
                    fe::time getTime();

                    // returns fe::time since the epoch of the system
                    static fe::time getTimeSinceEpoch();

                    // returns the current time formatted into a char array
                    // Arguments:
                    // Format -> The format which the string will be returned. Defined at http://en.cppreference.com/w/cpp/io/manip/put_time
                    static const char* getFormattedTime(const char *format);

            };
    }