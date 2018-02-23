// userEntityObject.hpp
// A generic object that holds Lua functions that will be called on update
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include <sol.hpp>

namespace fe
    {
        class gameWorld;
        class userEntityObject
            {
                private:
                    sol::protected_function m_onAdd;
                    sol::protected_function m_onRemove;
                    sol::protected_function m_update;
                    sol::protected_function m_postUpdate;

                    unsigned int m_index;
                    bool m_active;

                public:
                    FLAT_ENGINE_API userEntityObject();

                    FLAT_ENGINE_API void startUp(unsigned int index);
                    FLAT_ENGINE_API unsigned int index() const;
                    FLAT_ENGINE_API bool active() const;

                    FLAT_ENGINE_API void onAdd(fe::gameWorld &world);
                    FLAT_ENGINE_API void onRemove(fe::gameWorld &world);
                    FLAT_ENGINE_API void update();
                    FLAT_ENGINE_API void postUpdate();

                    FLAT_ENGINE_API void setOnAdd(const sol::protected_function &func);
                    FLAT_ENGINE_API void setOnRemove(const sol::protected_function &func);
                    FLAT_ENGINE_API void setUpdate(const sol::protected_function &func);
                    FLAT_ENGINE_API void setPostUpdate(const sol::protected_function &func);
            };
    }
