#include "baseEntity.hpp"

void fe::baseEntity::addCollider(fe::collider *collider)
    {
        m_collider = collider;
    }

fe::baseEntity::baseEntity()
    {
    }

fe::baseEntity::~baseEntity()
    {
    }

void fe::baseEntity::enableCollision(bool value)
    {
        m_collider->m_collisonEnabled = value;
    }

fe::collider &fe::baseEntity::getCollider() const
    {
        return *m_collider;
    }
