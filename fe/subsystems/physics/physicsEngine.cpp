#include "physicsEngine.hpp"
#include "../../typeDefines.hpp"
#include "../../math/Vector2.hpp"

fe::physicsEngine::physicsEngine() : m_gravityForce(0.f, 0.f)
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

void fe::physicsEngine::preUpdate()
    {
        
    }

void fe::physicsEngine::simulateForces(float deltaTime, unsigned int iterations)
    {
        for (unsigned int i = 0; i < m_rigidBodies.getObjectAllocCount(); i++)
            {
                fe::rigidBody *body = m_rigidBodies.at(i);
                if (body)
                    {
                        float frictionForceX = m_gravityForce.x * body->getFrictionCoefficient();
                        float frictionForceY = m_gravityForce.y * body->getFrictionCoefficient();

                        float accelX = (body->getForce().x + frictionForceX) / body->getMass();
                        float accelY = (body->getForce().y + frictionForceY) / body->getMass();

                        for (int j = 0; j < iterations; j++)
                            {
                                body->update(accelX, accelY, deltaTime);
                            }
                    }
            }
    }

fe::rigidBody *fe::physicsEngine::createRigidBody()
    {
        return m_rigidBodies.alloc();
    }

void fe::physicsEngine::deleteRigidBody(fe::rigidBody *body)
    {
        m_rigidBodies.free(body);
    }
