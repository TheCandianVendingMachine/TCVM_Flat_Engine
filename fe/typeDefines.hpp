// typeDefines.hpp
// all type defines defined in engine
#pragma once
#define FE_PROFILE_ENGINE true
#define FE_OUTPUT_PROFILE_RESULTS true
#define FE_PROFILE_RELEASE true
#define FE_PROFILE_PRINT_ZEROS false
#define FE_MAX_PROFILER_PROFILES 250 // How many different profiler profiles can be created
#define FE_PROFILER_AVERAGE_MAX 500 // how much data needs to be recorded before an average is computed

#define FE_DEBUG_NO_SEED true
#define FE_DEFAULT_RANDOM_SEED 555

#define FE_MAX_GAME_OBJECTS 1024 // Max = 2^16
#define FE_MAX_TEXT_OBJECTS 64
#define FE_MAX_Z_ORDER 64 // Maximum amount of Z-Order

#define FE_MAX_STRING_RENDER_SIZE 128

#define FE_CHAR_START 33 // The characters that will be generated into a bitmap font
#define FE_CHAR_END 126

namespace fe
    {
        using Handle = int;
        using guid = unsigned long;

        using int8 = signed char;
        using uInt8 = unsigned char;

        using int16 = signed short;
        using uInt16 = unsigned short;

        using int32 = signed int;
        using uInt32 = unsigned int;

        #if defined(_MSC_VER)
            using int64 = signed __int64;
            using uInt64 = unsigned __int64;
        #else
            using int64 = signed long long;
            using uInt64 = unsigned long long;
        #endif

        #if defined(_WIN64)
            using uIntPtr = uInt64;
        #else
            using uIntPtr = uInt32;
        #endif

    }