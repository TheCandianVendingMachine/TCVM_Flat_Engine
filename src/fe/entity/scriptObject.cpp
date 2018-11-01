#include "fe/entity/scriptObject.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/subsystems/physics/rigidBody.hpp"
#include "fe/subsystems/graphic/renderObject/renderObject.hpp"
#include "fe/subsystems/graphic/animation/animationActor.hpp"
#include "fe/objectManagement/str.hpp"

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

void fe::scriptObject::scriptObjectSetVelocity(fe::Vector2d velocity)
    {
        scriptObjectSetVelocity(velocity.x, velocity.y);
    }

void fe::scriptObject::scriptObjectSetVelocity(float x, float y)
    {
        if (m_entity->getRigidBody())
            {
                m_entity->getRigidBody()->setVelocity(x, y);
            }
    }

void fe::scriptObject::scriptObjectSetForce(fe::Vector2d force)
    {
        scriptObjectSetForce(force.x, force.y);
    }

void fe::scriptObject::scriptObjectSetForce(float x, float y)
    {
        if (m_entity->getRigidBody())
            {
                m_entity->getRigidBody()->setForce(x, y);
            }
    }

void fe::scriptObject::scriptObjectApplyForce(fe::Vector2d force)
    {
        scriptObjectApplyForce(force.x, force.y);
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

void fe::scriptObject::scriptObjectSetColour(int r, int g, int b, int a)
    {
        m_entity->setColour(sf::Color(r, g, b, a));
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

fe::Vector2d fe::scriptObject::scriptObjectGetDirection()
    {
        if (m_entity->getRigidBody())
            {
                return m_entity->getRigidBody()->getDirection();
            }
        return fe::Vector2d();
    }

void fe::scriptObject::scriptObjectMoveToPosition(fe::Vector2d pos)
    {
        scriptObjectMoveToPosition(pos.x, pos.y);
    }

void fe::scriptObject::scriptObjectMoveToPosition(float x, float y)
    {
        m_entity->getUtilities().moveToPosition(x, y);
    }

float fe::scriptObject::scriptObjectGetSlowdownDistance()
    {
        return m_entity->getUtilities().getSlowdownDistance();
    }

void fe::scriptObject::scriptObjectSetSlowdownDistance(float distance)
    {
        m_entity->getUtilities().setSlowdownDistance(distance);
    }

bool fe::scriptObject::scriptObjectHasArrived()
    {
        return m_entity->getUtilities().getArrived();
    }

bool fe::scriptObject::scriptObjectIsTargeted()
    {
        return m_entity->getUtilities().isTargeted();
    }

void fe::scriptObject::scriptObjectSetAnimationFrame(unsigned int x, unsigned int y)
    {
        if (m_entity->getActor())
            {
                m_entity->getActor()->setCurrentFrame(x, y);
            }
    }

void fe::scriptObject::scriptObjectPlayAnimationSequence(const char *sequence)
    {
        if (m_entity->getActor())
            {
                m_entity->getActor()->playSequence(FE_STR(sequence));
            }
    }

bool fe::scriptObject::scriptObjectAnimationPlayingSequence() const
    {
        if (m_entity->getActor())
            {
                return m_entity->getActor()->isPlayingSequence();
            }
        return false;
    }

bool fe::scriptObject::scriptObjectAnimationPlaying() const
    {
        if(m_entity->getActor())
            {
                return m_entity->getActor()->isPlaying();
            }
        return false;
    }

