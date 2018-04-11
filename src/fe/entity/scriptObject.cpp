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

fe::baseEntity *fe::scriptObject::getBaseEntity() const
    {
        return m_entity;
    }

std::string fe::scriptObject::scriptObjectGetName()
    {
        return m_entity->getName();
    }

void fe::scriptObject::scriptObjectSetVelocity(float x, float y)
    {
        if (m_entity->getRigidBody())
            {
                m_entity->getRigidBody()->setVelocity(x, y);
            }
    }

void fe::scriptObject::scriptObjectSetForce(float x, float y)
    {
        if (m_entity->getRigidBody())
            {
                m_entity->getRigidBody()->setForce(x, y);
            }
    }

void fe::scriptObject::scriptObjectApplyForce(float x, float y)
    {
        if (m_entity->getRigidBody())
            {
                m_entity->getRigidBody()->applyForce(x, y);
            }
    }

fe::Vector2d fe::scriptObject::scriptObjectGetPosition()
    {
        return m_entity->getPosition();
    }

void fe::scriptObject::scriptObjectDestroy()
    {
        m_entity->kill(true);
    }

fe::Vector2d fe::scriptObject::scriptObjectGetVelocity()
    {
        if (m_entity->getRigidBody())
            {
                return m_entity->getRigidBody()->getVelocity();
            }
        return fe::Vector2d();
    }

fe::Vector2d fe::scriptObject::scriptObjectGetNormalForce()
    {
        if (m_entity->getRigidBody())
            {
                return fe::Vector2d(m_entity->getRigidBody()->getNormalForceX(), m_entity->getRigidBody()->getNormalForceY());
            }
        return fe::Vector2d();
    }

fe::Vector2d fe::scriptObject::scriptObjectGetForce()
    {
        if (m_entity->getRigidBody())
            {
                return m_entity->getRigidBody()->getForce();
            }
        return fe::Vector2d();
    }
