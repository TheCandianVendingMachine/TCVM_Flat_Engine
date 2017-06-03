// string.hpp
// a hashed-string. Used for GUID's since it takes up no memory, can be compared quickly, and is easy to store
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../debug/logger.hpp"

namespace fe
    {
        using guid = unsigned long;
        constexpr fe::guid hash(const char *input, unsigned int hash)
            {
                return input[0] == '\0' ? hash : fe::hash(&input[1], hash * 33 ^ (fe::guid)(input[0]));
            }
        // implicitely calls the hash with the magic number "5381"
        constexpr fe::guid hashImpl(const char *input)
            {
                return hash(input, 5381);
            }
    }

// Creates a GUID based on the input string
#define FE_STR(input) fe::hashImpl(input)