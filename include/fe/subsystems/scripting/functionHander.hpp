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

#include "scriptHelpers.hpp"

namespace fe
    {
        class functionHandler
            {
                private:
                    sol::state &m_state;

                public:
                    FLAT_ENGINE_API functionHandler(sol::state &state);

                    template<typename Function, typename T>
                    void registerObjectFunction(const std::string &functionName, T objRef, Function &func);
                    template<typename ...Functions, typename T>
                    void registerObjectFunction(const std::string &functionName, T objRef, Functions &&...funcs);

                    template<typename Function>
                    void registerFunction(const std::string &functionName, Function &func);
                    template<typename ...Functions>
                    void registerFunction(const std::string &functionName, Functions &&...funcs);

                    template<typename ...Args>
                    decltype(auto) call(const std::string &functionName, Args &&...args);
                    decltype(auto) call(const std::string &functionName);

                    FLAT_ENGINE_API sol::protected_function getFunction(const std::string &functionName);

            };

        template<typename Function, typename T>
        void functionHandler::registerObjectFunction(const std::string &functionName, T objRef, Function &func)
            {
                m_state.set_function(functionName, fe::imp::getFunctionOverload(func), objRef);
            }

        template<typename ...Functions, typename T>
        void functionHandler::registerObjectFunction(const std::string &functionName, T objRef, Functions &&...funcs)
            {
                m_state.set_function(functionName, fe::imp::getFunctionOverload(std::forward<Functions>(funcs)...), objRef);
            }

        template<typename Function>
        void functionHandler::registerFunction(const std::string &functionName, Function &func)
            {
                m_state.set_function(functionName, fe::imp::getFunctionOverload(func));
            }

        template<typename ...Functions>
        void functionHandler::registerFunction(const std::string &functionName, Functions &&...funcs)
            {
                m_state.set_function(functionName, fe::imp::getFunctionOverload(std::forward<Functions>(funcs)...));
            }

        template<typename ...Args>
        decltype(auto) functionHandler::call(const std::string &functionName, Args &&...args)
            {
                return m_state[functionName].call(std::forward<Args>(args)...);
            }

        inline decltype(auto) fe::functionHandler::call(const std::string &functionName)
            {
                return m_state[functionName].call();
            }
    }
