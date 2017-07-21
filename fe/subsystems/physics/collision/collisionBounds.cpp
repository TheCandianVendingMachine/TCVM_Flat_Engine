#include "collisionBounds.hpp"
#include <algorithm>

bool fe::AABB::intersects(const collisionBounds *other) const
    {
        return intersects(static_cast<const fe::AABB*>(other));
    }

bool fe::AABB::contains(const collisionBounds *other) const
    {
        return contains(static_cast<const fe::AABB*>(other));
    }

bool fe::AABB::intersects(const fe::AABB *other) const
    {
        if (m_positionX + m_sizeX < other->m_positionX || m_positionX > other->m_positionX + other->m_sizeX) return false;
        if (m_positionY + m_sizeY < other->m_positionY || m_positionY > other->m_positionY + other->m_sizeY) return false;
        return true;
    }

bool fe::AABB::contains(const fe::AABB *other) const
    {
        return  (other->m_positionX >= m_positionX && other->m_positionX + other->m_sizeX <= m_positionX + m_sizeX) &&
                (other->m_positionY >= m_positionY && other->m_positionY + other->m_sizeY <= m_positionY + m_sizeY);
    }

fe::AABB fe::AABB::merge(const fe::AABB *other)
    {
        fe::AABB aabb;
        aabb.m_positionX = std::min(m_positionX, other->m_positionX);
        aabb.m_positionY = std::min(m_positionY, other->m_positionY);

        float highestMinX = std::max(m_positionX, other->m_positionX);
        float highestMinY = std::max(m_positionY, other->m_positionY);

        aabb.m_sizeX = m_positionX + other->m_sizeX + (highestMinX - aabb.m_positionX);
        aabb.m_sizeY = m_positionY + other->m_sizeY + (highestMinY - aabb.m_positionY);
        return aabb;
    }

float fe::AABB::volume() const
    {
        return m_sizeX * m_sizeY;
    }
