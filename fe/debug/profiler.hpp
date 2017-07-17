// profiler.hpp
// a custom profiler to store the name, and the time it takes for code to execute
#pragma once
#include "profilerLogger.hpp"
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
		
                char m_groupStr[512];
                char m_nameStr[512];
		
		        profiler(const char *group, const char *name) 
		            {
                    #ifdef FE_PROFILE_ENGINE
                        strcpy(m_nameStr, name);
                        strcpy(m_groupStr, group);
			            m_startTime = fe::clock::getTimeSinceEpoch();
                    #endif
		            }
		
		        ~profiler()
		            {
                    #if FE_PROFILE_ENGINE
			            m_endTime = fe::clock::getTimeSinceEpoch();
			            fe::time runtime = m_endTime - m_startTime;
                        fe::profilerLogger::get().add(m_groupStr, m_nameStr, runtime);
                    #endif
		            }
	        };
    }

#define FE_PROFILE(group, name) { fe::profiler t(group, name);
#define FE_END_PROFILE }