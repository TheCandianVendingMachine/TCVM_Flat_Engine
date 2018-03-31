#include "physicsEngine.hpp"
#include "../../typeDefines.hpp"
#include "../../math/Vector2.hpp"
#include "../../engine.hpp"
#include "../threading/threadPool.hpp"
#include "../../debug/profiler.hpp"
#include "../../math/mathFuncs.hpp"
#include "../../engineEvents.hpp"
#include "collision/collisionBody.hpp"
#include "../../entity/baseEntity.hpp"
#include "../messaging/gameEvent.hpp"
#include "../messaging/eventSender.hpp"
#include "../../debug/debugDraw.hpp"

void fe::physicsEngine::handleCollision(fe::collider *a, fe::collider *b, fe::collisionData aData, fe::collisionData bData)
    {
        fe::baseEntity *aEnt = static_cast<fe::baseEntity*>(a->m_owner);

        fe::Vector2d lineStart(aData.m_colliderPositionX, aData.m_colliderPositionY);
        fe::Vector2d lineEnd = lineStart + fe::lightVector2d(aData.m_normalX * 50, aData.m_normalY * 50);

        FE_DEBUG_DRAW_LINE(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, sf::Color::White);
        if (aEnt)
            {
                fe::rigidBody *aRigid = aEnt->getRigidBody();
                if (aRigid)
                    {
                        fe::Vector2d normal(aData.m_normalX, aData.m_normalY);
                        fe::Vector2d force(aRigid->getForce());
                        fe::lightVector2d normalForce = force.project(normal);
                        aRigid->setNormalForce(normalForce.x, normalForce.y);

                        if (std::abs(aData.m_penetrationX) < std::abs(aData.m_penetrationY))
                            {
                                aEnt->setPosition(aEnt->getPosition() + fe::lightVector2d(aData.m_penetrationX, 0));
                                if (aData.m_penetrationX < 0.f)
                                    {
                                        aRigid->setVelocity(std::min(aRigid->getVelocityX(), 0.f), aRigid->getVelocityY());
                                        aRigid->setForce(std::min(aRigid->getForceX(), 0.f), aRigid->getForceY());
                                    }
                                else
                                    {
                                        aRigid->setVelocity(std::max(aRigid->getVelocityX(), 0.f), aRigid->getVelocityY());
                                        aRigid->setForce(std::max(aRigid->getForceX(), 0.f), aRigid->getForceY());
                                    }
                            }
                        else 
                            {
                                aEnt->setPosition(aEnt->getPosition() + fe::lightVector2d(0, aData.m_penetrationY));
                                if (aData.m_penetrationY < 0.f)
                                    {
                                        aRigid->setVelocity(aRigid->getVelocityX(), std::min(aRigid->getVelocityY(), 0.f));
                                        aRigid->setForce(aRigid->getForceX(), std::min(aRigid->getForceY(), 0.f));
                                    }
                                else
                                    {
                                        aRigid->setVelocity(aRigid->getVelocityX(), std::max(aRigid->getVelocityY(), 0.f));
                                        aRigid->setForce(aRigid->getForceX(), std::max(aRigid->getForceY(), 0.f));
                                    }
                            }
                    }
            }

        fe::baseEntity *bEnt = static_cast<fe::baseEntity*>(b->m_owner);
        if (bEnt)
            {
                
            }
    }

fe::physicsEngine::physicsEngine() :
    m_gravityForceX(0.f),
    m_gravityForceY(0.f),
    m_gravityForceZ(0.f),
    m_maxObjectsUntilThread(5000) // 5000 = ~when intersection of FPS occurs between threaded physics and not
    {
    }

void fe::physicsEngine::handleEvent(const gameEvent &event)
    {
        switch (event.eventType)
            {
                case COLLISION:
                    handleCollision(static_cast<fe::collider*>(event.args[0].arg.TYPE_VOIDP), static_cast<fe::collider*>(event.args[1].arg.TYPE_VOIDP),
                                    *static_cast<fe::collisionData*>(event.args[2].arg.TYPE_VOIDP), *static_cast<fe::collisionData*>(event.args[3].arg.TYPE_VOIDP));
                    break;
                default:
                    break;
            }
    }

void fe::physicsEngine::startUp()
    {
        m_rigidBodies.startUp(FE_MAX_GAME_OBJECTS);
        fe::engine::get().getEventSender().subscribe(this, COLLISION);
    }

void fe::physicsEngine::shutDown()
    {
        m_rigidBodies.clear();
        fe::engine::get().getEventSender().unsubscribeAll(this);
    }

void fe::physicsEngine::clear()
    {
        m_rigidBodies.clear();
    }

void fe::physicsEngine::setGravity(fe::Vector3d gravity)
    {
        m_gravityForceX = gravity.x;
        m_gravityForceY = gravity.y;
        m_gravityForceZ = gravity.z;
    }

void fe::physicsEngine::setGravityX(float gravity)
    {
        m_gravityForceX = gravity;
    }

void fe::physicsEngine::setGravityY(float gravity)
    {
        m_gravityForceY = gravity;
    }

void fe::physicsEngine::setGravityZ(float gravity)
    {
        m_gravityForceZ = gravity;
    }

fe::Vector3d fe::physicsEngine::getGravity() const
    {
        return fe::Vector3d(m_gravityForceX, m_gravityForceY, m_gravityForceZ);
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

                        float normalForceX = abs(body->getMass() * body->getNormalForceX());
                        float normalForceY = abs(body->getMass() * body->getNormalForceY());
                        const float normalForceZ = abs(body->getMass() * m_gravityForceZ);

                        float frictionY = body->getFrictionCoefficient() * normalForceX; // since friction = friction coef * force being pushed into wall we flip these
                        float frictionX = body->getFrictionCoefficient() * normalForceY;
                        float frictionZ = body->getFrictionCoefficient() * normalForceZ;

                        float forceX = body->getMass() * m_gravityForceX;
                        float forceY = body->getMass() * m_gravityForceY;

                        const float bodyUnitForceX = body->getForceX() / bodyTotalForce;
                        const float bodyUnitForceY = body->getForceY() / bodyTotalForce;

                        float frictionForceX = (-frictionZ * bodyUnitForceX) + (-frictionX * bodyUnitForceX);
                        float frictionForceY = (-frictionZ * bodyUnitForceY) + (-frictionY * bodyUnitForceY);

                        if (frictionForceX > 0.f && abs(frictionForceX + forceX) > abs(body->getForceX()))
                            {
                                frictionForceX = abs(body->getForceX()) - forceX;
                            }

                        if (frictionForceY > 0.f && abs(frictionForceY + forceY) > abs(body->getForceY()))
                            {
                                frictionForceY = abs(body->getForceY()) - forceY;
                            }

                        forceX += frictionForceX;
                        forceY += frictionForceY;

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
