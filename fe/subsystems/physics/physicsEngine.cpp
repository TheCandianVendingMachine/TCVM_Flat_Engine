#include "physicsEngine.hpp"
#include "../../typeDefines.hpp"

fe::physicsEngine::physicsEngine() : m_gravityForce(1.f)
    {
    }

void fe::physicsEngine::startUp()
    {
        m_rigidBodies.startUp(FE_MAX_GAME_OBJECTS);
    }

void fe::physicsEngine::shutDown()
    {
        m_rigidBodies.clear();
    }

void fe::physicsEngine::clear()
    {
        m_rigidBodies.clear();
    }

void fe::physicsEngine::setGravity(fe::Vector2d gravity)
    {
        m_gravityForce = gravity;
    }

fe::Vector2d fe::physicsEngine::getGravity() const
    {
        return m_gravityForce;
    }

void fe::physicsEngine::simulateForces(float deltaTime)
    {
        for (unsigned int i = 0; i < FE_MAX_GAME_OBJECTS; i++)
            {
                fe::rigidBody *body = m_rigidBodies.at(i);

                fe::Vector2d accelForce = body->getForce() / body->getMass();
                fe::Vector2d frictionForce = body->getFrictionCoefficient() * m_gravityForce;
                
                fe::Vector2d acceleration = (accelForce + frictionForce) * body->getMass();
                body->update(acceleration, deltaTime);
            }
    }
