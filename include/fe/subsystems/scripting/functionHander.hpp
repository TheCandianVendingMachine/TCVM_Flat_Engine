// functionHandler.hpp
// Handles the registration of functions
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/objectManagement/str.hpp"
#include "fe/typeDefines.hpp"
#include <string>
#include <sol.hpp>
#include <vector>

#include "scriptHelpers.hpp"
#include "luaFunctionReference.hpp"

namespace fe
    {
        class functionHandler
            {
                private:
                    sol::state &m_state;
                    std::vector<fe::luaFunctionReference*> m_registeredFunctions;

                public:
                    FLAT_ENGINE_API functionHandler(sol::state &state);

                    template<typename ...Functions, typename T>
                    void registerCPPObjectFunction(const std::string &functionName, T *objRef, Functions &&...funcs);
                    template<typename ...Functions>
                    void registerCPPFunction(const std::string &functionName, Functions &&...funcs);

                    template<typename ...Args>
                    decltype(auto) call(const std::string &functionName, Args &&...args);

                    FLAT_ENGINE_API void reloadAllLuaFunctions();
                    FLAT_ENGINE_API void reloadLuaFunction(fe::luaFunctionReference &function);
                    FLAT_ENGINE_API fe::luaFunctionReference &getLuaFunction(const std::string &functionName);
                    FLAT_ENGINE_API fe::luaFunctionReference &getLuaFunction(const std::string &luaPathName, const std::string &functionName);

                    FLAT_ENGINE_API void shutDown();

            };

        template<typename ...Functions, typename T>
        void functionHandler::registerCPPObjectFunction(const std::string &functionName, T *objRef, Functions &&...funcs)
            {
                m_state.set_function(functionName, fe::imp::getFunctionOverload(std::forward<Functions>(funcs)...), objRef);
            }

        template<typename ...Functions>
        void functionHandler::registerCPPFunction(const std::string &functionName, Functions &&...funcs)
            {
                m_state.set_function(functionName, fe::imp::getFunctionOverload(std::forward<Functions>(funcs)...));
            }

        template<typename ...Args>
        decltype(auto) functionHandler::call(const std::string &functionName, Args &&...args)
            {
                return m_registeredFunctions[functionName]->call(std::forward<Args>(args)...);
            }
    }
