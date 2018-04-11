// userEntityObject.hpp
// A generic object that holds Lua functions that will be called on update
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../messaging/eventHandler.hpp"
#include "../../typeDefines.hpp"
#include <sol.hpp>
#include <unordered_map>

namespace fe
    {
        class gameWorld;
        class scriptObject;
        class userEntityObject : public fe::eventHandler
            {
                private:
                    sol::protected_function m_onAdd;
                    sol::protected_function m_onRemove;
                    sol::protected_function m_update;
                    sol::protected_function m_fixedUpdate;
                    sol::protected_function m_postUpdate;

                    std::unordered_map<fe::str, sol::protected_function> m_events;

                    unsigned int m_index;
                    bool m_active;

                public:
                    FLAT_ENGINE_API userEntityObject();

                    FLAT_ENGINE_API void startUp(unsigned int index);
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API unsigned int index() const;
                    FLAT_ENGINE_API bool active() const;

                    FLAT_ENGINE_API void onAdd(fe::scriptObject *object, fe::gameWorld &world);
                    FLAT_ENGINE_API void onRemove(fe::scriptObject *object, fe::gameWorld &world);
                    FLAT_ENGINE_API void update(fe::scriptObject *object);
                    FLAT_ENGINE_API void fixedUpdate(fe::scriptObject *object, float deltaTime);
                    FLAT_ENGINE_API void postUpdate(fe::scriptObject *object);

                    FLAT_ENGINE_API void setOnAdd(const sol::protected_function &func);
                    FLAT_ENGINE_API void setOnRemove(const sol::protected_function &func);
                    FLAT_ENGINE_API void setUpdate(const sol::protected_function &func);
                    FLAT_ENGINE_API void setFixedUpdate(const sol::protected_function &func);
                    FLAT_ENGINE_API void setPostUpdate(const sol::protected_function &func);

                    FLAT_ENGINE_API void addEvent(fe::str event, const sol::protected_function &callback);
                    FLAT_ENGINE_API void handleEvent(const fe::gameEvent &event);
            };
    }
