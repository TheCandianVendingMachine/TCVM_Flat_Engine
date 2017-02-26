// flatEngineImport.hpp
// defines many useful macros to be used throughout the engine
#pragma once
#include <assert.h>

#ifdef FLAT_ENGINE_EXPORT
    #define FLAT_ENGINE_API __declspec(dllexport)
#else
    #define FLAT_ENGINE_API __declspec(dllimport)
#endif

// define a custom ASSERT macro to allow us to push the message to the logger if needed
#ifdef _DEBUG
    #define FE_ASSERT(check, message) assert(check && message)
#elif
    #define FE_ASSERT(check, message)
#endif

namespace fe 
    {
        namespace priv
            {
                
            }
    }