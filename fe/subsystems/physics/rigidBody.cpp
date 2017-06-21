#include "rigidBody.hpp"

fe::rigidBody::rigidBody(float mass) : m_mass(mass), m_maxSpeed(0.f), m_frictionCoeff(1.f)
    {
    }

fe::rigidBody::rigidBody(float mass, float maxSpeed) : m_mass(mass), m_maxSpeed(maxSpeed), m_frictionCoeff(1.f)
    {
    }

fe::rigidBody::rigidBody(float mass, float maxSpeed, float frictionCoeff) : m_mass(mass), m_maxSpeed(maxSpeed), m_frictionCoeff(frictionCoeff)
    {
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
        m_force += force;
    }


void fe::rigidBody::setVelocity(fe::Vector2d newVel)
    {
        m_velocity = newVel;
    }

void fe::rigidBody::setForce(fe::Vector2d newForce)
    {
        m_force = newForce;
    }

void fe::rigidBody::setDirection(fe::Vector2d newDirection)
    {
        m_velocity.x *= newDirection.normalize().x;
        m_velocity.y *= newDirection.normalize().y;
    }


fe::Vector2d fe::rigidBody::getVelocity() const
    {
        return m_velocity;
    }

fe::Vector2d fe::rigidBody::getForce() const
    {
        return m_force;
    }

fe::Vector2d fe::rigidBody::getDirection() const
    {
        return m_velocity.normalize();
    }


float fe::rigidBody::getSpeed() const
    {
        return m_velocity.magnitude();
    }

float fe::rigidBody::getTotalForce() const
    {
        return m_force.magnitude();
    }

float fe::rigidBody::getHeading() const
    {
        return m_velocity.normalize().magnitude();
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
void fe::rigidBody::update(fe::Vector2d accel, float deltaTime)
    {
        m_velocity += accel;

        m_velocity.clamp(m_maxSpeed);
        move(m_velocity * deltaTime);
    }