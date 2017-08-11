#include "collisionWorld.hpp"
#include "../../../debug/profiler.hpp"
#include "../../../engine.hpp"
#include "../../threading/threadPool.hpp"
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

fe::collisionWorld::collisionWorld() : m_collisionWorldUpdate([this]() { m_broadphase->update();  return true; })
    {
    }

void fe::collisionWorld::startUp()
    {
        m_collisionBodies.startUp(FE_MAX_GAME_OBJECTS);
        m_broadphase = nullptr;
        m_threaded = false;
    }

void fe::collisionWorld::shutDown()
    {
        clear();
    }

void fe::collisionWorld::clear()
    {
        m_collisionBodies.clear();
    }

void fe::collisionWorld::update()
    {
        if (m_collisionBodies.getObjectAllocCount() > 2000)
            {
                fe::engine::get().getThreadPool().addJob(m_collisionWorldUpdate);
                m_threaded = true;
            }
        else
            {
                m_threaded = false;
            }
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
                if (m_threaded)
                    {
                        FE_ENGINE_PROFILE("collision_world", "wait_for_broadphase_update");
                        fe::engine::get().getThreadPool().waitFor(m_collisionWorldUpdate);
                        FE_END_PROFILE;
                    }
                else
                    {
                        FE_ENGINE_PROFILE("collision_world", "broadphase_update");
                        m_broadphase->update();
                        FE_END_PROFILE;
                    }
                std::pair<std::pair<fe::collider*, fe::collider*>*, unsigned int> pairs;
                FE_ENGINE_PROFILE("collision_world", "broadphase_pair_compute");
                pairs = m_broadphase->computeColliderPairs();
                FE_END_PROFILE;
                for (unsigned int i = 0; i < pairs.second; i++)
                    {
                        FE_ENGINE_PROFILE("collison_world", "handle_collisions_broadphase");
                        handleCollision(pairs.first[i].first, pairs.first[i].second);
                        FE_END_PROFILE;
                    }
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
