// component.hpp
// A component base class for the entity. Allows users to implement functionality without having to modify the base class
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/scripting/scriptManager.hpp"
#include "fe/subsystems/scripting/luaFunctionReference.hpp"
#include "fe/engine.hpp"
#include "fe/entity/baseEntity.hpp"
#include <sol.hpp>
#include <utility>

namespace fe
    {
        class baseEntity;

        class componentBase
            {
                protected:
                    fe::str m_id;
                    fe::luaFunctionReference *m_onAdd;
                    fe::luaFunctionReference *m_onRemove;
                    fe::luaFunctionReference *m_update;
                    fe::luaFunctionReference *m_fixedUpdate;
                    fe::baseEntity *m_owner;

                    bool m_initialized;

                    virtual void initLuaValues(sol::table table, const char *componentPath, const char *entName) {}

                public:
                    FLAT_ENGINE_API componentBase(const char *str);

                    FLAT_ENGINE_API bool isInitialized() const;
                    FLAT_ENGINE_API void engineInitLuaValues(sol::table table, const char *componentPath, const char *entName);

                    FLAT_ENGINE_API fe::scriptObject *getOwner();
                    FLAT_ENGINE_API fe::str getID() const;

                    virtual void engineOnAdd(fe::baseEntity *ent) {}
                    virtual void engineOnRemove(fe::baseEntity *ent) {}
                    virtual void engineUpdate() {}
                    virtual void engineFixedUpdate(float deltaTime) {}

            };

        template<typename TObj>
        class component : public componentBase
            {
                protected:
                    virtual void onAdd(fe::baseEntity *ent) {}
                    virtual void onRemove(fe::baseEntity *ent) {}
                    virtual void update() {}
                    virtual void fixedUpdate(float deltaTime) {}

                public:
                    template<typename ...Args>
                    component(const std::string &name, Args &&...args);

                    void engineOnAdd(fe::baseEntity *ent);
                    void engineOnRemove(fe::baseEntity *ent);
                    void engineUpdate();
                    void engineFixedUpdate(float deltaTime);

            };

        template<typename TObj>
        inline void fe::component<TObj>::engineOnAdd(fe::baseEntity *ent)
            {
                if (m_onAdd)
                    {
                        m_onAdd->call(static_cast<TObj*>(this), static_cast<fe::scriptObject*>(ent));
                    }
                m_owner = ent;
                onAdd(ent);
            }

        template<typename TObj>
        inline void fe::component<TObj>::engineOnRemove(fe::baseEntity *ent)
            {
                if (m_onRemove)
                    {
                        m_onRemove->call(static_cast<TObj*>(this), static_cast<fe::scriptObject*>(ent));
                    }
                onRemove(ent);
            }

        template<typename TObj>
        inline void fe::component<TObj>::engineUpdate()
            {
                if (m_update)
                    {
                        m_update->call(static_cast<TObj*>(this));
                    }
                update();
            }

        template<typename TObj>
        inline void fe::component<TObj>::engineFixedUpdate(float deltaTime)
            {
                if (m_fixedUpdate)
                    {
                        m_fixedUpdate->call(static_cast<TObj*>(this), deltaTime);
                    }
                fixedUpdate(deltaTime);
            }

        template<typename TObj>
        template<typename ...Args>
        inline component<TObj>::component(const std::string &name, Args &&...args) :
            componentBase(name.c_str())
            {
                fe::engine::get().getScriptManager().getUserTypeHandler().addCustomType<TObj>(name, sol::base_classes, sol::bases<component>(),
                    "getOwner", &TObj::getOwner,
                std::forward<Args>(args)...);
            }
    }

