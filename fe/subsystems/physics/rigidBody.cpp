#include "rigidBody.hpp"

float fe::rigidBody::getVelocityX() const
    {
        return m_velocityX;
    }

float fe::rigidBody::getVelocityY() const
    {
        return m_velocityY;
    }

float fe::rigidBody::getForceX() const
    {
        return m_forceX;
    }

float fe::rigidBody::getForceY() const
    {
        return m_forceY;
    }

float fe::rigidBody::getPositionX() const
    {
        return m_positionX;
    }

float fe::rigidBody::getPositionY() const
    {
        return m_positionY;
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

fe::Vector2d fe::rigidBody::getVelocity() const
    {
        return { m_velocityX, m_velocityY };
    }

fe::Vector2d fe::rigidBody::getForce() const
    {
        return { m_forceX, m_forceY };
    }

fe::Vector2d fe::rigidBody::getDirection() const
    {
        return fe::Vector2d(m_velocityX, m_velocityY).normalize();
    }

fe::Vector2d fe::rigidBody::getPosition() const
    {
        return fe::Vector2d(m_positionX, m_positionY);
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
void fe::rigidBody::applyForce(float x, float y)
    {
        if (!m_enabled) return;
        m_impulseX += x;
        m_impulseY += y;
    }


void fe::rigidBody::setVelocity(float x, float y)
    {
        m_velocityX = x;
        m_velocityY = y;
    }

void fe::rigidBody::setForce(float x, float y)
    {
        m_forceX = x;
        m_forceY = y;
    }

void fe::rigidBody::setDirection(float x, float y)
    {
        if (!m_enabled) return;

        m_velocityX = getSpeed() * fe::Vector2d(x, y).normalize().x;
        m_velocityY = getSpeed() *fe::Vector2d(x, y).normalize().y;
    }

fe::rigidBody::rigidBody() : m_mass(1.f), m_maxSpeed(0.f), m_frictionCoeff(1.f), m_enabled(true) {}

fe::rigidBody::rigidBody(float mass) : m_mass(mass), m_maxSpeed(0.f), m_frictionCoeff(1.f), m_enabled(true)
    {
    }

fe::rigidBody::rigidBody(float mass, float maxSpeed) : m_mass(mass), m_maxSpeed(maxSpeed), m_frictionCoeff(1.f), m_enabled(true)
    {
    }

fe::rigidBody::rigidBody(float mass, float maxSpeed, float frictionCoeff) : m_mass(mass), m_maxSpeed(maxSpeed), m_frictionCoeff(frictionCoeff), m_enabled(true)
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