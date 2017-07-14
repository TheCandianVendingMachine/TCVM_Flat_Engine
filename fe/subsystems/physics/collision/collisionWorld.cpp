#include "collisionWorld.hpp"

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

void fe::collisionWorld::setBroadphase(fe::broadphaseAbstract *broadphase)
    {
        m_broadphase = broadphase;
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
                                        handleCollision(a, b);
                                    }
                            }
                    }
            }
        else
            {
                auto pairs = m_broadphase->computeColliderPairs();
                for (auto &pair : pairs)
                    {
                        handleCollision(pair.first, pair.second);
                    }
            }
    }

fe::collider *fe::collisionWorld::createCollider()
    {
        return m_collisionBodies.alloc();
    }

void fe::collisionWorld::deleteCollider(fe::collider *body)
    {
        m_collisionBodies.free(body);
    }
