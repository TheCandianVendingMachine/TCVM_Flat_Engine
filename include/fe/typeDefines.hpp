// typeDefines.hpp
// all type defines defined in engine
#pragma once
#define FE_IS_ENGINE true // Defined only if engine exists. Useful if writing potentially modular classes

#ifndef FE_PROFILE_ENGINE
    #define FE_PROFILE_ENGINE false
#endif

#ifndef FE_OUTPUT_PROFILE_RESULTS
    #define FE_OUTPUT_PROFILE_RESULTS true
#endif

#ifndef FE_PROFILE_RELEASE
    #define FE_PROFILE_RELEASE true
#endif

#ifndef FE_PROFILE_PRINT_ZEROS
    #define FE_PROFILE_PRINT_ZEROS false
#endif

#define FE_MAX_PROFILER_PROFILES 250 // How many different profiler profiles can be created
#define FE_PROFILER_AVERAGE_MAX 500 // how much data needs to be recorded before an average is computed

#ifndef FE_DEBUG_NO_SEED
    #define FE_DEBUG_NO_SEED true
#endif

#ifndef FE_DEFAULT_RANDOM_SEED
    #define FE_DEFAULT_RANDOM_SEED 555
#endif

#define FE_MAX_GAME_OBJECTS 16384 // Max = 2^16
#define FE_MAX_TEXT_OBJECTS 64
#define FE_MAX_Z_ORDER 64 // Maximum amount of Z-Order

#define FE_MAX_STRING_RENDER_SIZE 128

#define FE_CHAR_START 33 // The characters that will be generated into a bitmap font
#define FE_CHAR_END 126
#ifndef FE_DEBUG_ALLOCATER
    #define FE_DEBUG_ALLOCATER true
#endif

#ifndef FE_THREAD_COUNT
    #define FE_THREAD_COUNT 8
#endif

#ifndef FE_THREAD_HERTZ
    #define FE_THREAD_HERTZ 60
#endif

namespace fe
    {
        using Handle = int;
        using str = unsigned long;

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

        static constexpr float FE_EPSILON = 0.0001f;

    }