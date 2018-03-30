#include "scriptObject.hpp"
#include "baseEntity.hpp"
#include "../subsystems/physics/rigidBody.hpp"

fe::scriptObject::scriptObject(baseEntity *obj) :
    m_entity(obj)
    {
    }

void fe::scriptObject::applyForce(float x, float y)
    {
        if (m_entity->getRigidBody())
            {
                m_entity->getRigidBody()->applyForce(x, y);
            }
    }

fe::Vector2d fe::scriptObject::getPosition()
    {
        return m_entity->getPosition();
    }
