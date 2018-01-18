// utilities.hpp
// Utility objects and functions with no real place to call home
#pragma once
#define FLAT_ENGINE_EXPORT
#include "flatEngineExport.hpp"
#include <utility>
#include <vector>
#include <memory>
#include <type_traits>

namespace fe
    {
        template<typename T>
        struct is_vector : std::false_type
            {};

        template<typename ...Args>
        struct is_vector<std::vector<Args...>> : std::true_type
            {};

        template<typename ...Args>
        struct is_vector<const std::vector<Args...>> : std::true_type
            {};

        template<typename T>
        struct is_smart_pointer : std::false_type
            {};

        template<typename ...Args>
        struct is_smart_pointer<std::unique_ptr<Args...>> : std::true_type
            {};

        template<typename ...Args>
        struct is_smart_pointer<const std::unique_ptr<Args...>> : std::true_type
            {};

        template<typename ...Args>
        struct is_smart_pointer<std::shared_ptr<Args...>> : std::true_type
            {};

        template<typename ...Args>
        struct is_smart_pointer<const std::shared_ptr<Args...>> : std::true_type
            {};
    }