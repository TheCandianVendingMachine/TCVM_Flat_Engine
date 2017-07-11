#include "physicsEngine.hpp"
#include "../../typeDefines.hpp"
#include "../../math/Vector2.hpp"
#include "../../engine.hpp"
#include "../threading/threadPool.hpp"
#include "../../debug/profiler.hpp"

fe::physicsEngine::physicsEngine() :
    m_gravityForceX(9.81f),
    m_gravityForceY(9.81f),
    m_jobA(m_rigidBodies, m_gravityForceX, m_gravityForceY),
    m_jobB(m_rigidBodies, m_gravityForceX, m_gravityForceY),
    m_jobC(m_rigidBodies, m_gravityForceX, m_gravityForceY),
    m_jobD(m_rigidBodies, m_gravityForceX, m_gravityForceY),
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
        if (m_rigidBodies.getObjectAllocCount() <= m_maxObjectsUntilThread)
            {
            }
        else
            {
                m_jobA.m_iterations = iterations; m_jobA.m_deltaTime = deltaTime;
                m_jobB.m_iterations = iterations; m_jobB.m_deltaTime = deltaTime;
                m_jobC.m_iterations = iterations; m_jobC.m_deltaTime = deltaTime;
                m_jobD.m_iterations = iterations; m_jobD.m_deltaTime = deltaTime;

                fe::engine::get().getThreadPool().addJob(m_jobA);
                fe::engine::get().getThreadPool().addJob(m_jobB);
                fe::engine::get().getThreadPool().addJob(m_jobC);
                fe::engine::get().getThreadPool().addJob(m_jobD);
            }
    }

void fe::physicsEngine::simulateForces(float deltaTime, unsigned int iterations)
    {
        if (m_rigidBodies.getObjectAllocCount() <= m_maxObjectsUntilThread)
            {
                for (unsigned int i = 0; i < m_rigidBodies.getObjectAllocCount(); i++)
                    {
                        fe::rigidBody *body = m_rigidBodies.at(i);
                        // taking advantage of branch prediction as for the CPU assumes that this statement will be false
                        if (!body)
                            {
                            }    
                        else
                            {
                                float forceX = (-body->getFrictionCoefficient() * body->getForceX());
                                float forceY = (-body->getFrictionCoefficient() * body->getForceY());

                                //if (abs(body->getForce().x) - abs(forceX) < 0.f) body->setForce(0.f, body->getForce().y);
                                //if (abs(body->getForce().y) - abs(forceY) < 0.f) body->setForce(body->getForce().x, 0.f);

                                for (int j = 0; j < iterations; j++)
                                    {
                                        body->applyForce(forceX, forceY);
                                        body->update(deltaTime);
                                    }
                            }
                    }
            }
        else
            {
                fe::engine::get().getThreadPool().waitFor(m_jobA);
                fe::engine::get().getThreadPool().waitFor(m_jobB);
                fe::engine::get().getThreadPool().waitFor(m_jobC);
                fe::engine::get().getThreadPool().waitFor(m_jobD);
            }
    }

fe::rigidBody *fe::physicsEngine::createRigidBody()
    {
        fe::rigidBody *allocated = m_rigidBodies.alloc();
        unsigned int allocCount = m_rigidBodies.getObjectAllocCount();
        if (allocCount < 4)
            {
                if (allocCount < 4)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = true;
                        m_jobC.m_active = true;
                        m_jobD.m_active = false;
                    }
                if (allocCount < 3)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = true;
                        m_jobC.m_active = false;

                    }
                if (allocCount < 2)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = false;
                    }
                if (allocCount < 1)
                    {
                        m_jobA.m_active = false;
                    }

                unsigned int objectCountQuart = allocCount;
                m_jobA.m_initialIndex = 0 * objectCountQuart; m_jobA.m_endIndex = objectCountQuart * 1;
                m_jobB.m_initialIndex = 1 * objectCountQuart; m_jobB.m_endIndex = objectCountQuart * 2;
                m_jobC.m_initialIndex = 2 * objectCountQuart; m_jobC.m_endIndex = objectCountQuart * 3;
                m_jobD.m_initialIndex = 3 * objectCountQuart; m_jobD.m_endIndex = objectCountQuart * 4;
            }
        else
            {
                m_jobA.m_active = true;
                m_jobB.m_active = true;
                m_jobC.m_active = true;
                m_jobD.m_active = true;

                unsigned int objectCountQuart = allocCount / 4;
                m_jobA.m_initialIndex = 0 * objectCountQuart; m_jobA.m_endIndex = objectCountQuart * 1;
                m_jobB.m_initialIndex = 1 * objectCountQuart; m_jobB.m_endIndex = objectCountQuart * 2;
                m_jobC.m_initialIndex = 2 * objectCountQuart; m_jobC.m_endIndex = objectCountQuart * 3;
                m_jobD.m_initialIndex = 3 * objectCountQuart; m_jobD.m_endIndex = objectCountQuart * 4;
            }
        return allocated;
    }

void fe::physicsEngine::deleteRigidBody(fe::rigidBody *body)
    {
        m_rigidBodies.free(body);
        unsigned int allocCount = m_rigidBodies.getObjectAllocCount();
        if (allocCount < 4)
            {
                if (allocCount < 4)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = true;
                        m_jobC.m_active = true;
                        m_jobD.m_active = false;
                    }
                if (allocCount < 3)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = true;
                        m_jobC.m_active = false;

                    }
                if (allocCount < 2)
                    {
                        m_jobA.m_active = true;
                        m_jobB.m_active = false;
                    }
                if (allocCount < 1)
                    {
                        m_jobA.m_active = false;
                    }

                unsigned int objectCountQuart = allocCount;
                m_jobA.m_initialIndex = 0 * objectCountQuart; m_jobA.m_endIndex = objectCountQuart * 1;
                m_jobB.m_initialIndex = 1 * objectCountQuart; m_jobB.m_endIndex = objectCountQuart * 2;
                m_jobC.m_initialIndex = 2 * objectCountQuart; m_jobC.m_endIndex = objectCountQuart * 3;
                m_jobD.m_initialIndex = 3 * objectCountQuart; m_jobD.m_endIndex = objectCountQuart * 4;
            }
        else
            {
                m_jobA.m_active = true;
                m_jobB.m_active = true;
                m_jobC.m_active = true;
                m_jobD.m_active = true;

                unsigned int objectCountQuart = allocCount / 4;
                m_jobA.m_initialIndex = 0 * objectCountQuart; m_jobA.m_endIndex = objectCountQuart * 1;
                m_jobB.m_initialIndex = 1 * objectCountQuart; m_jobB.m_endIndex = objectCountQuart * 2;
                m_jobC.m_initialIndex = 2 * objectCountQuart; m_jobC.m_endIndex = objectCountQuart * 3;
                m_jobD.m_initialIndex = 3 * objectCountQuart; m_jobD.m_endIndex = objectCountQuart * 4;
            }
    }

fe::physicsEngine::physicsJob::physicsJob(fe::poolAllocater<rigidBody> &rigidBodies, float &gravityX, float &gravityY) :
    m_gravityX(gravityX),
    m_gravityY(gravityY),
    m_rigidBodies(rigidBodies)
    {
    }

bool fe::physicsEngine::physicsJob::execute()
    {
        for (unsigned int i = m_initialIndex; i < m_endIndex; i++)
            {
                fe::rigidBody *body = m_rigidBodies.at(i);
                // taking advantage of branch prediction as for the CPU assumes that this statement will be false
                if (!body)
                    {
                    }    
                else
                    {
                        float forceX = (-body->getFrictionCoefficient() * body->getForceX());
                        float forceY = (-body->getFrictionCoefficient() * body->getForceY());

                        //if (abs(body->getForce().x) - abs(forceX) < 0.f) body->setForce(0.f, body->getForce().y);
                        //if (abs(body->getForce().y) - abs(forceY) < 0.f) body->setForce(body->getForce().x, 0.f);

                        for (int j = 0; j < m_iterations; j++)
                            {
                                body->applyForce(forceX, forceY);
                                body->update(m_deltaTime);
                            }
                    }
            }

        return true;
    }
