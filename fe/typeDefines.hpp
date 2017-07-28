// typeDefines.hpp
// all type defines defined in engine
#pragma once
#define FE_PROFILE_ENGINE true
#define FE_PROFILE_RELEASE false
#define FE_PROFILE_PRINT_ZEROS false
#define FE_MAX_PROFILER_PROFILES 5000 // How many different profiler profiles can be created
#define FE_PROFILER_AVERAGE_MAX 500 // how much data needs to be recorded before an average is computed

#define FE_DEBUG_NO_SEED true
#define FE_DEFAULT_RANDOM_SEED 555

#define FE_MAX_GAME_OBJECTS 65536 // 2^16

namespace fe
    {
        using Handle = int;
        using guid = unsigned long;
    }