// functionHandler.hpp
// Handles the registration of functions
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/str.hpp"
#include "../../typeDefines.hpp"
#include <unordered_map>
#include <string>
#include <sol.hpp>

namespace fe
    {
        class functionHandler
            {
                private:
                    sol::state &m_state;

                    std::unordered_map<fe::str, sol::protected_function> m_registeredFunctions;

                public:
                    FLAT_ENGINE_API functionHandler(sol::state &state);

                    FLAT_ENGINE_API void registerLuaFunctions();

                    template<typename Function, typename T>
                    void registerObjectFunction(const std::string &functionName, T objRef, Function &func);
                    template<typename ...Functions, typename T>
                    void registerObjectFunction(const std::string &functionName, T objRef, Functions &&...funcs);

                    template<typename Function>
                    void registerFunction(const std::string &functionName, Function &func);
                    template<typename ...Functions>
                    void registerFunction(const std::string &functionName, Functions &&...funcs);

                    template<typename Function>
                    decltype(auto) getFunctionOverload(Function &func);
                    template<typename ...Functions>
                    decltype(auto) getFunctionOverload(Functions &&...funcs);

                    template<typename ...Args>
                    decltype(auto) call(const std::string &functionName, Args &&...args);

                    FLAT_ENGINE_API sol::protected_function &getFunction(const std::string &functionName);
                    FLAT_ENGINE_API sol::protected_function &getFunction(fe::str functionName);

            };

        template<typename Function, typename T>
        void functionHandler::registerObjectFunction(const std::string &functionName, T objRef, Function &func)
            {
                m_state.set_function(functionName, getFunctionOverload(func), objRef);
                m_registeredFunctions[FE_STR(functionName.c_str())] = m_state[functionName];
            }

        template<typename ...Functions, typename T>
        void functionHandler::registerObjectFunction(const std::string &functionName, T objRef, Functions &&...funcs)
            {
                m_state.set_function(functionName, getFunctionOverload(std::forward<Functions>(funcs)...), objRef);
                m_registeredFunctions[FE_STR(functionName.c_str())] = m_state[functionName];
            }

        template<typename Function>
        void functionHandler::registerFunction(const std::string &functionName, Function &func)
            {
                m_state.set_function(functionName, getFunctionOverload(func));
                m_registeredFunctions[FE_STR(functionName.c_str())] = m_state[functionName];
            }

        template<typename ...Functions>
        void functionHandler::registerFunction(const std::string &functionName, Functions &&...funcs)
            {
                m_state.set_function(functionName, getFunctionOverload(std::forward<Functions>(funcs)...));
                m_registeredFunctions[FE_STR(functionName.c_str())] = m_state[functionName];
            }

        template<typename Function>
        decltype(auto) functionHandler::getFunctionOverload(Function &func)
            {
                return func;
            }

        template<typename ...Functions>
        decltype(auto) functionHandler::getFunctionOverload(Functions &&...funcs)
            {
                return sol::overload(std::forward<Functions>(funcs)...);
            }

        template<typename ...Args>
        decltype(auto) functionHandler::call(const std::string &functionName, Args &&...args)
            {
                return m_registeredFunctions[FE_STR(functionName.c_str())].call(std::forward<Args>(args)...);
            }
    }
