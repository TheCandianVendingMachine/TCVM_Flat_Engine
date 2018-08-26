// functionHandler.hpp
// Handles the registration of functions
#pragma once
#include "../../flatEngineExport.hpp"
#include "../../objectManagement/str.hpp"
#include "../../typeDefines.hpp"
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

                    FLAT_ENGINE_API sol::table getTableFromPath(const std::string &path);

                public:
                    FLAT_ENGINE_API functionHandler(sol::state &state);

                    template<typename Function, typename T>
                    void registerCPPObjectFunction(const std::string &functionName, T objRef, Function &func);
                    template<typename ...Functions, typename T>
                    void registerCPPObjectFunction(const std::string &functionName, T objRef, Functions &&...funcs);

                    template<typename Function>
                    void registerCPPFunction(const std::string &functionName, Function &func);
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

        template<typename Function, typename T>
        void functionHandler::registerCPPObjectFunction(const std::string &functionName, T objRef, Function &func)
            {
                m_state.set_function(functionName, fe::imp::getFunctionOverload(func), objRef);
            }

        template<typename ...Functions, typename T>
        void functionHandler::registerCPPObjectFunction(const std::string &functionName, T objRef, Functions &&...funcs)
            {
                m_state.set_function(functionName, fe::imp::getFunctionOverload(std::forward<Functions>(funcs)...), objRef);
            }

        template<typename Function>
        void functionHandler::registerCPPFunction(const std::string &functionName, Function &func)
            {
                m_state.set_function(functionName, fe::imp::getFunctionOverload(func));
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
