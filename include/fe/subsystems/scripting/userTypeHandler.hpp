// userTypeHandler.hpp
// Creates a user-friendly interface for Sol 2's user type system
#pragma once
#include "../../flatEngineExport.hpp"
#include <sol.hpp>
#include <string>

namespace fe
    {
        class userTypeHandler
            {
                private:
                    sol::state &m_luaState;
                public:
                    FLAT_ENGINE_API userTypeHandler(sol::state &state);

                    template<typename TObj, typename ...Constructors, typename ...Args>
                    void addCustomType(const std::string &name, Args &&...args);
            };

        template<typename TObj, typename ...Constructors, typename ...Args>
        void userTypeHandler::addCustomType(const std::string &name, Args &&...args)
            {
                m_luaState.new_usertype<TObj>(name, sol::constructors<Constructors...>(), std::forward<Args>(args)...);
            }

    }
