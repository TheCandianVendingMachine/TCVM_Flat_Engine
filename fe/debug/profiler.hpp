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
		
                char m_groupStr[512];
                char m_nameStr[512];
                bool m_profile;
                bool m_profileToLogger;
		        profiler(const char *group, const char *name, bool profileLogger) 
		            {
                    #if FE_PROFILE_RELEASE || _DEBUG
                        m_profile = false;
                        m_profileToLogger = profileLogger;
                        if (m_profileToLogger)
                            {
                                if (fe::profilerLogger::get().wantProfile(FE_STR(group)))
                                    {
                                        strcpy(m_nameStr, name);
                                        strcpy(m_groupStr, group);
			                            m_startTime = fe::clock::getTimeSinceEpoch();
                                        m_profile = true;
                                    }
                            }
                        else
                            {
                                m_startTime = fe::clock::getTimeSinceEpoch();
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
                                fe::profilerLogger::get().add(m_groupStr, m_nameStr, runtime);
                            }
                        else if (!m_profileToLogger)
                            {
                                m_endTime = fe::clock::getTimeSinceEpoch();
                                fe::time runtime = m_endTime - m_startTime;
                                int i = 0;
                                //FE_LOG_DEBUG(runtime.asMicroseconds());
                            }
                    #endif
		            }
	        };
    }

#define FE_PROFILE(group, name) { fe::profiler t(group, name, true);
#define FE_END_PROFILE }

#if FE_PROFILE_ENGINE
    #define FE_ENGINE_PROFILE(group, name) { fe::profiler t(group, name, true);
#else
    #define FE_ENGINE_PROFILE(group, name) {
#endif

#if FE_PROFILE_ENGINE
    #define FE_ENGINE_PROFILE_NO_LOG(group, name) { fe::profiler t(group, name, false);
#else
    #define FE_ENGINE_PROFILE_NO_LOG(group, name) {
#endif