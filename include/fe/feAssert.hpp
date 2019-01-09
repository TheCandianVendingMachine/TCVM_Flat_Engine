// feAssert.hpp
// defines a assert Macro to assert a value, and print a message through the logger
#pragma once

// define a custom ASSERT macro to allow us to push the message to the logger if needed
#ifdef _DEBUG
    #include <assert.h>
    #define FE_ASSERT(check, message) assert(check && message)
#else
    #include "debug/logger.hpp"
    #define FE_ASSERT(check, message) if (!(check)) { FE_LOG_ERROR(message, "Line:", __LINE__, "File:", __FILE__); }
#endif
