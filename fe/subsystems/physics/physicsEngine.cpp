#include "physicsEngine.hpp"
#include "../../typeDefines.hpp"
#include "../../math/Vector2.hpp"
#include "../../engine.hpp"
#include "../threading/threadPool.hpp"
#include "../../debug/profiler.hpp"
#include "../../math/mathFuncs.hpp"

fe::physicsEngine::physicsEngine() :
    m_gravityForceX(0.f),
    m_gravityForceY(0.f),
    m_gravityForceZ(10.f),
    m_maxObjectsUntilThread(5000) // 5000 = ~when intersection of FPS occurs between threaded physics and not
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
        m_gravityForceX = gravity.x;
        m_gravityForceY = gravity.y;
    }

fe::Vector2d fe::physicsEngine::getGravity() const
    {
        return fe::Vector2d(m_gravityForceX, m_gravityForceY);
    }

void fe::physicsEngine::preUpdate(float deltaTime, unsigned int iterations)
    {

    }

void fe::physicsEngine::simulateForces(float deltaTime, unsigned int iterations)
    {
        for (unsigned int i = 0; i < m_rigidBodies.getObjectAllocCount(); i++)
            {
                fe::rigidBody *body = m_rigidBodies.at(i);
                // taking advantage of branch prediction as for the CPU assumes that this statement will be false
                if (!body)
                    {
                    }    
                else if (body->getEnabled())
                    {
                        float bodyTotalForce = body->getTotalForce() + 0.00001f;

                        float normalForceX = abs(body->getMass() * m_gravityForceX);
                        float normalForceY = abs(body->getMass() * m_gravityForceY);
                        float normalForceZ = abs(body->getMass() * m_gravityForceZ);

                        float friction = body->getFrictionCoefficient() * normalForceZ;

                        float forceX = normalForceX;
                        float forceY = normalForceY;

                        forceX += (-friction * (body->getForceX() / bodyTotalForce));
                        forceY += (-friction * (body->getForceY() / bodyTotalForce));

                        for (unsigned int j = 0; j < iterations; j++)
                            {
                                body->applyForce(forceX, forceY);
                                body->update(deltaTime);
                            }
                    }
            }
    }

fe::rigidBody *fe::physicsEngine::createRigidBody()
    {
        fe::rigidBody *allocated = m_rigidBodies.alloc();
        return allocated;
    }

void fe::physicsEngine::deleteRigidBody(fe::rigidBody *body)
    {
        m_rigidBodies.free(body);
    }
