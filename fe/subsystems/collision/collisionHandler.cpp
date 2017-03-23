#include "collisionHandler.hpp"
#include "../../collision/colliders.hpp"
#include <algorithm>

void fe::collisonHandler::add(const collider *collider)
    {
        m_colliders.push_back(collider);
    }
 
void fe::collisonHandler::remove(const collider *collider)
    {
        m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), collider), m_colliders.end());
    }
 
void fe::collisonHandler::handleCollisions()
    {
        for (auto &collider : m_colliders)
            {
                for (auto &testCollider : m_colliders)
                    {
                        if (collider != testCollider)
                            {
                                if (collider->collide(*testCollider))
                                    {
                                        (*collider)(*testCollider);
                                    }
                            }
                    }
            }
    }