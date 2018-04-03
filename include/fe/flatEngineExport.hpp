// flatEngineExport.hpp
// just the macro to allow for exporting DLL's
#pragma once

#ifdef FLAT_ENGINE_EXPORT
    #define FLAT_ENGINE_API __declspec(dllexport)
#else
    #define FLAT_ENGINE_API __declspec(dllimport)
#endif