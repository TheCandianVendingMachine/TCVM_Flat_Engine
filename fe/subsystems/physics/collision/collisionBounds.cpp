#include "collisionBounds.hpp"

bool fe::AABB::intersects(const fe::AABB *other)
    {
        return  (other->m_positionX >= m_positionX && other->m_positionX < m_positionX + m_sizeX) &&
                (other->m_positionY >= m_positionY && other->m_positionY < m_positionY + m_sizeY);
    }

bool fe::AABB::contains(const fe::AABB *other)
    {
        return  (other->m_positionX >= m_positionX && other->m_positionX + other->m_sizeX < m_positionX + m_sizeX) &&
                (other->m_positionY >= m_positionY && other->m_positionY + other->m_sizeY < m_positionY + m_sizeY);
    }
