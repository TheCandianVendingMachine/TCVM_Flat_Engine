// countdown.hpp
// a timer which will start its count at a set time, and when it reaches zero the user must check to see if its zero
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineInclude.hpp"
#include "clock.hpp"
#include "time.hpp"

namespace fe 
    {
        class FLAT_ENGINE_API countdown
            {
                private:
                    fe::clock m_timer;
                    fe::time m_length;

                public:
                    // starts the timer for the length provided
                    void start(fe::time length);
                    
                    // returns true if the countdown has completed
                    bool isDone();

                    // returns how long the timer has to completion
                    fe::time timeUntilCompletion();

            };
    }