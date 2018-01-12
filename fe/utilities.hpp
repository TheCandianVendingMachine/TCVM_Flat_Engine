// utilities.hpp
// Utility objects and functions with no real place to call home
#pragma once
#define FLAT_ENGINE_EXPORT
#include "flatEngineExport.hpp"
#include <utility>
#include <vector>
#include <memory>

namespace fe
    {
        template<typename T>
        struct is_vector
            {
                static const bool value = false;
            };

        template<typename ...Args>
        struct is_vector<std::vector<Args...>>
            {
                static const bool value = true;
            };

        template<typename ...Args>
        struct is_vector<const std::vector<Args...>>
            {
                static const bool value = true;
            };

        template<typename T>
        struct is_smart_pointer
            {
                static const bool value = false;
            };

        template<typename ...Args>
        struct is_smart_pointer<std::unique_ptr<Args...>>
            {
                static const bool value = true;
            };

        template<typename ...Args>
        struct is_smart_pointer<const std::unique_ptr<Args...>>
            {
                static const bool value = true;
            };

        template<typename ...Args>
        struct is_smart_pointer<std::shared_ptr<Args...>>
            {
                static const bool value = true;
            };

        template<typename ...Args>
        struct is_smart_pointer<const std::shared_ptr<Args...>>
            {
                static const bool value = true;
            };
    }