// scriptHelpers.hpp
// Helpers for common script functions
#pragma once
#include "../../flatEngineExport.hpp"
#include <string>
#include <sol.hpp>

namespace fe
    {
        namespace imp
            {
                template<typename Function>
                decltype(auto) getFunctionOverload(Function &func);
                template<typename ...Functions>
                decltype(auto) getFunctionOverload(Functions &&...funcs);
            }
    }

template<typename Function>
decltype(auto) fe::imp::getFunctionOverload(Function &func)
    {
        return func;
    }

template<typename ...Functions>
decltype(auto) fe::imp::getFunctionOverload(Functions &&...funcs)
    {
        return sol::overload(std::forward<Functions>(funcs)...);
    }
