#include "userEntityObject.hpp"

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

unsigned int fe::userEntityObject::index() const
    {
        return m_index;
    }

bool fe::userEntityObject::active() const
    {
        return m_active;
    }

void fe::userEntityObject::onAdd(fe::gameWorld &world)
    {
        if (!m_onAdd.valid()) return;
        m_onAdd.call();
    }

void fe::userEntityObject::onRemove(fe::gameWorld &world)
    {
        if (!m_onRemove.valid()) return;
        m_onRemove.call();
    }

void fe::userEntityObject::update()
    {
        if (!m_update.valid()) return;
        m_update.call();
    }

void fe::userEntityObject::postUpdate()
    {
        if (!m_postUpdate.valid()) return;
        m_postUpdate.call();
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

void fe::userEntityObject::setPostUpdate(const sol::protected_function &func)
    {
        m_postUpdate = func;
    }
