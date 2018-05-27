// profiler.hpp
// a custom profiler to store the name, and the time it takes for code to execute
#pragma once
#include "profilerLogger.hpp"

#define FE_PROFILE(group, name) { fe::profilerLogger::profiler t(fe::profilerLogger::get().createProfile(), group ## "_" name);
#define FE_END_PROFILE }

#if FE_PROFILE_ENGINE
    #define FE_ENGINE_PROFILE(group, name) FE_PROFILE(group, name)
#else
    #define FE_ENGINE_PROFILE(group, name) {
#endif
