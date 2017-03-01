// countdown.hpp
// a timer which will start its count at a set time, and when it reaches zero the user must check to see if its zero
#pragma once
#define FLAT_ENGINE_EXPORT
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

                public:
                    // starts the timer for the length provided
                    FLAT_ENGINE_API void start(fe::time length);
                    
                    // returns true if the countdown has completed
                    FLAT_ENGINE_API bool isDone();

                    // returns how long the timer has to completion
                    FLAT_ENGINE_API fe::time timeUntilCompletion();

            };
    }