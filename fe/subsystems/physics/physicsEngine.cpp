#include "physicsEngine.hpp"
#include "../../typeDefines.hpp"
#include "../../math/Vector2.hpp"
#include "../../engine.hpp"
#include "../threading/threadPool.hpp"
#include "../../debug/profiler.hpp"

fe::physicsEngine::physicsEngine() :
    m_gravityForceX(0.f),
    m_gravityForceY(0.f),
    jobA(m_rigidBodies, m_gravityForceX, m_gravityForceY),
    jobB(m_rigidBodies, m_gravityForceX, m_gravityForceY),
    jobC(m_rigidBodies, m_gravityForceX, m_gravityForceY),
    jobD(m_rigidBodies, m_gravityForceX, m_gravityForceY)
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
        unsigned int objectQuart = m_rigidBodies.getObjectAllocCount() / 4;
        jobA.m_initialIndex = objectQuart * 0; jobA.m_endIndex = objectQuart * 1; jobA.m_iterations = iterations; jobA.m_deltaTime = deltaTime;
        jobB.m_initialIndex = objectQuart * 1; jobB.m_endIndex = objectQuart * 2; jobB.m_iterations = iterations; jobB.m_deltaTime = deltaTime;
        jobC.m_initialIndex = objectQuart * 2; jobC.m_endIndex = objectQuart * 3; jobC.m_iterations = iterations; jobC.m_deltaTime = deltaTime;
        jobD.m_initialIndex = objectQuart * 3; jobD.m_endIndex = objectQuart * 4; jobD.m_iterations = iterations; jobD.m_deltaTime = deltaTime;

        FE_PROFILE("physics_add_job");
        fe::engine::get().getThreadPool().addJob(jobA);
        fe::engine::get().getThreadPool().addJob(jobB);
        fe::engine::get().getThreadPool().addJob(jobC);
        fe::engine::get().getThreadPool().addJob(jobD);
        FE_END_PROFILE;
    }

void fe::physicsEngine::simulateForces()
    {
        FE_PROFILE("physics_wait_job");
        fe::engine::get().getThreadPool().waitFor(jobA);
        fe::engine::get().getThreadPool().waitFor(jobB);
        fe::engine::get().getThreadPool().waitFor(jobC);
        fe::engine::get().getThreadPool().waitFor(jobD);
        FE_END_PROFILE;
    }

fe::rigidBody *fe::physicsEngine::createRigidBody()
    {
        return m_rigidBodies.alloc();
    }

void fe::physicsEngine::deleteRigidBody(fe::rigidBody *body)
    {
        m_rigidBodies.free(body);
    }

fe::physicsEngine::physicsJob::physicsJob(fe::poolAllocater<rigidBody> &rigidBodies, float gravityX, float gravityY) :
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
                if (body)
                    {
                        float frictionForceX = m_gravityX * body->getFrictionCoefficient();
                        float frictionForceY = m_gravityY * body->getFrictionCoefficient();

                        float accelX = (body->getForce().x + frictionForceX) / body->getMass();
                        float accelY = (body->getForce().y + frictionForceY) / body->getMass();

                        for (int j = 0; j < m_iterations; j++)
                            {
                                body->update(accelX, accelY, m_deltaTime);
                            }
                    }
            }

        return true;
    }
