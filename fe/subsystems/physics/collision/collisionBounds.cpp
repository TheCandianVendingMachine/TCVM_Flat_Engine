#include "collisionBounds.hpp"
#include <algorithm>

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

fe::AABB fe::AABB::merge(const fe::AABB *other)
    {
        fe::AABB aabb;
        aabb.m_positionX = std::min(m_positionX, other->m_positionX);
        aabb.m_positionY = std::min(m_positionY, other->m_positionY);

        float lowestMaxX = m_positionX <  other->m_positionX ? m_positionX : other->m_positionX;
        float highestMinX = m_positionX > other->m_positionX ? m_positionX : other->m_positionX;

        float lowestMaxY =  m_positionY < other->m_positionY ? m_positionY : other->m_positionY;
        float highestMinY = m_positionY > other->m_positionY ? m_positionY : other->m_positionY;

        aabb.m_sizeX = m_positionX + other->m_sizeX + (highestMinX - lowestMaxX);
        aabb.m_sizeY = m_positionY + other->m_sizeY + (highestMinY - lowestMaxY);
    }

float fe::AABB::volume()
    {
        return m_sizeX * m_sizeY;
    }
