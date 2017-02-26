// flatEngineExports.hpp
// defines a Macro for all classes to be built as DLL and Lib
#pragma once

#ifdef FLAT_ENGINE_EXPORTS
    #define FLAT_ENGINE_API __declspec(dllexport)
#else
    #define FLAT_ENGINE_API __declspec(dllimport)
#endif