#include "collisionWorld.hpp"
#include "../../../debug/profiler.hpp"
#include "../../../engine.hpp"
#include "../../threading/threadPool.hpp"
#include "aabbTree.hpp"
#include <functional>

void fe::collisionWorld::handleCollision(fe::collider *a, fe::collider *b)
    {
        fe::AABB *first = static_cast<fe::AABB*>(a->m_bounds);
        fe::AABB *second = static_cast<fe::AABB*>(b->m_bounds);

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
                dataFirst.m_colliderPositionX = a->m_bounds->m_positionX;
                dataFirst.m_colliderPositionY = a->m_bounds->m_positionY;

                dataSecond.m_penetrationX = -(distance.x > 0 ? minDistance.x - distance.x : -minDistance.x - distance.x) / 2.f;
                dataSecond.m_penetrationY = -(distance.y > 0 ? minDistance.y - distance.y : -minDistance.y - distance.y) / 2.f;
                dataSecond.m_colliderPositionX = b->m_bounds->m_positionX;
                dataSecond.m_colliderPositionY = b->m_bounds->m_positionY;

                a->m_collisionCallback(dataFirst);
                b->m_collisionCallback(dataSecond);
            }
    }

void fe::collisionWorld::handleCollision(void *leftCollider, void *rightCollider)
    {
        if (leftCollider == rightCollider) return;
        handleCollision(static_cast<fe::collider*>(leftCollider), static_cast<fe::collider*>(rightCollider));
    }

fe::collisionWorld::collisionWorld()
    {
    }

void fe::collisionWorld::startUp()
    {
        m_collisionBodies.startUp(FE_MAX_GAME_OBJECTS);
        m_broadphase = nullptr;
    }

void fe::collisionWorld::shutDown()
    {
        clear();
    }

void fe::collisionWorld::clear()
    {
        m_collisionBodies.clear();
    }

void fe::collisionWorld::updateCollider(fe::collider *collider)
    {
        if (!collider) return;
        m_broadphase->update(collider);
    }

void fe::collisionWorld::setBroadphase(fe::broadphaseAbstract *broadphase)
    {
        m_broadphase = broadphase;
        m_broadphase->startUp();
    }

fe::broadphaseAbstract *fe::collisionWorld::getBroadphase() const
    {
        return m_broadphase;
    }

void fe::collisionWorld::handleCollisions()
    {
        if (!m_broadphase)
            {
                for (int i = 0; i < m_collisionBodies.getObjectAllocCount(); i++)
                    {
                        for (int j = i + 1; j < m_collisionBodies.getObjectAllocCount(); j++)
                            {
                                auto a = m_collisionBodies.at(i);
                                auto b = m_collisionBodies.at(j);

                                if (!a || !b)
                                    {
                                        break;
                                    }
                                else
                                    {
                                        FE_ENGINE_PROFILE("collison_world", "handle_collisions_no_broadphase");
                                        handleCollision(a, b);
                                        FE_END_PROFILE;
                                    }
                            }
                    }
            }
        else
            {
                FE_ENGINE_PROFILE("collision_world", "broadphase_compute");
                for (int i = 0; i < m_collisionBodies.getObjectAllocCount(); i++)
                    {
                        m_broadphase->colliderAABB(m_collisionBodies.at(i)->m_aabb, std::bind(static_cast<void(fe::collisionWorld::*)(void*, void*)>(&fe::collisionWorld::handleCollision), this, std::placeholders::_1, m_collisionBodies.at(i)));
                    }
                FE_END_PROFILE;
                m_broadphase->debugDraw();
            }
    }

fe::collider *fe::collisionWorld::createCollider(float sizeX, float sizeY)
    {
        fe::collider *collider = m_collisionBodies.alloc();
        collider->m_aabb.m_sizeX = sizeX;
        collider->m_aabb.m_sizeY = sizeY;
        if (m_broadphase)
            {
                m_broadphase->add(collider);
            }
        return collider;
    }

void fe::collisionWorld::deleteCollider(fe::collider *body)
    {
        if (m_broadphase)
            {
                m_broadphase->remove(body);
            }
        m_collisionBodies.free(body);
    }
