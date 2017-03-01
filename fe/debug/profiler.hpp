// profiler.hpp
// a custom profiler to store the name, and the time it takes for code to execute
#pragma once
#include "../time/time.hpp"
#include "../time/clock.hpp"
#include <cstring>

namespace fe
{
	struct profiler
	{
		fe::time m_startTime;
		fe::time m_endTime;
		
		const char *m_name;
		
		profiler(const char *name) 
		{
			strcpy(m_name, name);
			m_startTime = fe::clock::getTimeSinceEpoch();
		}
		
		~profiler()
		{
			m_endTime = fe::clock::getTimeSinceEpoch();
			fe::time runtime = m_endTime - m_startTime;
		}
	}
}

#define PROFILE(name) { fe::profiler t(name);
#define END_PROFILE() }