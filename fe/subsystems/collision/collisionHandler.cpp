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
        auto it1 = m_colliders.begin();
        auto it2 = it1 + 1;

        for (; it1 != m_colliders.end(); ++it1)
            {
                it2 = it1 + 1;
                for (; it2 != m_colliders.end(); ++it2)
                    {
                        if ((*it2)->collide(**it1))
                            {
                                (*it1)->operator()(**it2);
                                (*it2)->operator()(**it1);
                            }
                    }
            }
    }