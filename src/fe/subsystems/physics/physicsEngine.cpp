#include "fe/subsystems/physics/physicsEngine.hpp"
#include "fe/subsystems/messaging/gameEvent.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include "fe/subsystems/collision/collisionBody.hpp"
#include "fe/subsystems/threading/threadPool.hpp"
#include "fe/typeDefines.hpp"
#include "fe/math/Vector2.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/math/signOf.hpp"
#include "fe/engineEvents.hpp"
#include "fe/entity/baseEntity.hpp"
#include "fe/debug/debugDraw.hpp"
#include "fe/engine.hpp"

void fe::physicsEngine::handleCollision(fe::collider *a, fe::collider *b, fe::collisionData aData, fe::collisionData bData)
    {
        fe::baseEntity *aEnt = static_cast<fe::baseEntity*>(a->m_owner);

        fe::Vector2d lineStart(aData.m_colliderPositionX, aData.m_colliderPositionY);
        fe::Vector2d lineEnd = lineStart + fe::lightVector2d(aData.m_normalX * 50, aData.m_normalY * 50);

        if (aEnt && b->m_solid)
            {
                FE_DEBUG_DRAW_LINE(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, sf::Color::White);
                fe::rigidBody *aRigid = aEnt->getRigidBody();
                if (aRigid)
                    {
                        fe::Vector2d normal(aData.m_normalX, aData.m_normalY);
                        fe::Vector2d force(aRigid->getForce());
                        fe::lightVector2d normalForce = force.project(normal);
                        aRigid->applyNormalForce(normalForce.x, normalForce.y);

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
    }

fe::physicsEngine::physicsEngine() :
    m_gravityForceX(0.f),
    m_gravityForceY(0.f),
    m_gravityForceZ(0.f),
    m_frictionImpulse(3.f),
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

void fe::physicsEngine::setFrictionImpulse(float friction)
    {
        m_frictionImpulse = friction;
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
                        float bodyMass = body->getMass();

                        const float frictionCoef = body->getFrictionCoefficient();
                        float frictionForceX = frictionCoef * body->getNormalForceY();
                        float frictionForceY = frictionCoef * body->getNormalForceX();
                        float frictionForceZ = frictionCoef * m_gravityForceZ;

                        float forceX = 0.f;
                        float forceY = 0.f;
                        fe::lightVector2d frictionForceZVec(0, 0);
                        fe::Vector2d bodyVelocityNormal(0, 0);

                        for (unsigned int j = 0; j < iterations; j++)
                            {
                                //bodyVelocityNormal = ;
                                float bodyVelocityX = body->getVelocityX();
                                float bodyVelocityY = body->getVelocityY();

                                forceX = m_gravityForceX + frictionForceX * fe::signOf(bodyVelocityX);
                                forceY = m_gravityForceY + frictionForceY * fe::signOf(bodyVelocityY);

                                frictionForceZVec = -body->getVelocity().normalize() * frictionForceZ;
                                forceX += frictionForceZVec.x;
                                forceY += frictionForceZVec.y;

                                // (Old - New) because we are reducing velocity so the old velocity will be lower
                                if (std::abs(bodyVelocityX) - std::abs(bodyVelocityX + ((forceX / bodyMass) * deltaTime)) < 0.f)
                                    {
                                        // flipped X via friction
                                        // a = (f/m)*d
                                        // a = -v
                                        // -v = (f/m)*d
                                        // (-v/d) = f/m
                                        // (-v/d)*m = f
                                        forceX = (-bodyVelocityX / deltaTime) * bodyMass;
                                    }

                                if (std::abs(bodyVelocityY) - std::abs(bodyVelocityY + ((forceY / bodyMass) * deltaTime)) < 0.f)
                                    {
                                        // flipped Y via friction
                                        forceY = (-bodyVelocityY / deltaTime) * bodyMass;
                                    }

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
