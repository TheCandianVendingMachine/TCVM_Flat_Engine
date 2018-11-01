// scriptHelpers.hpp
// Helpers for common script functions
#pragma once
#include "fe/flatEngineExport.hpp"
#include <string>
#include <sol.hpp>

namespace fe
    {
        namespace imp
            {
                template<typename Function>
                Function &getFunctionOverload(Function &&func);
                template<typename ...Functions>
                decltype(auto) getFunctionOverload(Functions &&...funcs);

                FLAT_ENGINE_API sol::table getTableFromPath(const std::string &path, sol::state &state);

            }
    }

template<typename Function>
Function &fe::imp::getFunctionOverload(Function &&func)
    {
        return func;
    }

template<typename ...Functions>
decltype(auto) fe::imp::getFunctionOverload(Functions &&...funcs)
    {
        return sol::overload(std::forward<Functions>(funcs)...);
    }
