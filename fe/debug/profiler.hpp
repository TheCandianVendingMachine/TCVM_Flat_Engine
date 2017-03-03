// profiler.hpp
// a custom profiler to store the name, and the time it takes for code to execute
#pragma once
#include "../time/time.hpp"
#include "../time/clock.hpp"
#include "../subsystems/memory/memoryManager.hpp"
#include <iostream>
#include <cstring>

namespace fe
    {
	    struct profiler
	        {
		        fe::time m_startTime;
		        fe::time m_endTime;
		
		        char *m_name;
		
		        profiler(const char *name) 
		            {
                        void *alloc = FE_ALLOC_STACK("Profiler", sizeof(name));
                        m_name = static_cast<char*>(alloc);
			            strcpy(m_name, name);
			            m_startTime = fe::clock::getTimeSinceEpoch();
		            }
		
		        ~profiler()
		            {
			            m_endTime = fe::clock::getTimeSinceEpoch();
			            fe::time runtime = m_endTime - m_startTime;

                        std::cout << "\n" << m_name << "\nMicroseconds: " << runtime.asMicroseconds() << "\nMilliseconds: " << runtime.asMilliseconds() << "\nSeconds: " << runtime.asSeconds() << "\n\n";
		            }
	        };
    }

#define PROFILE(name) { fe::profiler t(name);
#define END_PROFILE }