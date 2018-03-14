// profiler.hpp
// a custom profiler to store the name, and the time it takes for code to execute
#pragma once
#include "profilerLogger.hpp"
#include "../time/time.hpp"
#include "../time/clock.hpp"
#include "../subsystems/memory/memoryManager.hpp"
#include "../objectManagement/str.hpp"
#include <iostream>
#include <cstring>

namespace fe
    {
	    struct profiler
	        {
		        fe::time m_startTime;
		        fe::time m_endTime;
		
                char m_nameStr[512];
                bool m_profile;
		        profiler( const char *name) 
		            {
                    #if FE_PROFILE_RELEASE || _DEBUG
                        m_profile = false;
                        strcpy(m_nameStr, name);
                        if (fe::profilerLogger::get().wantProfile(FE_STR(name)))
                            {
                                fe::profilerLogger::get().startProfile(name);
			                    m_startTime = fe::clock::getTimeSinceEpoch();
                                m_profile = true;
                            }

                    #endif
		            }
		
		        ~profiler()
		            {
                    #if FE_PROFILE_RELEASE || _DEBUG
                        if (m_profile)
                            {
			                    m_endTime = fe::clock::getTimeSinceEpoch();
			                    fe::time runtime = m_endTime - m_startTime;
                                fe::profilerLogger::get().endProfile(m_nameStr, runtime);
                            }
                    #endif
		            }
	        };
    }

#define FE_PROFILE(group, name) { fe::profiler t(group ## "_" name);
#define FE_END_PROFILE }

#if FE_PROFILE_ENGINE
    #define FE_ENGINE_PROFILE(group, name) { fe::profiler t(group ## "_" name);
#else
    #define FE_ENGINE_PROFILE(group, name) {
#endif
