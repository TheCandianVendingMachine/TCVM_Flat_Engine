#include "rigidBody.hpp"

fe::rigidBody::rigidBody() : m_mass(1.f), m_maxSpeed(0.f), m_frictionCoeff(1.f) {}

fe::rigidBody::rigidBody(float mass) : m_mass(mass), m_maxSpeed(0.f), m_frictionCoeff(1.f)
    {
    }

fe::rigidBody::rigidBody(float mass, float maxSpeed) : m_mass(mass), m_maxSpeed(maxSpeed), m_frictionCoeff(1.f)
    {
    }

fe::rigidBody::rigidBody(float mass, float maxSpeed, float frictionCoeff) : m_mass(mass), m_maxSpeed(maxSpeed), m_frictionCoeff(frictionCoeff)
    {
    }

void fe::rigidBody::enable(bool value)
    {
        m_enabled = value;
    }

bool fe::rigidBody::getEnabled() const
    {
        return m_enabled;
    }

void fe::rigidBody::setMass(float newMass)
    {
        m_mass = newMass;
    }

void fe::rigidBody::setMaxSpeed(float maxSpeed)
    {
        m_maxSpeed = maxSpeed;
    }

void fe::rigidBody::setFrictionCoefficient(float fricCoeff)
    {
        m_frictionCoeff = fricCoeff;
    }


// applies a force upon the rigid body. 
void fe::rigidBody::applyForce(fe::Vector2d force)
    {
        if (!m_enabled) return;
        m_forceX += force.x;
        m_forceY += force.y;
    }


void fe::rigidBody::setVelocity(fe::Vector2d newVel)
    {
        m_velocityX = newVel.x;
        m_velocityY = newVel.y;
    }

void fe::rigidBody::setForce(fe::Vector2d newForce)
    {
        m_forceX = newForce.x;
        m_forceY = newForce.y;
    }

void fe::rigidBody::setDirection(fe::Vector2d newDirection)
    {
        if (!m_enabled) return;

        m_velocityX *= newDirection.normalize().x;
        m_velocityY *= newDirection.normalize().y;
    }


fe::Vector2d fe::rigidBody::getVelocity() const
    {
        return { m_velocityX, m_velocityX };
    }

fe::Vector2d fe::rigidBody::getForce() const
    {
        return { m_forceX, m_forceY };
    }

fe::Vector2d fe::rigidBody::getDirection() const
    {
        return fe::Vector2d(m_velocityX, m_velocityY).normalize();
    }


float fe::rigidBody::getSpeed() const
    {
        return fe::Vector2d(m_velocityX, m_velocityY).magnitude();
    }

float fe::rigidBody::getTotalForce() const
    {
        return fe::Vector2d(m_forceX, m_forceY).magnitude();
    }

float fe::rigidBody::getHeading() const
    {
        return fe::Vector2d(m_velocityX, m_velocityY).normalize().magnitude();
    }


float fe::rigidBody::getMass() const
    {
        return m_mass;
    }

float fe::rigidBody::getMaxVelocity() const
    {
        return m_maxSpeed;
    }

float fe::rigidBody::getFrictionCoefficient() const
    {
        return m_frictionCoeff;
    }

// Updates position of object based on force
void fe::rigidBody::update(float acellX, float acellY, float deltaTime)
    {
        if (!m_enabled) return;

        m_velocityX += acellX;
        m_velocityY += acellY;

        if (m_maxSpeed * m_maxSpeed > m_velocityX * m_velocityX + m_velocityY * m_velocityY) 
            {
                float modifier = std::sqrt((m_maxSpeed * m_maxSpeed) / (m_velocityX * m_velocityX + m_velocityY * m_velocityY));
                m_velocityX *= modifier;
                m_velocityY *= modifier;
            }

        move(m_velocityX * deltaTime, m_velocityY * deltaTime);
    }