// enumHandler.hpp
// handles the addition of enums into code.
#pragma once
#include "../../flatEngineExport.hpp"
#include <sol.hpp>

namespace fe
    {
        class enumHandler
            {
                private:
                    sol::state &m_state;

                public:
                    FLAT_ENGINE_API enumHandler(sol::state &state);

                    template<typename T, typename ...Args>
                    void registerEnum(const char *enumName, const char *valueName, T &&enumValue, Args &&...args);

                    template<typename T>
                    void addEnumValue(const char *enumName, const char *valueName, T &&enumValue);

            };


        template<typename T, typename ...Args>
        void enumHandler::registerEnum(const char *enumName, const char *valueName, T &&enumValue, Args &&...args)
            {
                m_state.new_enum(enumName, valueName, enumValue, std::forward<Args>(args)...);
            }

        template<typename T>
        void enumHandler::addEnumValue(const char *enumName, const char *valueName, T &&enumValue)
            {
                m_state[enumName].get<sol::table>().add(valueName, enumValue);
            }
    }
