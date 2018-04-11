#include "fe/subsystems/entitySpawner/userEntityObject.hpp"
#include "fe/entity/scriptObject.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include "fe/engine.hpp"

fe::userEntityObject::userEntityObject() : 
    m_index(0),
    m_active(false)
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
        if (!m_onAdd.valid()) return;
        m_onAdd.call(*object);
    }

void fe::userEntityObject::onRemove(fe::scriptObject *object, fe::gameWorld &world)
    {
        if (!m_onRemove.valid()) return;
        m_onRemove.call(*object);
    }

void fe::userEntityObject::update(fe::scriptObject *object)
    {
        if (!m_update.valid()) return;
        m_update.call(*object);
    }

void fe::userEntityObject::fixedUpdate(fe::scriptObject *object, float deltaTime)
    {
        if (!m_fixedUpdate.valid()) return;
        m_fixedUpdate.call(*object, deltaTime);
    }

void fe::userEntityObject::postUpdate(fe::scriptObject *object)
    {
        if (!m_postUpdate.valid()) return;
        m_postUpdate.call(*object);
    }

void fe::userEntityObject::setOnAdd(const sol::protected_function &func)
    {
        m_onAdd = func;
    }

void fe::userEntityObject::setOnRemove(const sol::protected_function &func)
    {
        m_onRemove = func;
    }

void fe::userEntityObject::setUpdate(const sol::protected_function &func)
    {
        m_update = func;
    }

void fe::userEntityObject::setFixedUpdate(const sol::protected_function &func)
    {
        m_fixedUpdate = func;
    }

void fe::userEntityObject::setPostUpdate(const sol::protected_function &func)
    {
        m_postUpdate = func;
    }

void fe::userEntityObject::addEvent(fe::str event, const sol::protected_function &callback)
    {
        m_events[event] = callback;
        fe::engine::get().getEventSender().subscribe(this, event);
    }

void fe::userEntityObject::handleEvent(const fe::gameEvent &event)
    {
        if (m_events.find(event.eventType) != m_events.end())
            {
                m_events[event.eventType].call(event.argNumber, event.args);
            }
    }
