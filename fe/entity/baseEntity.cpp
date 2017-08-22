#include "baseEntity.hpp"
#include "../subsystems/physics/rigidBody.hpp"
#include "../subsystems/graphic/renderObject/renderObject.hpp"

fe::baseEntity::baseEntity() : 
    m_killEntity(false),
    m_enabled(false),
    m_renderObject(nullptr),
    m_rigidBody(nullptr)
    {}

void fe::baseEntity::enable(bool value)
    {
        m_enabled = value;
        m_renderObject->m_draw = value;
        m_rigidBody->enable(value);
    }

bool fe::baseEntity::getEnabled() const
    {
        return m_enabled;
    }

void fe::baseEntity::kill(bool value)
    {
        m_killEntity = value;
    }

bool fe::baseEntity::isKilled() const
    {
        return m_killEntity;
    }

void fe::baseEntity::setHandle(fe::Handle handle)
    {
        m_handle = handle;
    }

fe::Handle fe::baseEntity::getHandle() const
    {
        return m_handle;
    }

void fe::baseEntity::enableCollision(bool value)
    {
        
    }

void fe::baseEntity::onDestroy(fe::baseGameState &state)
    {
        
    }

void fe::baseEntity::updateModules()
    {
        float posX = m_rigidBody->getPositionX();
        float posY = m_rigidBody->getPositionY();

        m_renderObject->m_transform.setPosition(posX, posY);
        m_collisionBody->m_bounds->m_positionX = posX;
        m_collisionBody->m_bounds->m_positionY = posY;
        m_collisionBody->m_aabb.m_positionX = posX;
        m_collisionBody->m_aabb.m_positionY = posY;

        m_moved = abs(m_rigidBody->m_oldPositionX - m_rigidBody->m_positionX) > 0.0001f || abs(m_rigidBody->m_oldPositionY - m_rigidBody->m_positionY) > 0.0001f;
    }

fe::renderObject *fe::baseEntity::getRenderObject() const
    {
        return m_renderObject;
    }

fe::rigidBody *fe::baseEntity::getRigidBody() const
    {
        return m_rigidBody;
    }

fe::collider *fe::baseEntity::getCollider() const
    {
        return m_collisionBody;
    }

