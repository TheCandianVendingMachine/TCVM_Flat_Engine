// feNew.hpp
// replaces new and delete for a custom memory allocator
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"

FLAT_ENGINE_API void *feAlloc(size_t bytes);
FLAT_ENGINE_API void feDelete(void *memory);

inline void *operator new(size_t bytes)
    {
        return feAlloc(bytes);
    }

inline void operator delete(void *memory)
    {
        feDelete(memory);
    }
