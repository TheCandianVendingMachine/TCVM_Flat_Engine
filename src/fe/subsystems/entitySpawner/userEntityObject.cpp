#include "fe/subsystems/entitySpawner/userEntityObject.hpp"
#include "fe/entity/scriptObject.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/entitySpawner/prefabObject.hpp"
#include "fe/subsystems/scripting/scriptManager.hpp"

fe::userEntityObject::userEntityObject() : 
    m_index(0),
    m_active(false),
    m_onAdd(nullptr),
    m_onRemove(nullptr),
    m_update(nullptr),
    m_fixedUpdate(nullptr),
    m_postUpdate(nullptr)
    {
    }

void fe::userEntityObject::startUp(unsigned int index)
    {
        m_index = index;
        m_active = true;
    }

void fe::userEntityObject::shutDown()
    {
        fe::engine::get().getEventSender().unsubscribeAll(this);
    }

unsigned int fe::userEntityObject::index() const
    {
        return m_index;
    }

bool fe::userEntityObject::active() const
    {
        return m_active;
    }

void fe::userEntityObject::onAdd(fe::scriptObject *object, fe::gameWorld &world)
    {
        if (!m_onAdd) return;
        m_onAdd->call(*object);
    }

void fe::userEntityObject::onRemove(fe::scriptObject *object, fe::gameWorld &world)
    {
        if (!m_onRemove) return;
        m_onRemove->call(*object);
    }

void fe::userEntityObject::update(fe::scriptObject *object)
    {
        if (!m_update) return;
        m_update->call(*object);
    }

void fe::userEntityObject::fixedUpdate(fe::scriptObject *object, float deltaTime)
    {
        if (!m_fixedUpdate) return;
        m_fixedUpdate->call(*object, deltaTime);
    }

void fe::userEntityObject::postUpdate(fe::scriptObject *object)
    {
        if (!m_postUpdate) return;
        m_postUpdate->call(*object);
    }

void fe::userEntityObject::handleEvent(const fe::gameEvent &event)
    {
        if (m_events.find(event.eventType) != m_events.end())
            {
                m_events[event.eventType]->call(event.argNumber, event.args);
            }
    }

decltype(auto) fe::userEntityObject::call(const std::string &functionName)
    {
        if (m_userFunctions.find(FE_STR(functionName.c_str())) == m_userFunctions.end())
            {
                FE_LOG_ERROR("Function with name '", functionName, "' does not exist");
                return m_userFunctions[0]->call();
            }

        return m_userFunctions[FE_STR(functionName.c_str())]->call();
    }

fe::userEntityObject &fe::userEntityObject::operator=(const fe::prefabObject &rhs)
    {
        m_onAdd =           rhs.m_onAdd;
        m_onRemove =        rhs.m_onRemove;
        m_update =          rhs.m_update;
        m_postUpdate =      rhs.m_postUpdate;
        m_fixedUpdate =     rhs.m_fixedUpdate;
        
        for (auto &event : rhs.m_events)
            {
                m_events[event.first] = event.second;
                fe::engine::get().getEventSender().subscribe(this, event.first);
            }

        m_userFunctions.insert(rhs.m_userFunctions.begin(), rhs.m_userFunctions.end());


        sol::state_view(rhs.m_entityTable.lua_state()).script(
R"(
error_func = function()
    return 0 
end
)");
        m_userFunctions[0] = &fe::engine::get().getScriptManager().getFunctionHandler().getLuaFunction("error_func");

        return *this;
    }

