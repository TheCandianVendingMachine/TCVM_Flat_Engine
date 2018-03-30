#include "collisionWorld.hpp"
#include "../../../debug/profiler.hpp"
#include "../../../engine.hpp"
#include "../../threading/threadPool.hpp"
#include "../../messaging/gameEvent.hpp"
#include "../../messaging/eventSender.hpp"
#include "../../../engineEvents.hpp"
#include "aabbTree.hpp"
#include <functional>

void fe::collisionWorld::handleCollision(fe::collider *a, fe::collider *b)
    {
        FE_ENGINE_PROFILE("collision_world", "collision_check");
        fe::AABB *first = static_cast<fe::AABB*>(&a->m_aabb);
        fe::AABB *second = static_cast<fe::AABB*>(&b->m_aabb);

        fe::collisionData dataFirst;
        fe::collisionData dataSecond;

        fe::lightVector2d positionA(first->m_globalPositionX, first->m_globalPositionY);
        fe::lightVector2d sizeA(first->m_sizeX, first->m_sizeY);

        fe::lightVector2d positionB(second->m_globalPositionX, second->m_globalPositionY);
        fe::lightVector2d sizeB(second->m_sizeX, second->m_sizeY);

        if (((sizeA.x + positionA.x >= positionB.x && positionA.x < sizeB.x + positionB.x) &&
             (sizeA.y + positionA.y >= positionB.y && positionA.y < sizeB.y + positionB.y)) || 
            ((sizeB.x + positionB.x >= positionA.x && positionB.x < sizeA.x + positionA.x) &&
             (sizeB.y + positionB.y >= positionA.y && positionB.y < sizeA.y + positionA.y)))
            {
                fe::Vector2d centerFirst(positionA.x + (sizeA.x / 2),
                                         positionA.y + (sizeA.y / 2));

                fe::Vector2d centerSecond(positionB.x + (sizeB.x / 2),
                                          positionB.y + (sizeB.y / 2));

                fe::lightVector2d distance(centerFirst - centerSecond);
                fe::lightVector2d minDistance((sizeA.x / 2) + (sizeB.x / 2),
                                              (sizeA.y / 2) + (sizeB.y / 2));
               
                dataFirst.m_penetrationX = (distance.x > 0 ? minDistance.x - distance.x : -minDistance.x - distance.x) / 2.f;
                dataFirst.m_penetrationY = (distance.y > 0 ? minDistance.y - distance.y : -minDistance.y - distance.y) / 2.f;
                dataFirst.m_colliderPositionX = a->m_aabb.m_globalPositionX;
                dataFirst.m_colliderPositionY = a->m_aabb.m_globalPositionY;

                dataSecond.m_penetrationX = -(distance.x > 0 ? minDistance.x - distance.x : -minDistance.x - distance.x) / 2.f;
                dataSecond.m_penetrationY = -(distance.y > 0 ? minDistance.y - distance.y : -minDistance.y - distance.y) / 2.f;
                dataSecond.m_colliderPositionX = b->m_aabb.m_globalPositionX;
                dataSecond.m_colliderPositionY = b->m_aabb.m_globalPositionY;

                a->m_collisionCallback(dataFirst);
                b->m_collisionCallback(dataSecond);

                if (a->m_eventOnCollision == b->m_eventOnCollision)
                    {
                        fe::gameEvent collisionEvent(a->m_eventOnCollision, 2);
                        collisionEvent.args[0].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                        collisionEvent.args[1].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                        collisionEvent.args[0].arg.TYPE_VOIDP = a;
                        collisionEvent.args[1].arg.TYPE_VOIDP = b;
                        fe::engine::get().getEventSender().sendEngineEvent(collisionEvent, a->m_eventOnCollision);
                    }
                else
                    {
                        fe::gameEvent collisionEventLeft(a->m_eventOnCollision, 2);
                        collisionEventLeft.args[0].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                        collisionEventLeft.args[1].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                        collisionEventLeft.args[0].arg.TYPE_VOIDP = a;
                        collisionEventLeft.args[1].arg.TYPE_VOIDP = b;

                        fe::gameEvent collisionEventRight(b->m_eventOnCollision, 2);
                        collisionEventRight.args[0].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                        collisionEventRight.args[1].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                        collisionEventRight.args[0].arg.TYPE_VOIDP = b;
                        collisionEventRight.args[1].arg.TYPE_VOIDP = a;

                        fe::engine::get().getEventSender().sendEngineEvent(collisionEventLeft, a->m_eventOnCollision);
                        fe::engine::get().getEventSender().sendEngineEvent(collisionEventRight, b->m_eventOnCollision);
                    }

                fe::gameEvent collisionEventGeneral(COLLISION, 4);
                collisionEventGeneral.args[0].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                collisionEventGeneral.args[1].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                collisionEventGeneral.args[2].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                collisionEventGeneral.args[3].argType = fe::gameEventArgument::type::TYPE_VOIDP;

                collisionEventGeneral.args[0].arg.TYPE_VOIDP = a;
                collisionEventGeneral.args[1].arg.TYPE_VOIDP = b;
                collisionEventGeneral.args[2].arg.TYPE_VOIDP = &dataFirst;
                collisionEventGeneral.args[3].arg.TYPE_VOIDP = &dataSecond;

                fe::engine::get().getEventSender().sendEngineEvent(collisionEventGeneral, COLLISION);
            }
        FE_END_PROFILE;
    }

void fe::collisionWorld::handleCollision(void *leftCollider, void *rightCollider)
    {
        if (leftCollider == rightCollider) return;
        handleCollision(static_cast<fe::collider*>(leftCollider), static_cast<fe::collider*>(rightCollider));
    }

void fe::collisionWorld::handleCollision(void *collider, fe::str event)
    {
        if (collider == nullptr) return;
        fe::collisionData a;
        a.m_colliderPositionX = 0.f;
        a.m_colliderPositionY = 0.f;
        a.m_penetrationX = 0.f;
        a.m_penetrationY = 0.f;

        fe::collider *test = static_cast<fe::collider*>(collider);

        fe::gameEvent eventObj(event, 2);
        eventObj.args[0].arg.TYPE_VOIDP = collider;
        eventObj.args[0].argType = gameEventArgument::type::TYPE_VOIDP;

        eventObj.args[1].arg.TYPE_VOIDP = &a; // we can send this without worrying about data corruption since the event is sent immediately
        eventObj.args[1].argType = gameEventArgument::type::TYPE_VOIDP;

        fe::engine::get().getEventSender().sendEngineEvent(eventObj, event);
        static_cast<fe::collider*>(collider)->m_collisionCallback(a);
    }

fe::collisionWorld::collisionWorld() : m_maxPointIndex(0)
    {
    }

void fe::collisionWorld::startUp()
    {
        m_collisionBodies.startUp(FE_MAX_GAME_OBJECTS);
    }

void fe::collisionWorld::shutDown()
    {
        clear();
    }

void fe::collisionWorld::clear()
    {
        m_collisionBodies.clear();
    }

void fe::collisionWorld::queryPoint(fe::lightVector2d point, fe::str event)
    {
        m_pointCollision[m_maxPointIndex].event = event;
        m_pointCollision[m_maxPointIndex++].position = point;
    }

void fe::collisionWorld::queryPoint(fe::Vector2d point, fe::str event)
    {
        queryPoint(fe::lightVector2d(point), event);
    }

void fe::collisionWorld::queryPoint(float x, float y, fe::str event)
    {
        queryPoint(fe::lightVector2d(x, y), event);
    }

void fe::collisionWorld::handleCollisions(const fe::broadphaseAbstract *broadphase)
    {
        if (!broadphase)
            {
                // Nothing
            }
        else
            {
                FE_ENGINE_PROFILE("collision_world", "broadphase_compute_partial");
                for (unsigned int i = 0; i < m_collisionBodies.getObjectAllocCount(); i++)
                    {
                        if (!m_collisionBodies.at(i)->m_static && m_collisionBodies.at(i)->m_moved)
                            {
                                broadphase->colliderAABB(m_collisionBodies.at(i)->m_aabb, [this, i] (void *otherCollider) { handleCollision(m_collisionBodies.at(i), otherCollider); });
                            }
                        m_collisionBodies.at(i)->m_moved = false;
                    }
                FE_END_PROFILE;

                FE_ENGINE_PROFILE("collision_world", "broadphase_compute_point_collision_partial");
                for (unsigned int i = 0; i < m_maxPointIndex; i++)
                    {
                        handleCollision(broadphase->colliderAtPoint(m_pointCollision[i].position), m_pointCollision[i].event);
                    }
                FE_END_PROFILE;
            }
        m_maxPointIndex = 0;
    }

void fe::collisionWorld::handleCollisions(const fe::broadphaseAbstract *broadphaseDynamic, const fe::broadphaseAbstract *broadphaseStatic)
    {
        if (!broadphaseStatic)
            {
                handleCollisions(broadphaseDynamic);
            }
        else if (!broadphaseDynamic)
            {
                handleCollisions(broadphaseStatic);
            }
        else
            {
                FE_ENGINE_PROFILE("collision_world", "broadphase_compute_full");
                for (unsigned int i = 0; i < m_collisionBodies.getObjectAllocCount(); i++)
                    {
                        if (!m_collisionBodies.at(i)->m_static)
                            {
                                auto func = std::bind(static_cast<void(fe::collisionWorld::*)(void*, void*)>(&fe::collisionWorld::handleCollision), this, std::placeholders::_1, m_collisionBodies.at(i));
                                broadphaseDynamic->colliderAABB(m_collisionBodies.at(i)->m_aabb, func);
                                broadphaseStatic->colliderAABB(m_collisionBodies.at(i)->m_aabb, func);
                            }
                    }
                FE_END_PROFILE;

                FE_ENGINE_PROFILE("collision_world", "broadphase_compute_point_collision_full");
                for (unsigned int i = 0; i < m_maxPointIndex; i++)
                    {
                        handleCollision(broadphaseDynamic->colliderAtPoint(m_pointCollision[i].position), m_pointCollision[i].event);
                        handleCollision(broadphaseStatic->colliderAtPoint(m_pointCollision[i].position), m_pointCollision[i].event);
                    }
                FE_END_PROFILE;
            }
        m_maxPointIndex = 0;
    }

fe::collider *fe::collisionWorld::createCollider(float sizeX, float sizeY)
    {
        fe::collider *collider = m_collisionBodies.alloc();
        collider->m_aabb.m_sizeX = sizeX;
        collider->m_aabb.m_sizeY = sizeY;
        return collider;
    }

void fe::collisionWorld::deleteCollider(fe::collider *body)
    {
        m_collisionBodies.free(body);
    }
