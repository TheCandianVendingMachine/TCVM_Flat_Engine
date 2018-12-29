#include "fe/subsystems/collision/collisionWorld.hpp"
#include "fe/subsystems/collision/aabbTree.hpp"
#include "fe/subsystems/collision/bounds/aabbTests.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/engine.hpp"
#include "fe/subsystems/threading/threadPool.hpp"
#include "fe/subsystems/messaging/gameEvent.hpp"
#include "fe/subsystems/messaging/eventSender.hpp"
#include "fe/engineEvents.hpp"

#include <functional>

void fe::collisionWorld::handleCollision(fe::collider *a, fe::collider *b)
    {
        FE_ENGINE_PROFILE("collision_world", "collision_check");
        if (a == b || !a || !b) return;
        fe::AABB *first = static_cast<fe::AABB*>(&a->m_aabb);
        fe::AABB *second = static_cast<fe::AABB*>(&b->m_aabb);

        fe::collisionData dataFirst;
        fe::collisionData dataSecond;

        fe::lightVector2d positionA(first->m_globalPositionX + first->m_offsetX, first->m_globalPositionY + first->m_offsetY);
        fe::lightVector2d sizeA(first->m_sizeX, first->m_sizeY);
        fe::lightVector2d cornersA[4] = {
            positionA,
            positionA + fe::lightVector2d(sizeA.x, 0),
            positionA + sizeA,
            positionA + fe::lightVector2d(0, sizeA.y)
        };

        fe::lightVector2d positionB(second->m_globalPositionX + second->m_offsetX, second->m_globalPositionY + second->m_offsetY);
        fe::lightVector2d sizeB(second->m_sizeX, second->m_sizeY);
        fe::lightVector2d cornersB[4] = {
            positionB,
            positionB + fe::lightVector2d(sizeB.x, 0),
            positionB + sizeB,
            positionB + fe::lightVector2d(0, sizeB.y)
        };

        if (std::abs(first->perimeter()) <= 0.f || std::abs(second->perimeter()) <= 0.f)
            {
                return;
            }

        if (fe::intersects(*first, *second))
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
                dataFirst.m_colliderPositionX = positionB.x;
                dataFirst.m_colliderPositionY = positionB.y;
                
                fe::Vector2d edgeCollided;
                int leftHanded = 0;
                if (std::abs(dataFirst.m_penetrationX) < std::abs(dataFirst.m_penetrationY))
                    {
                        if (dataFirst.m_penetrationX < 0.f)
                            {
                                // Left
                                edgeCollided = (cornersB[2] - cornersB[1]);
                                leftHanded = 1;
                            }
                        else if (dataFirst.m_penetrationX > 0.f)
                            {
                                // Right
                                edgeCollided = (cornersB[3] - cornersB[0]);
                                leftHanded = -1;
                            }
                    }
                else
                    {
                        if (dataFirst.m_penetrationY < 0.f)
                            {
                                // Top
                                edgeCollided = (cornersB[0] - cornersB[1]);
                                leftHanded = 1;
                            }
                        else if (dataFirst.m_penetrationY > 0.f)
                            {
                                // Down
                                edgeCollided = (cornersB[3] - cornersB[2]);
                                leftHanded = -1;
                            }
                    }

                fe::lightVector2d normal = edgeCollided.normal().normalize() * leftHanded;
                dataFirst.m_normalX = normal.x;
                dataFirst.m_normalY = normal.y;

                dataSecond.m_penetrationX = -(distance.x > 0 ? minDistance.x - distance.x : -minDistance.x - distance.x) / 2.f;
                dataSecond.m_penetrationY = -(distance.y > 0 ? minDistance.y - distance.y : -minDistance.y - distance.y) / 2.f;
                dataSecond.m_colliderPositionX = positionA.x;
                dataSecond.m_colliderPositionY = positionA.y;

                if (std::abs(dataSecond.m_penetrationX) < std::abs(dataSecond.m_penetrationY))
                    {
                        if (dataSecond.m_penetrationX < 0.f)
                            {
                                // Left
                                edgeCollided = (cornersA[2] - cornersA[1]);
                                leftHanded = -1;
                            }
                        else if (dataSecond.m_penetrationX > 0.f)
                            {
                                // Right
                                edgeCollided = (cornersA[0] - cornersA[3]);
                                leftHanded = 1;
                            }
                    }
                else
                    {
                        if (dataSecond.m_penetrationY < 0.f)
                            {
                                // Top
                                edgeCollided = (cornersA[0] - cornersA[1]);
                                leftHanded = 1;
                            }
                        else if (dataSecond.m_penetrationY > 0.f)
                            {
                                // Down
                                edgeCollided = (cornersA[3] - cornersA[2]);
                                leftHanded = -1;
                            }
                    }
                normal = edgeCollided.normal().normalize() * leftHanded;
                dataSecond.m_normalX = normal.x;
                dataSecond.m_normalY = normal.y;

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

                m_collisionPairs[m_collisionPairIndex + 0].collider = a;
                m_collisionPairs[m_collisionPairIndex + 0].data = dataFirst;
                m_collisionPairs[m_collisionPairIndex + 1].collider = b;
                m_collisionPairs[m_collisionPairIndex + 1].data = dataSecond;
                m_collisionPairIndex += 2;
            }
        FE_END_PROFILE;
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

void fe::collisionWorld::cullCollisionPairs()
    {
        FE_ENGINE_PROFILE("collision_world", "cull_collision_pairs");
        for (unsigned int i = 0; i < m_collisionPairIndex; i += 2)
            {
                for (unsigned int j = i + 2; j < m_collisionPairIndex; j += 2)
                    {
                        // If the collision pairs have the same data like this, then the colliders are flipped
                        // and duplicate collider pairs exist
                        if (m_collisionPairs[i + 0].collider == m_collisionPairs[j + 1].collider &&
                            m_collisionPairs[i + 1].collider == m_collisionPairs[j + 0].collider)
                            {
                                m_collisionPairs[i + 0].collider = nullptr;
                                m_collisionPairs[i + 1].collider = nullptr;
                                break;
                            }
                    }
            }
        FE_END_PROFILE;
    }

void fe::collisionWorld::handleCollisionPairs()
    {
        FE_ENGINE_PROFILE("collision_world", "handle_collision_pairs");
        for (unsigned int i = 0; i < m_collisionPairIndex; i += 2)
            {
                if (!m_collisionPairs[i + 0].collider || !m_collisionPairs[i + 1].collider)
                    {
                        continue;
                    }
                fe::collider *a = m_collisionPairs[i + 0].collider;
                fe::collider *b = m_collisionPairs[i + 1].collider;

                a->m_collisionCallback(m_collisionPairs[i + 0].data);
                b->m_collisionCallback(m_collisionPairs[i + 1].data);

                if (a->m_eventOnCollision != 0)
                    {
                        fe::gameEvent collisionEventLeft(a->m_eventOnCollision, 2);
                        collisionEventLeft.args[0].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                        collisionEventLeft.args[1].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                        collisionEventLeft.args[2].argType = fe::gameEventArgument::type::TYPE_UINT;
                        collisionEventLeft.args[3].argType = fe::gameEventArgument::type::TYPE_UINT;

                        collisionEventLeft.args[0].arg.TYPE_VOIDP = a;
                        collisionEventLeft.args[1].arg.TYPE_VOIDP = b;
                        collisionEventLeft.args[2].arg.TYPE_UINTEGER = a->m_collisionGroup;
                        collisionEventLeft.args[3].arg.TYPE_UINTEGER = b->m_collisionGroup;

                        fe::engine::get().getEventSender().sendEngineEvent(collisionEventLeft, a->m_eventOnCollision);
                    }

                if (b->m_eventOnCollision != 0)
                    {
                        fe::gameEvent collisionEventLeft(b->m_eventOnCollision, 2);
                        collisionEventLeft.args[0].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                        collisionEventLeft.args[1].argType = fe::gameEventArgument::type::TYPE_VOIDP;
                        collisionEventLeft.args[2].argType = fe::gameEventArgument::type::TYPE_UINT;
                        collisionEventLeft.args[3].argType = fe::gameEventArgument::type::TYPE_UINT;

                        collisionEventLeft.args[0].arg.TYPE_VOIDP = b;
                        collisionEventLeft.args[1].arg.TYPE_VOIDP = a;
                        collisionEventLeft.args[2].arg.TYPE_UINTEGER = b->m_collisionGroup;
                        collisionEventLeft.args[3].arg.TYPE_UINTEGER = a->m_collisionGroup;
                        fe::engine::get().getEventSender().sendEngineEvent(collisionEventLeft, b->m_eventOnCollision);
                    }
            }
        FE_END_PROFILE;
    }

void fe::collisionWorld::handlePointCollisions(const fe::broadphaseAbstract *broadphase)
    {
        FE_ENGINE_PROFILE("collision_world", "broadphase_compute_point_collision_partial");
        for (unsigned int i = 0; i < m_maxPointIndex; i++)
            {
                handleCollision(broadphase->colliderAtPoint(m_pointCollision[i].position), m_pointCollision[i].event);
            }
        FE_END_PROFILE;
    }

void fe::collisionWorld::handlePointCollisions(const fe::broadphaseAbstract *broadphase0, const fe::broadphaseAbstract *broadphase1)
    {
        FE_ENGINE_PROFILE("collision_world", "broadphase_compute_point_collision_full");
        for (unsigned int i = 0; i < m_maxPointIndex; i++)
            {
                handleCollision(broadphase0->colliderAtPoint(m_pointCollision[i].position), m_pointCollision[i].event);
                handleCollision(broadphase1->colliderAtPoint(m_pointCollision[i].position), m_pointCollision[i].event);
            }
        FE_END_PROFILE;
    }

fe::collisionWorld::collisionWorld() : m_maxPointIndex(0), m_collisionPairIndex(0)
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
                        auto body = m_collisionBodies.at(i);
                        if (!m_collisionBodies.at(i)->m_static && m_collisionBodies.at(i)->m_moved && m_collisionBodies.at(i)->m_enabled)
                            {
                                broadphase->colliderAABB(m_collisionBodies.at(i)->m_aabb, [this, i] (fe::collider *otherCollider) { handleCollision(m_collisionBodies.at(i), otherCollider); });
                            }
                        m_collisionBodies.at(i)->m_moved = false;
                    }
                FE_END_PROFILE;

                cullCollisionPairs();
                handleCollisionPairs();
                handlePointCollisions(broadphase);

            }
        m_collisionPairIndex = 0;
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
                        auto body = m_collisionBodies.at(i);
                        if (!m_collisionBodies.at(i)->m_static && m_collisionBodies.at(i)->m_moved && m_collisionBodies.at(i)->m_enabled)
                            {
                                auto func = [this, i](fe::collider *otherCollider) { handleCollision(m_collisionBodies.at(i), otherCollider); };
                                broadphaseDynamic->colliderAABB(m_collisionBodies.at(i)->m_aabb, func);
                                broadphaseStatic->colliderAABB(m_collisionBodies.at(i)->m_aabb, func);
                                m_collisionBodies.at(i)->m_moved = false;
                            }
                    }
                FE_END_PROFILE;

                cullCollisionPairs();
                handleCollisionPairs();
                handlePointCollisions(broadphaseDynamic, broadphaseStatic);
            }
        m_collisionPairIndex = 0;
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
