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

fe::collider &fe::baseEntity::getCollider() const
    {
        return *m_collider;
    }
