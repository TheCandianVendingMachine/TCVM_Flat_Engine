// feAssert.hpp
// defines a assert Macro to assert a value, and print a message through the logger
#pragma once
#include "debug/logger.hpp"
#include <assert.h>

// define a custom ASSERT macro to allow us to push the message to the logger if needed
#ifdef _DEBUG
    #define FE_ASSERT(check, message) assert(check && message)
#else
    #define FE_ASSERT(check, message) if (!(check)) { FE_LOG_ERROR(message, "Line:", __LINE__, "File:", __FILE__); }
#endif
