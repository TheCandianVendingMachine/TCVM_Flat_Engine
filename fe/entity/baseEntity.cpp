#include "baseEntity.hpp"
#include "../subsystems/gameState/gameState.hpp"
#include "../subsystems/collision/collisionHandler.hpp"

void fe::baseEntity::addCollider(fe::collider *collider)
    {
        m_collider = collider;
    }

fe::baseEntity::baseEntity()
    {
        m_collider = nullptr;
        m_destroyed = false;
    }

fe::baseEntity::~baseEntity()
    {
    }

void fe::baseEntity::setHandle(fe::Handle handle)
    {
        m_handle = handle;
    }

void fe::baseEntity::enableCollision(bool value)
    {
        m_collider->m_collisonEnabled = value;
    }

void fe::baseEntity::destroy()
    {
        m_destroyed = true;
    }

bool fe::baseEntity::isDestroyed() const
    {
        return m_destroyed;
    }

void fe::baseEntity::onDestroy(fe::baseGameState &state)
    {
        state.getCollisionHandler().remove(m_collider);
    }

fe::collider &fe::baseEntity::getCollider() const
    {
        return *m_collider;
    }

bool fe::baseEntity::hasCollider() const
    {
        return m_collider;
    }

fe::Handle fe::baseEntity::getHandle() const
    {
        return m_handle;
    }

fe::Vector2d fe::baseEntity::getSize() const
    {
        return fe::transformable::getSize(m_verticies[2].position);
    }
