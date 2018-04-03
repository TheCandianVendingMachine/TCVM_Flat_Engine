#include "fe/entity/scriptObject.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/subsystems/physics/rigidBody.hpp"

fe::scriptObject::scriptObject(baseEntity *obj) :
    m_entity(obj)
    {
    }

void fe::scriptObject::setEntityDefinition(sol::table table)
    {
        m_entityDefinition = table;
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
