// typeDefines.hpp
// all type defines defined in engine
#pragma once
#define FE_PROFILE_ENGINE false
#define FE_PROFILE_PRINT_ZEROS false
#define FE_DEBUG_NO_SEED true
#define FE_MAX_GAME_OBJECTS 65536 // 2^16

namespace fe
    {
        using Handle = int;
        using guid = unsigned long;
    }