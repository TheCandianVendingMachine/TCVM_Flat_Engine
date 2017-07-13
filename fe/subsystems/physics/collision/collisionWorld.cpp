#include "collisionWorld.hpp"

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

void fe::collisionWorld::handleCollisions()
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
                                fe::lightVector2d positionA(a->m_positionX, a->m_positionY);
                                fe::lightVector2d sizeA = a->m_colliderSize.m_squareSize;

                                fe::lightVector2d positionB(b->m_positionX, b->m_positionY);
                                fe::lightVector2d sizeB = b->m_colliderSize.m_squareSize;

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

                                    m_collisionData[i].m_penetrationX = (distance.x > 0 ? minDistance.x - distance.x : -minDistance.x - distance.x) / 2.f;
                                    m_collisionData[i].m_penetrationY = (distance.y > 0 ? minDistance.y - distance.y : -minDistance.y - distance.y) / 2.f;

                                    m_collisionData[j].m_penetrationX = -(distance.x > 0 ? minDistance.x - distance.x : -minDistance.x - distance.x) / 2.f;
                                    m_collisionData[j].m_penetrationY = -(distance.y > 0 ? minDistance.y - distance.y : -minDistance.y - distance.y) / 2.f;

                                    a->m_collisonCallback(m_collisionData[i]);
                                    b->m_collisonCallback(m_collisionData[j]);
                                }
                            }
                    }
            }
    }

fe::collisionBody *fe::collisionWorld::createCollider()
    {
        return m_collisionBodies.alloc();
    }

void fe::collisionWorld::deleteCollider(fe::collisionBody *body)
    {
        m_collisionBodies.free(body);
    }
