// userEntityObject.hpp
// A generic object that holds Lua functions that will be called on update
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../../flatEngineExport.hpp"
#include "../messaging/eventHandler.hpp"
#include "../../typeDefines.hpp"
#include "../../objectManagement/str.hpp"
#include "../scripting/luaFunctionReference.hpp"
#include <unordered_map>

namespace fe
    {
        class gameWorld;
        class scriptObject;
        struct prefabObject;
        class userEntityObject : public fe::eventHandler
            {
                private:
                    fe::luaFunctionReference *m_onAdd;
                    fe::luaFunctionReference *m_onRemove;
                    fe::luaFunctionReference *m_update;
                    fe::luaFunctionReference *m_fixedUpdate;
                    fe::luaFunctionReference *m_postUpdate;

                    std::unordered_map<fe::str, fe::luaFunctionReference*> m_userFunctions;
                    std::unordered_map<fe::str, fe::luaFunctionReference*> m_events;

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

                    FLAT_ENGINE_API void handleEvent(const fe::gameEvent &event);

                    template<typename ...Args>
                    decltype(auto) call(const std::string &functionName, Args &&...args);
                    decltype(auto) call(const std::string &functionName);

                    FLAT_ENGINE_API userEntityObject &operator=(const fe::prefabObject &rhs);
            };

        template<typename ...Args>
        decltype(auto) userEntityObject::call(const std::string &functionName, Args &&...args)
            {
                if (m_userFunctions.find(FE_STR(functionName.c_str())) == m_userFunctions.end())
                    {
                        FE_LOG_ERROR("Function with name '", functionName, "' does not exist");
                        return m_userFunctions[0]->call();
                    }

                return m_userFunctions[FE_STR(functionName.c_str())]->call(std::forward<Args>(args)...);
            }
    }
