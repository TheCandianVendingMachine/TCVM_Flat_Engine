#include "collisionHandler.hpp"
#include <algorithm>

#include "../../collision/colliders.hpp"

void fe::collisonHandler::add(collider *collider)
    {
        m_colliders.push_back(collider);
    }

void fe::collisonHandler::remove(collider *collider)
    {
        m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), collider), m_colliders.end());
    }

void fe::collisonHandler::handleCollisions()
    {
    }
