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

        template<typename T>
        struct is_c_str : std::integral_constant<bool,
            std::is_same<typename std::decay<T>::type, char*>::value ||
            std::is_same<typename std::decay<T>::type, char const*>::value>
            {};

        template<typename T>
        struct is_cpp_str : std::integral_constant<bool,
            std::is_same<typename std::decay<T>::type, std::string>::value ||
            std::is_same<typename std::decay<T>::type, const std::string>::value>
            {};

        template<typename T>
        struct is_str : std::integral_constant<bool,
            fe::is_c_str<T>::value ||
            fe::is_cpp_str<T>::value>
            {};
    }