// countdown.hpp
// a timer which will start its count at a set time, and when it reaches zero the user must check to see if its zero
#pragma once
#include "../flatEngineExport.hpp"
#include "clock.hpp"
#include "time.hpp"

namespace fe 
    {
        class countdown
            {
                private:
                    fe::clock m_timer;
                    fe::time m_length;

                    bool m_started;

                public:
                    FLAT_ENGINE_API countdown();

                    FLAT_ENGINE_API void pause(bool pause);

                    // starts the timer for the length provided
                    FLAT_ENGINE_API void start(fe::time length, fe::time startTime = fe::time());

                    // Resets the internal state of the countdown
                    FLAT_ENGINE_API void reset();

                    // returns true if the countdown has completed.
                    FLAT_ENGINE_API bool isDone(fe::time currentTime) const;

                    // returns true if the countdown has passed 0, even if it hasnt been started yet.
                    FLAT_ENGINE_API bool hasCompleted(fe::time currentTime) const;

                    // returns how long the timer has to completion.
                    FLAT_ENGINE_API fe::time timeUntilCompletion(fe::time currentTime) const;

                    // Returns true if the countdown has started
                    FLAT_ENGINE_API bool hasStarted() const;

                    // Returns time of the internal clock.
                    FLAT_ENGINE_API fe::time getInternalClockTime() const;

            };
    }