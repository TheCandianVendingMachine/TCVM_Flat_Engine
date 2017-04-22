#include "collisionHandler.hpp"
#include "../../collision/colliders.hpp"
#include "../memory/memoryManager.hpp"
#include <algorithm>

fe::collisonHandler::collisonHandler()
    {}

void fe::collisonHandler::remove(const collider *collider)
    {
        m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), collider), m_colliders.end());
    }
 
void fe::collisonHandler::handleCollisions()
    {
        if (m_colliders.empty()) return;

        auto it1 = m_colliders.begin();
        auto it2 = it1 + 1;

        for (; it1 != m_colliders.end(); ++it1)
            {
                if ((*it1)->m_collisonEnabled)
                    {
                        it2 = it1 + 1;
                        for (; it2 != m_colliders.end(); ++it2)
                            {
                                if ((*it2)->m_collisonEnabled && (*it2)->collide(**it1))
                                    {
                                        (*it1)->operator()(**it2);
                                        (*it2)->operator()(**it1);
                                    }
                            }
                    }
            }
    }

fe::collisonHandler::~collisonHandler()
    {
        for (auto &collider : m_colliders)
            {
                delete collider;
            }
    }
