#include "collisionWorld.hpp"
#include "../../../debug/profiler.hpp"
#include "../../../engine.hpp"
#include "../../threading/threadPool.hpp"
#include "aabbTree.hpp"
#include <functional>

void fe::collisionWorld::handleCollision(fe::collider *a, fe::collider *b)
    {
        FE_ENGINE_PROFILE("collision_world", "collision_check");
        fe::AABB *first = static_cast<fe::AABB*>(&a->m_aabb);
        fe::AABB *second = static_cast<fe::AABB*>(&b->m_aabb);

        fe::collisionData dataFirst;
        fe::collisionData dataSecond;

        fe::lightVector2d positionA(first->m_positionX, first->m_positionY);
        fe::lightVector2d sizeA(first->m_sizeX, first->m_sizeY);

        fe::lightVector2d positionB(second->m_positionX, second->m_positionY);
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
                dataFirst.m_colliderPositionX = a->m_aabb.m_positionX;
                dataFirst.m_colliderPositionY = a->m_aabb.m_positionY;

                dataSecond.m_penetrationX = -(distance.x > 0 ? minDistance.x - distance.x : -minDistance.x - distance.x) / 2.f;
                dataSecond.m_penetrationY = -(distance.y > 0 ? minDistance.y - distance.y : -minDistance.y - distance.y) / 2.f;
                dataSecond.m_colliderPositionX = b->m_aabb.m_positionX;
                dataSecond.m_colliderPositionY = b->m_aabb.m_positionY;

                a->m_collisionCallback(dataFirst);
                b->m_collisionCallback(dataSecond);
            }
        FE_END_PROFILE;
    }

void fe::collisionWorld::handleCollision(void *leftCollider, void *rightCollider)
    {
        if (leftCollider == rightCollider) return;
        fe::collider *left = static_cast<fe::collider*>(leftCollider);
        fe::collider *right = static_cast<fe::collider*>(rightCollider);

        handleCollision(static_cast<fe::collider*>(left), static_cast<fe::collider*>(right));

    }

void fe::collisionWorld::handleCollision(void *collider)
    {
        if (collider == nullptr) return;
        fe::collisionData a;
        a.m_colliderPositionX = 0.f;
        a.m_colliderPositionY = 0.f;
        a.m_penetrationX = 0.f;
        a.m_penetrationY = 0.f;
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

void fe::collisionWorld::queryPoint(fe::lightVector2d point)
    {
        m_pointCollision[m_maxPointIndex++] = point;
    }

void fe::collisionWorld::queryPoint(fe::Vector2d point)
    {
        queryPoint(fe::lightVector2d(point));
    }

void fe::collisionWorld::queryPoint(float x, float y)
    {
        queryPoint(fe::lightVector2d(x, y));
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
                        if (!m_collisionBodies.at(i)->m_static)
                            {
                                broadphase->colliderAABB(
                                    m_collisionBodies.at(i)->m_aabb, 
                                    std::bind(static_cast<void(fe::collisionWorld::*)(void*, void*)>(&fe::collisionWorld::handleCollision), this, std::placeholders::_1, m_collisionBodies.at(i)));
                            }
                    }
                FE_END_PROFILE;

                FE_ENGINE_PROFILE("collision_world", "broadphase_compute_point_collision_partial");
                for (unsigned int i = 0; i < m_maxPointIndex; i++)
                    {
                        handleCollision(broadphase->colliderAtPoint(m_pointCollision[i]));
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
                        handleCollision(broadphaseDynamic->colliderAtPoint(m_pointCollision[i]));
                        handleCollision(broadphaseStatic->colliderAtPoint(m_pointCollision[i]));
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
