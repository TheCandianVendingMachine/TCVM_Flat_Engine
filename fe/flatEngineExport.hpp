// flatEngineImport.hpp
// defines many useful macros to be used throughout the engine
#pragma once

#ifdef FLAT_ENGINE_EXPORT
    #define FLAT_ENGINE_API __declspec(dllexport)
#else
    #define FLAT_ENGINE_API __declspec(dllimport)
#endif