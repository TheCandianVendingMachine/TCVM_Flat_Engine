// luaFunctionReference.hpp
// A reference to a Lua function. Used instead of and is a wrapper for sol::protected_function
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "fe/typeDefines.hpp"

#include <utility>
#include <string>
#include <sol.hpp>

namespace fe
    {
        class functionHandler;

        class luaFunctionReference
            {
                private:
                    sol::protected_function m_function;
                    std::string m_luaPathName;
                    std::string m_functionName;

                    functionHandler &m_functionHandler;

                    FLAT_ENGINE_API explicit luaFunctionReference(sol::protected_function function, std::string luaPath, const std::string &functionName, functionHandler &handler);

                    friend class functionHandler;


                public:
                    luaFunctionReference() = delete;
                    FLAT_ENGINE_API void reload();

                    template<typename ...Args>
                    decltype(auto) call(Args &&...args);
                    template<typename ...Args>
                    decltype(auto) operator()(Args &&...args);

                    FLAT_ENGINE_API bool operator==(const luaFunctionReference &rhs);

            };

        template<typename ...Args>
        decltype(auto) luaFunctionReference::call(Args &&...args)
            {
                if (m_function.valid())
                    {
                        return m_function.call(std::forward<Args>(args)...);
                    }
            }

        template<typename ...Args>
        decltype(auto) luaFunctionReference::operator()(Args &&...args)
            {
                return call(std::forward<Args>(args)...);
            }
    }
