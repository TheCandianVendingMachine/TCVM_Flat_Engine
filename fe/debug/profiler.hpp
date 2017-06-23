// profiler.hpp
// a custom profiler to store the name, and the time it takes for code to execute
#pragma once
#include "../time/time.hpp"
#include "../time/clock.hpp"
#include "../subsystems/memory/memoryManager.hpp"
#include "../objectManagement/guid.hpp"
#include <iostream>
#include <cstring>

namespace fe
    {
	    struct profiler
	        {
		        fe::time m_startTime;
		        fe::time m_endTime;
		
		        fe::guid m_name;
                char m_nameStr[512];
		
		        profiler(const char *name) 
		            {
                    #ifdef FE_PROFILE_ENGINE
                        strcpy(m_nameStr, name);
                        m_name = FE_STR(name);
			            m_startTime = fe::clock::getTimeSinceEpoch();
                    #endif
		            }
		
		        ~profiler()
		            {
                    #ifdef FE_PROFILE_ENGINE
			            m_endTime = fe::clock::getTimeSinceEpoch();
			            fe::time runtime = m_endTime - m_startTime;

                        #ifdef FE_PROFILE_PRINT_ZEROS
                            std::cout << "\n" << m_nameStr <<
                                "\nMicroseconds: " << runtime.asMicroseconds() <<
                                "\nMilliseconds: " << runtime.asMilliseconds() <<
                                "\nSeconds: " << runtime.asSeconds() << "\n\n";
                        #else
                            if (runtime.asMicroseconds() != 0)
                                {
                                    std::cout << "\n" << m_nameStr <<
                                        "\nMicroseconds: " << runtime.asMicroseconds() <<
                                        "\nMilliseconds: " << runtime.asMilliseconds() <<
                                        "\nSeconds: " << runtime.asSeconds() << "\n\n";
                                }
                        #endif
                    #endif
		            }
	        };
    }

#define FE_PROFILE(name) { fe::profiler t(name);
#define FE_END_PROFILE }