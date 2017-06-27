// threadJob.hpp
// A abstract struct that allows the user to create custom jobs
#pragma once

namespace fe
    {
        struct threadJob
            {   
                // Runs a function that the thread executes
                // If this returns false, the thread will run again
                virtual bool execute() = 0;
                bool m_done;
                bool m_active;
            };
    }